/** @QspiDxeParseDt.c

  Functions for retrieving platform specific info from device tree and install protocols

  Copyright 2017-2020 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the bsd
  license which accompanies this distribution. the full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Library/IoAccessLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ItbParse.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "QspiDxe.h"

QSPI_MASTER  mQspiMasterTemplate = {
  .Handle = NULL, // Handle ... NEED TO BE FILLED

  .Regs = NULL, // QspiRegs
  .AmbaBase = 0, // AmbaBase ... NEED TO BE FILLED
  .CurAmbaBase = 0, // CurAmbaBase
  .NumChipselect = 0, // NumChipselect ... NEED TO BE FILLED

  .QspiHcProtocol = {
    .Attributes = SPI_SUPPORTS_4_BIT_DATA_BUS_WIDTH |
                  SPI_SUPPORTS_2_BIT_DATA_BUS_WIDTH |
                  SPI_HALF_DUPLEX |
                  SPI_TRANSFER_SIZE_INCLUDES_OPCODE |
                  SPI_TRANSFER_SIZE_INCLUDES_ADDRESS,
    .FrameSizeSupportMask = SPI_FRAME_MASK (8), // 8-bit frames only
    .MaximumTransferBytes = SIZE_64KB + sizeof (UINT32), // 0xFFFF - IPCR[IDATSZ] + 1 byte opcode + 4 bytes address
    .ChipSelect = QspiChipSelect,
    .Clock = QspiClock,
    .Transaction = QspiTransaction,
    .UpdateSpiPeripheral = QspiUpdateSpiPeripheral
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
  .Event = NULL
};

/**
 This function parses the device tree and for each QSPI controller node found,
 allocates runtime memory for the internal data structure containing a handle and installs
 the SpiMaster Protocol and Device Path protocol on to that handle.

 @param[in]  Fdt                 Platform's device tree blob
 @param[out] QSpiCount           Total number of QSPI controllers found.

 @retval EFI_UNSUPPORTED         The device tree node properties of QSPI controller are
                                 not supported by driver
 @retval EFI_INVALID_PARAMETER   No Parent node of QSPI node found in device tree.
 @retval EFI_OUT_OF_RESOURCES    No pool memory available
 @retval EFI_ALREADY_STARTED     The controller is busy with another transaction.
 @retval EFI_DEVICE_ERROR        There was an error during installing protocols.
 @retval EFI_SUCCESS             The protocols were installed successfully.
**/
EFI_STATUS
ParseDeviceTree (
  IN  VOID          *Fdt,
  OUT UINTN         *QSpiCount
  )
{
  INT32              NodeOffset;
  CONST fdt32_t      *Prop;
  INT32              PropLen;
  INT32              QspiIndex;
  INT32              QspiMemIndex;
  QSPI_MASTER        *QspiMasterPtr;
  EFI_STATUS         Status;
  UINT64             AmbaBase;
  UINT64             AmbaTotalSize;
  UINT32             QspiMasterIndex;
  UINT64             Regs;
  BOOLEAN            Runtime;
  INT32              FdtStatus;

  Status = EFI_SUCCESS;
  QspiMasterIndex = 0;
  *QSpiCount = 0;

  for  (NodeOffset = fdt_node_offset_by_compatible (Fdt, -1, (VOID *)(PcdGetPtr (PcdQspiFdtCompatible)));
        NodeOffset != -FDT_ERR_NOTFOUND;
        NodeOffset = fdt_node_offset_by_compatible (Fdt, NodeOffset, (VOID *)(PcdGetPtr (PcdQspiFdtCompatible))))
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

    QspiIndex = fdt_stringlist_search (Fdt, NodeOffset, "reg-names", "QuadSPI");
    if (QspiIndex < 0) {
      DEBUG ((DEBUG_ERROR, "Error: can't get regs base addresses(ret = %d)!\n", QspiIndex));
      continue;
    }

    Status = FdtGetAddressSize (Fdt, NodeOffset, "reg", QspiIndex, &Regs, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: can't get regs base addresses(ret = %r)!\n", Status));
      continue;
    }

    QspiMemIndex = fdt_stringlist_search (Fdt, NodeOffset, "reg-names", "QuadSPI-memory");
    if (QspiMemIndex < 0) {
      DEBUG ((DEBUG_ERROR, "Error: can't get AMBA base addresses(ret = %d)!\n", QspiMemIndex));
      continue;
    }

    Status = FdtGetAddressSize (Fdt, NodeOffset, "reg", QspiMemIndex, &AmbaBase, &AmbaTotalSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: can't get AMBA base addresses(ret = %r)!\n", Status));
      continue;
    }

    Prop = fdt_getprop (Fdt, NodeOffset, "uefi-runtime", &PropLen);
    if (Prop == NULL) {
      QspiMasterPtr = AllocateCopyPool (sizeof(QSPI_MASTER), &mQspiMasterTemplate);
      Runtime = FALSE;
    } else {
      QspiMasterPtr = AllocateRuntimeCopyPool (sizeof(QSPI_MASTER), &mQspiMasterTemplate);
      Runtime = TRUE;
    }
    if (!QspiMasterPtr) {
      DEBUG ((DEBUG_WARN, "No space left!!\n"));
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    QspiMasterPtr->Regs = (QSPI_REGISTERS *)Regs;
    QspiMasterPtr->AmbaBase = AmbaBase;

    // determine endianness automatically
    if (MmioRead32 ( (UINTN)&QspiMasterPtr->Regs->Lutkey) == LUT_KEY) {
      QspiMasterPtr->Read32 = MmioRead32;
      QspiMasterPtr->Write32 = MmioWrite32;
    } else {
      QspiMasterPtr->Read32 = SwapMmioRead32;
      QspiMasterPtr->Write32 = SwapMmioWrite32;
    }

    Prop = fdt_getprop(Fdt, NodeOffset, "num-cs", &PropLen);

    if (Prop == NULL) {
      QspiMasterPtr->NumChipselect = QSPI_CHIP_SELECT_MAX;
    } else {
      QspiMasterPtr->NumChipselect = fdt32_to_cpu(*Prop);
      if (QspiMasterPtr->NumChipselect > QSPI_CHIP_SELECT_MAX) {
        DEBUG ((DEBUG_WARN, "Warning: More than supported (%d) Chipselect Number %u!\n",
               QSPI_CHIP_SELECT_MAX, QspiMasterPtr->NumChipselect));
        QspiMasterPtr->NumChipselect = QSPI_CHIP_SELECT_MAX;
      }
    }

    Prop = fdt_getprop(Fdt, NodeOffset, "bus-num", &PropLen);

    if (Prop == NULL) {
      QspiMasterPtr->DevicePath.Controller.ControllerNumber = QspiMasterIndex;
    } else {
      QspiMasterPtr->DevicePath.Controller.ControllerNumber = fdt32_to_cpu(*Prop);
    }

    QspiMasterIndex++;

    // Setup FLEX_SPI Module
    Status = QspiSetup (QspiMasterPtr, AmbaTotalSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Qspi Setup failed!!\n"));
      FreePool (QspiMasterPtr);
      continue;
    }

    // Install SPI Host controller protocol and Device Path Protocol
    Status = QspiInstallProtocol (QspiMasterPtr, Runtime);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "QSpi Spi Host Controller Installation Failed!!\n"));
      FreePool (QspiMasterPtr);
      continue;
    }

    if (Runtime) {
      FdtStatus = fdt_setprop_string (Fdt, NodeOffset, "status", "disabled");
      if (FdtStatus) {
        DEBUG ((DEBUG_ERROR, "Error %a disabling Node %a\n",
          fdt_strerror (FdtStatus), fdt_get_name (Fdt, NodeOffset, NULL)));
      }
    }

    *QSpiCount += 1;
  }

  return Status;
}
