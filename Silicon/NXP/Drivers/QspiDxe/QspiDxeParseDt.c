/** @QspiDxeParseDt.c

  Functions for retrieving platform specific info from device tree and install protocols

  Copyright 2017 NXP

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
#include <Library/DxeServicesTableLib.h>
#include <Library/IoLib.h>
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
    .Clock = QspiSetupPeripheral,
    .Transaction = QspiTransaction
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
  .Link = {
    .ForwardLink = NULL,
    .BackLink = NULL
  }
};

/**
  Read a big number; size is in cells (not bytes)

  @param[in] Cell     Pointer to a cell in device tree in fdt32_t format.
  @param[in] Size     size in number of cells. each cell is 4 byte (32 bit) size.

  @retval             The number in CPU format.

**/
STATIC
UINT64
QspiReadNumber(
  IN CONST fdt32_t  *Cell,
  IN INT32          Size
  )
{
  UINT64 Number;

  Number = 0;
  while (Size--) {
    Number = (Number << 32) |
             fdt32_to_cpu (*(Cell++));
  }
  return Number;
}

/**
 This function parses the device tree and for each QSPI controller node found,
 allocates runtime memory for the internal data structure containing a handle and installs
 the SpiMaster Protocol and Device Path protocol on to that handle.

 @param[in]  Fdt                 Platform's device tree blob
 @param[out] QMasterList         Link List of QSPI_MASTER structures that are runtime.
                                 each member corresponds to one QSPI controller.
 @param[out] QspiMasterCount     Count of Qspi controllers in Platform

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
  OUT LIST_ENTRY    *QMasterList
  )
{
  UINT32             McrVal;
  INT32              NodeOffset;
  INT32              ParentOffset;
  INT32              AddressCells;
  INT32              SizeCells;
  CONST fdt32_t      *Prop;
  INT32              PropLen;
  INT32              QspiIndex;
  INT32              QspiMemIndex;
  INT32              RegCount;
  QSPI_MASTER        *QspiMasterPtr;
  EFI_STATUS         Status;
  UINT32             AmbaBase;
  UINT32             AmbaTotalSize;
  UINT32             AmbaSizePerChip;
  UINT32             QspiMasterIndex;
  BOOLEAN            ContainVariableStorage;

  Status = EFI_SUCCESS;
  QspiMasterIndex = 0;

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

    ParentOffset = fdt_parent_offset (Fdt, NodeOffset);
    if (ParentOffset < 0) {
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    AddressCells = fdt_address_cells (Fdt, ParentOffset);
    if ((AddressCells < 0) || (AddressCells > 2)) {
      DEBUG ((DEBUG_ERROR, "AddressCells = %d\n", AddressCells));
      Status = EFI_UNSUPPORTED;
      break;
    }

    SizeCells = fdt_size_cells (Fdt, ParentOffset);
    if ((SizeCells < 0) || (SizeCells > 2)) {
      DEBUG ((DEBUG_ERROR, "SizeCells = %d\n", SizeCells));
      Status = EFI_UNSUPPORTED;
      break;
    }

    Prop = fdt_getprop (Fdt, NodeOffset, "reg", &PropLen);
    if (Prop == NULL) {
      DEBUG ((DEBUG_ERROR, "reg property not found\n"));
      continue;
    }

    RegCount = fdt_stringlist_count (Fdt, NodeOffset, "reg-names");
    if (RegCount < 0) {
      DEBUG ((DEBUG_ERROR, "RegCount = %d\n", RegCount));
      continue;
    }

    QspiIndex = fdt_stringlist_search (Fdt, NodeOffset, "reg-names", "QuadSPI");
    if (QspiIndex < 0) {
      DEBUG ((DEBUG_ERROR, "Error: can't get regs base addresses(ret = %d)!\n", QspiIndex));
      continue;
    }

    QspiMemIndex = fdt_stringlist_search (Fdt, NodeOffset, "reg-names", "QuadSPI-memory");
    if (QspiMemIndex < 0) {
      DEBUG ((DEBUG_ERROR, "Error: can't get AMBA base addresses(ret = %d)!\n", QspiMemIndex));
      continue;
    }

    if (PropLen < RegCount * (AddressCells + SizeCells) * sizeof (INT32)) {
      continue;
    }

    if (sizeof (QSPI_REGISTERS) > QspiReadNumber (
                                    Prop + (QspiIndex * (AddressCells + SizeCells)) + AddressCells,
                                    SizeCells
                                    ))
    {
      DEBUG ((DEBUG_ERROR, "Qspi Registers span more space than allocated in device tree!!\n"));
      continue;
    }

    AmbaBase = QspiReadNumber (Prop + (QspiMemIndex * (AddressCells + SizeCells)), AddressCells);
    AmbaTotalSize = QspiReadNumber (
                      Prop + (QspiMemIndex * (AddressCells + SizeCells)) + AddressCells,
                      SizeCells
                      );

    // Check if this QSPI controller is connected to a device using runtime variables
    ContainVariableStorage =
        (AmbaBase <= PcdGet64 (PcdFlashNvStorageVariableBase64)) &&
        (PcdGet64 (PcdFlashNvStorageVariableBase64) + PcdGet32 (PcdFlashNvStorageVariableSize) <= AmbaTotalSize);

    if (ContainVariableStorage) {
      QspiMasterPtr = AllocateRuntimeCopyPool (sizeof(QSPI_MASTER), &mQspiMasterTemplate);
    } else {
      QspiMasterPtr = AllocateCopyPool (sizeof(QSPI_MASTER), &mQspiMasterTemplate);
    }
    if (!QspiMasterPtr) {
      DEBUG ((DEBUG_ERROR, "No space left!!\n"));
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    QspiMasterPtr->Regs = (VOID *)QspiReadNumber (Prop + (QspiIndex * (AddressCells + SizeCells)), AddressCells);
    QspiMasterPtr->AmbaBase = AmbaBase;

    if (fdt_getprop(Fdt, NodeOffset, "big-endian", NULL) != NULL) {
      QspiMasterPtr->Read32 = BeMmioRead32;
      QspiMasterPtr->Write32 = BeMmioWrite32;
    } else {
      QspiMasterPtr->Read32 = MmioRead32;
      QspiMasterPtr->Write32 = MmioWrite32;
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

    QspiMasterIndex++;
    Prop = fdt_getprop(Fdt, NodeOffset, "bus-num", &PropLen);

    if (Prop == NULL) {
      QspiMasterPtr->DevicePath.Controller.ControllerNumber = QspiMasterIndex;
    } else {
      QspiMasterPtr->DevicePath.Controller.ControllerNumber = fdt32_to_cpu(*Prop);
    }

    // Reset QSPI Module
    QspiSwReset (QspiMasterPtr, TRUE);

    /* Put the QSPI controller in Module Disable Mode */
    McrVal = QspiMasterPtr->Read32 ( (UINTN)&QspiMasterPtr->Regs->Mcr);
    McrVal |= MCR_IDLE_SIGNAL_DRIVE | MCR_MDIS_MASK | (McrVal & MCR_END_CFD_MASK);

    QspiMasterPtr->Write32 ( (UINTN)&QspiMasterPtr->Regs->Mcr, McrVal);

    QspiConfigureSampling (
      QspiMasterPtr,
      (SMPR_FSDLY_MASK | SMPR_DDRSMP_MASK | SMPR_FSPHS_MASK | SMPR_HSENA_MASK),
      0
      );

    if (QspiMasterPtr->NumChipselect) {
      //
      // Assign AMBA Memory Zone for Every CS
      // QSPI Has Two Channels And Every Channel Has Two CS.
      // if No Of CS Is 2, The AMBA Memory Will Be Divided Into Two Parts
      // And Assign To Every Channel. This Indicate That Every
      // Channel Only Has One Valid CS.
      // if No Of CS Is 4, The AMBA Memory Will Be Divided Into Four Parts
      // And Assign To Every Chipselect.Every Channel Will Has Two Valid CS.
      //
      AmbaSizePerChip = AmbaTotalSize >> ((QspiMasterPtr->NumChipselect + 1) >> 1);

      //
      // In case Of Single Die Flash Devices, TOP_ADDR_MEMA2 And
      // TOP_ADDR_MEMB2 Should Be Initialized To TOP_ADDR_MEMA1
      // And TOP_ADDR_MEMB1 Respectively. This Would Ensure
      // That Complete Memory Map Is Assigned To One Flash Device.
      //
      switch (QspiMasterPtr->NumChipselect) {
        case QSPI_CHIP_SELECT_MAX :
          QspiMasterPtr->Write32 (
                           (UINTN)&QspiMasterPtr->Regs->Sfb2ad,
                           QspiMasterPtr->AmbaBase + QSPI_CHIP_SELECT_MAX * AmbaSizePerChip
                           );
          // don't use break; use fall through mode

        case QSPI_CHIP_SELECT_3 :
          QspiMasterPtr->Write32 (
                           (UINTN)&QspiMasterPtr->Regs->Sfb1ad,
                           QspiMasterPtr->AmbaBase + QSPI_CHIP_SELECT_3 * AmbaSizePerChip
                           );
          // don't use break; use fall through mode

        case QSPI_CHIP_SELECT_2 :
          QspiMasterPtr->Write32 (
                           (UINTN)&QspiMasterPtr->Regs->Sfa2ad,
                            QspiMasterPtr->AmbaBase + QSPI_CHIP_SELECT_2 * AmbaSizePerChip
                            );
          // don't use break; use fall through mode

        default:
          QspiMasterPtr->Write32 (
                           (UINTN)&QspiMasterPtr->Regs->Sfa1ad,
                            QspiMasterPtr->AmbaBase + AmbaSizePerChip
                            );
          break;
      }
    }

    // Enable the QSPI Module
    EnableQspiModule (QspiMasterPtr, TRUE);
    // Clear SW Reset Bits
    QspiSwReset (QspiMasterPtr, FALSE);

    // Install SPI Host controller protocol and Device Path Protocol
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &QspiMasterPtr->Handle,
                    &gEfiSpiHcProtocolGuid, &QspiMasterPtr->QspiHcProtocol,
                    &gEfiDevicePathProtocolGuid, &QspiMasterPtr->DevicePath,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a:%d Error = %d\n", __FUNCTION__, __LINE__, Status));
      FreePool (QspiMasterPtr);
      continue;
    }

    if (ContainVariableStorage) {
      // Declare the controller registers as EFI_MEMORY_RUNTIME
      Status = gDS->AddMemorySpace (
                      EfiGcdMemoryTypeMemoryMappedIo,
                      (UINTN)QspiMasterPtr->Regs,
                      ALIGN_VALUE (sizeof (QSPI_REGISTERS), SIZE_64KB),
                      EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                      );
      ASSERT_EFI_ERROR (Status);

      Status = gDS->SetMemorySpaceAttributes (
                      (UINTN)QspiMasterPtr->Regs,
                      ALIGN_VALUE (sizeof (QSPI_REGISTERS), SIZE_64KB),
                      EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                      );
      ASSERT_EFI_ERROR (Status);

      InsertTailList (QMasterList, &QspiMasterPtr->Link);

      // Connect the controller Recursively to SPI bus
      Status = gBS->ConnectController (QspiMasterPtr->Handle, NULL, NULL, TRUE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a:%d Error = %d\n", __FUNCTION__, __LINE__, Status));
        FreePool (QspiMasterPtr);
        continue;
      }
    }
  }

  return Status;
}
