/** @FspiDxeParseDt.c

  Functions for retrieving platform specific info from device tree and install protocols

  Copyright 2018 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the bsd
  license which accompanies this distribution. the full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Library/BeIoLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ItbParse.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "FspiDxe.h"

FSPI_MASTER  mFspiMasterTemplate = {
  .Handle = NULL,

  .Regs = NULL,
  .AmbaBase = 0,
  .CurCSOffset = 0,
  .NumChipselect = 0,

  .FspiHcProtocol = {
    .Attributes = SPI_SUPPORTS_8_BIT_DATA_BUS_WIDTH |
                  SPI_SUPPORTS_4_BIT_DATA_BUS_WIDTH |
                  SPI_SUPPORTS_2_BIT_DATA_BUS_WIDTH |
                  SPI_HALF_DUPLEX |
                  SPI_TRANSFER_SIZE_INCLUDES_OPCODE |
                  SPI_TRANSFER_SIZE_INCLUDES_ADDRESS,
    .FrameSizeSupportMask = SPI_FRAME_MASK (8), // 8-bit frames only
    .MaximumTransferBytes = SIZE_64KB + sizeof (UINT32), // 0xFFFF - IPCR1[IDATSZ] + 1 byte opcode + 4 bytes address
    .ChipSelect = FspiChipSelect,
    .Clock = FspiClock,
    .Transaction = FspiTransaction,
    .UpdateSpiPeripheral = FspiUpdateSpiPeripheral
  }, // EFI_SPI_HC_PROTOCOL

  .DevicePath = {
    .Vendor = {
      .Header = {
        .Type = HARDWARE_DEVICE_PATH,
        .SubType = HW_VENDOR_DP,
        .Length = {
          (UINT8)sizeof(VENDOR_DEVICE_PATH),
          (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
        }
      },
      .Guid = EFI_CALLER_ID_GUID
    },
    .Controller = {
      .Header = {
        .Type = HARDWARE_DEVICE_PATH,
        .SubType = HW_CONTROLLER_DP,
        .Length = {
          (UINT8) (sizeof (CONTROLLER_DEVICE_PATH)),
          (UINT8) ((sizeof (CONTROLLER_DEVICE_PATH)) >> 8)
        }
      },
      .ControllerNumber = 0
    },
    .End = {
      .Type = END_DEVICE_PATH_TYPE,
      .SubType = END_ENTIRE_DEVICE_PATH_SUBTYPE,
      .Length = { sizeof (EFI_DEVICE_PATH_PROTOCOL), 0 }
    }
  },

  .Read32 = MmioRead32,
  .Write32 = MmioWrite32,
  .ClearBits32 = MmioClearBits32,
  .SetBits32 = MmioSetBits32,
  .ClearSet32 = MmioClearSet32,
  .Event = NULL
};

/**
 This function parses the device tree and for each FSPI controller node found,
 allocates runtime memory for the internal data structure containing a handle and installs
 the SpiMaster Protocol and Device Path protocol on to that handle.

 @param[in]  Fdt                 Platform's device tree blob
 @param[out] FspiList         Link List of FSPI_MASTER structures that are runtime.
                                 each member corresponds to one FSPI controller.
 @param[out] FspiMasterCount     Count of Fspi controllers in Platform

 @retval EFI_UNSUPPORTED         The device tree node properties of FSPI controller are
                                 not supported by driver
 @retval EFI_INVALID_PARAMETER   No Parent node of FSPI node found in device tree.
 @retval EFI_OUT_OF_RESOURCES    No pool memory available
 @retval EFI_ALREADY_STARTED     The controller is busy with another transaction.
 @retval EFI_DEVICE_ERROR        There was an error during installing protocols.
 @retval EFI_SUCCESS             The protocols were installed successfully.
**/
EFI_STATUS
ParseDeviceTree (
  IN  VOID          *Fdt,
  OUT UINTN         *FspiCount
  )
{
  INT32                 NodeOffset;
  CONST fdt32_t         *Prop;
  INT32                 PropLen;
  INT32                 FspiIndex;
  INT32                 FspiMemIndex;
  FSPI_MASTER           *FspiMasterPtr;
  EFI_STATUS            Status;
  UINT64                AmbaBase;
  UINT64                AmbaTotalSize;
  UINT32                FspiMasterIndex;
  UINT64                Regs;
  BOOLEAN               Runtime;
  INT32                 FdtStatus;

  Status = EFI_SUCCESS;
  FspiMasterIndex = 0;
  *FspiCount = 0;

  for  (NodeOffset = fdt_node_offset_by_compatible (Fdt, -1, (VOID *)(PcdGetPtr (PcdFlexSpiFdtCompatible)));
        NodeOffset != -FDT_ERR_NOTFOUND;
        NodeOffset = fdt_node_offset_by_compatible (Fdt, NodeOffset, (VOID *)(PcdGetPtr (PcdFlexSpiFdtCompatible))))
  {
    Prop = fdt_getprop (Fdt, NodeOffset, "status", &PropLen);
    if (Prop == NULL) {
      DEBUG ((DEBUG_WARN, "status property not found\n"));
    } else if (PropLen != (AsciiStrLen ("okay") + 1)
             || !fdt_stringlist_contains ( (CHAR8 *)Prop, PropLen, "okay"))
    {

      // It should say "okay", so only allow that. Some fdts use "ok" but
      // this is a bug. Please fix your device tree source file. See here
      // for discussion:
      //
      // http://www.mail-archive.com/u-boot@lists.denx.de/msg71598.html

      continue;
    }

    FspiIndex = fdt_stringlist_search (Fdt, NodeOffset, "reg-names", "FSPI");
    if (FspiIndex < 0) {
      DEBUG ((DEBUG_WARN, "Error: can't get regs base addresses(ret = %d)!\n", FspiIndex));
      continue;
    }

    Status = FdtGetAddressSize (Fdt, NodeOffset, "reg", FspiIndex, &Regs, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: can't get regs base addresses(ret = %r)!\n", Status));
      continue;
    }

    FspiMemIndex = fdt_stringlist_search (Fdt, NodeOffset, "reg-names", "FSPI-memory");
    if (FspiMemIndex < 0) {
      DEBUG ((DEBUG_WARN, "Error: can't get AMBA base addresses(ret = %d)!\n", FspiMemIndex));
      continue;
    }

    Status = FdtGetAddressSize (Fdt, NodeOffset, "reg", FspiMemIndex, &AmbaBase, &AmbaTotalSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: can't get AMBA base addresses(ret = %r)!\n", Status));
      continue;
    }

    Prop = fdt_getprop (Fdt, NodeOffset, "uefi-runtime", &PropLen);
    if (Prop == NULL) {
      FspiMasterPtr = AllocateCopyPool (sizeof(FSPI_MASTER), &mFspiMasterTemplate);
      Runtime = FALSE;
    } else {
      FspiMasterPtr = AllocateRuntimeCopyPool (sizeof(FSPI_MASTER), &mFspiMasterTemplate);
      Runtime = TRUE;
    }
    if (!FspiMasterPtr) {
      DEBUG ((DEBUG_WARN, "No space left!!\n"));
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    FspiMasterPtr->Regs = (FSPI_REGISTERS *)Regs;
    FspiMasterPtr->AmbaBase = AmbaBase;

    if (fdt_getprop(Fdt, NodeOffset, "big-endian", NULL) != NULL) {
      FspiMasterPtr->Read32 = MmioReadBe32;
      FspiMasterPtr->Write32 = MmioWriteBe32;
      FspiMasterPtr->ClearBits32 = MmioClearBitsBe32;
      FspiMasterPtr->SetBits32 = MmioSetBitsBe32;
      FspiMasterPtr->ClearSet32 = MmioClearSetBe32;
    } else {
      FspiMasterPtr->Read32 = MmioRead32;
      FspiMasterPtr->Write32 = MmioWrite32;
      FspiMasterPtr->ClearBits32 = MmioClearBits32;
      FspiMasterPtr->SetBits32 = MmioSetBits32;
      FspiMasterPtr->ClearSet32 = MmioClearSet32;
    }

    Prop = fdt_getprop(Fdt, NodeOffset, "num-cs", &PropLen);

    if (Prop == NULL) {
      FspiMasterPtr->NumChipselect = FSPI_CHIP_SELECT_MAX;
    } else {
      FspiMasterPtr->NumChipselect = fdt32_to_cpu(*Prop);
      if (FspiMasterPtr->NumChipselect > FSPI_CHIP_SELECT_MAX) {
        DEBUG ((DEBUG_WARN, "Warning: More than supported (%d) Chipselect Number %u!\n",
               FSPI_CHIP_SELECT_MAX, FspiMasterPtr->NumChipselect));
        FspiMasterPtr->NumChipselect = FSPI_CHIP_SELECT_MAX;
      }
    }

    Prop = fdt_getprop(Fdt, NodeOffset, "bus-num", &PropLen);
    if (Prop == NULL) {
      FspiMasterPtr->DevicePath.Controller.ControllerNumber = FspiMasterIndex;
    } else {
      FspiMasterPtr->DevicePath.Controller.ControllerNumber = fdt32_to_cpu(*Prop);
    }

    FspiMasterIndex++;

    // Setup FSPI Module
    Status = FspiSetup (FspiMasterPtr, AmbaTotalSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Fspi Setup failed!!\n"));
      FreePool (FspiMasterPtr);
      continue;
    }

    // Install SPI Host controller protocol and Device Path Protocol
    Status = FspiInstallProtocol (FspiMasterPtr, Runtime);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Fspi Spi Host Controller Installation Failed!!\n"));
      FreePool (FspiMasterPtr);
      continue;
    }

    if (Runtime) {
      FdtStatus = fdt_setprop_string (Fdt, NodeOffset, "status", "disabled");
      if (FdtStatus) {
        DEBUG ((DEBUG_ERROR, "Error %a disabling Node %a\n",
          fdt_strerror (FdtStatus), fdt_get_name (Fdt, NodeOffset, NULL)));
      }
    }

    *FspiCount += 1;
  }

  return Status;
}
