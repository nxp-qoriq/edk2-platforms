/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Chassis.h>
#include <Library/AcpiPlatformLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/NxpMcfgTable.h>

#define AML_BUFFER_OP                0x11
#define AML_RESOURCE_BUS             0x2

#define PHY_ADDR1                    0x4
#define PHY_ADDR2                    0x5
#define PHY_ADDR1_REVC               0x0
#define PHY_ADDR2_REVC               0x8

static
EFI_STATUS
_FixPhyAddress(
    IN EFI_ACPI_SDT_PROTOCOL  *AcpiTableProtocol,
    IN EFI_ACPI_HANDLE        CurrentHandle
    )
{
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST UINT8         *Data;
  CONST VOID          *Buffer;
  UINTN               DataSize;
  UINT8               PhyAddr[1];

  Status = AcpiTableProtocol->GetOption(CurrentHandle, 1, &DataType, &Buffer, &DataSize);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Data = Buffer;
  if (Data[0] != PHY_ADDR1 && Data[0] != PHY_ADDR2) {
    return EFI_SUCCESS;
  }

  PhyAddr[0] = (Data[0] == PHY_ADDR1) ? PHY_ADDR1_REVC : PHY_ADDR2_REVC;
  Status = AcpiTableProtocol->SetOption(CurrentHandle, 1, (VOID*)&PhyAddr[0], sizeof(UINT8));

  return Status;
}

static
EFI_STATUS
ProcessDSDTDevice (
    EFI_ACPI_SDT_PROTOCOL *AcpiTableProtocol,
    EFI_ACPI_HANDLE ChildHandle
    )
{
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST UINT8         *Data;
  CONST VOID          *Buffer;
  UINTN               DataSize;
  EFI_ACPI_HANDLE     DevHandle;

  DBG ("[%a:%d] - ChildHandle=%p\n", __FUNCTION__, __LINE__, ChildHandle);
  Status = AcpiTableProtocol->GetOption(ChildHandle, 0, &DataType, &Buffer, &DataSize);
  if (EFI_ERROR(Status)) {
    return EFI_SUCCESS;
  }

  Data = Buffer;
  //
  // Skip all non-device type
  //
  if (DataSize != 2 || Data[0] != AML_EXT_OP || Data[1] != AML_EXT_DEVICE_OP) {
    return EFI_SUCCESS;
  }

  Status = AcpiTableProtocol->GetOption(ChildHandle, 1, &DataType, &Buffer, &DataSize);
  if (EFI_ERROR(Status)) {
    return EFI_SUCCESS;
  }

  Data = Buffer;
  if (AsciiStrnCmp((CHAR8 *)Data, "PHY", 3) == 0) {
    //
    // Walk the PHY device type node
    //
    for (DevHandle = NULL; ; ) {
      Status = AcpiTableProtocol->GetChild(ChildHandle, &DevHandle);
      if (EFI_ERROR(Status) || DevHandle == NULL) {
        break;
      }

      //
      // Search for _ADR
      //
      Status = AcpiTableProtocol->GetOption(DevHandle, 0, &DataType, &Buffer, &DataSize);
      if (EFI_ERROR(Status)) {
        break;
      }

      Data = Buffer;      
      if (DataSize == 1 && Data[0] == AML_NAME_OP) {
        Status = AcpiTableProtocol->GetOption(DevHandle, 1, &DataType, &Buffer, &DataSize);
        if (EFI_ERROR(Status)) {
          break;
        }

        Data = Buffer;
        if (DataType == EFI_ACPI_DATA_TYPE_NAME_STRING) {
          if (AsciiStrnCmp((CHAR8 *) Data, "_ADR", 4) == 0) {
            EFI_ACPI_HANDLE ValueHandle;

            Status = AcpiTableProtocol->GetOption(DevHandle, 2, &DataType, &Buffer, &DataSize);
            if (EFI_ERROR(Status)) {
              break;
            }

            if (DataType != EFI_ACPI_DATA_TYPE_CHILD) {
              continue;
            }

            AcpiTableProtocol->Open((VOID *) Buffer, &ValueHandle);
            Status = _FixPhyAddress(AcpiTableProtocol, ValueHandle);
            if (EFI_ERROR(Status)) {
              AcpiTableProtocol->Close(ValueHandle);
              break;
            }
            AcpiTableProtocol->Close(ValueHandle);
          }
        }
      }       
    }
  }

  return EFI_SUCCESS;
}

static
BOOLEAN
IsSbScope (
    EFI_ACPI_SDT_PROTOCOL   *AcpiTableProtocol,
    EFI_ACPI_HANDLE         ChildHandle
    )
{
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST UINT8         *Data;
  CONST VOID          *Buffer;
  UINTN               DataSize;

  DBG ("[%a:%d] - ChildHandle=%p\n", __FUNCTION__, __LINE__, ChildHandle);
  Status = AcpiTableProtocol->GetOption (ChildHandle, 0, &DataType, &Buffer, &DataSize);
  if (EFI_ERROR(Status)) {
    return FALSE;
  }

  Data = Buffer;
  if (DataSize != 1 || Data[0] != AML_SCOPE_OP) {
    return FALSE;
  }

  return TRUE;
}

static
EFI_STATUS
ProcessDSDTChild (
    EFI_ACPI_SDT_PROTOCOL *AcpiTableProtocol,
    EFI_ACPI_HANDLE ChildHandle
    )
{
  EFI_STATUS          Status;
  EFI_ACPI_HANDLE     DevHandle;

  DBG ("[%a:%d] - ChildHandle=%p\n", __FUNCTION__, __LINE__, ChildHandle);
  // Check Scope(_SB) at first
  if (!(Status = IsSbScope (AcpiTableProtocol, ChildHandle))) {
    DBG ("[%a:%d] - IsSbScope=%r\n", __FUNCTION__, __LINE__, Status);
    return ProcessDSDTDevice (AcpiTableProtocol, ChildHandle);
  }

  for (DevHandle = NULL; ; ) {
    Status = AcpiTableProtocol->GetChild (ChildHandle, &DevHandle);
    if (EFI_ERROR(Status) || DevHandle == NULL) {
      break;
    }

    ProcessDSDTDevice (AcpiTableProtocol, DevHandle);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
PlatformProcessDSDT (
    EFI_ACPI_SDT_PROTOCOL *AcpiTableProtocol,
    EFI_ACPI_HANDLE TableHandle
    ) 
{
  
  EFI_STATUS              Status;
  EFI_ACPI_HANDLE         ChildHandle;
  //
  // Parse table for device type
  for (ChildHandle = NULL; ; ) {
    Status = AcpiTableProtocol->GetChild(TableHandle, &ChildHandle);
    if (EFI_ERROR(Status)) {
      break;
    }
    if (ChildHandle == NULL) {
      break;
    }

    ProcessDSDTChild(AcpiTableProtocol, ChildHandle);
  }
 
  return EFI_SUCCESS; 
}

void
PlatformAcpiCheckSum (
    IN OUT  EFI_ACPI_SDT_HEADER *Table
    )
{
  UINTN ChecksumOffset;
  UINT8 *Buffer;

  ChecksumOffset = OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum);
  Buffer = (UINT8 *)Table;

  //
  // set checksum to 0 first
  //
  Buffer[ChecksumOffset] = 0;

  //
  // Update checksum value
  //
  Buffer[ChecksumOffset] = CalculateCheckSum8 (Buffer, Table->Length);
}

/**
  Fixup the DSDT Table based on running SOC's properties.

  @param[in]  Table   The DSDR Table to fixup.

  @return EFI_SUCCESS       Successfully fixed up the ACPI Table
**/
STATIC
EFI_STATUS
UpdateDsdtPcie (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  EFI_ACPI_COMMON_HEADER                 *TableHeader;
  EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR *WordBusRsc;
  UINT8                                  *DataPtr;

  TableHeader = (EFI_ACPI_COMMON_HEADER *)Table;

  if (PcdGetBool (PcdPciHideRootPort)) {
    for (DataPtr = (UINT8 *)(TableHeader + 1);
         DataPtr < (UINT8 *) ((UINT8 *) TableHeader + TableHeader->Length - 4);
         DataPtr++) {
      if (CompareMem(DataPtr, "RBUF", 4) == 0) {
        DataPtr += 4; // Skip RBUF
        if (*DataPtr == AML_BUFFER_OP) {
          DataPtr += 4; // Skip Resource Type, Length etc.
          WordBusRsc = (EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR *)DataPtr;
          if (WordBusRsc->GenFlag == AML_RESOURCE_BUS) {
            WordBusRsc->AddrRangeMin = SwapBytes16 (0x1);
            WordBusRsc->AddrLen = SwapBytes16 (0xff);
            DataPtr = DataPtr + sizeof (EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR);
            *(DataPtr) = 0x0;
          }
        }
      }
    }
  }

  return  EFI_SUCCESS;
}

/**
  Fixup the Mcfg Table based on running SOC's properties.

  @param[in]  Table   The Mcfg Table to fixup.

  @return EFI_SUCCESS       Successfully fixed up the ACPI Table
  @return EFI_DEVICE_ERROR  Could not fix the ACPI Table
**/
STATIC
EFI_STATUS
UpdateMcfg (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  UINT32 Svr;
  NXP_EFI_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_TABLE *Mcfg;
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE *McfgNode;
  UINTN Index;

  if (PcdGetBool (PcdPciHideRootPort)) {

    if (PcdGetBool (PcdDynamicIortTable)) {
      EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER  *McfgTblPtr;
      UINT32  McfgNodeSize;

      McfgTblPtr = (EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER*)Table;
      if (McfgTblPtr->Header.Signature != EFI_ACPI_6_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE) {
        DEBUG ((DEBUG_ERROR, "UpdateMcfg : MCFG table not found\n"));
        return EFI_NOT_FOUND;
      }
      McfgNode = (EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE*)((UINT8*)McfgTblPtr +
                    sizeof(EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER));
      McfgNodeSize = (McfgTblPtr->Header.Length - sizeof(EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER));
      while (McfgNodeSize) {
      McfgNode->StartBusNumber = 0x1;
      McfgNodeSize = (McfgNodeSize - sizeof(EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE));
      McfgNode = (EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE*)((UINT8*)McfgNode + McfgNodeSize);
      }
    } else {
      Mcfg = (NXP_EFI_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_TABLE *)Table;
      Index = 0;
      while (Index < ARRAY_SIZE(Mcfg->Config_Structure)) {
        McfgNode = &(Mcfg->Config_Structure[Index++]);
        McfgNode->StartBusNumber = 0x1;
      }
    }
    Table->OemRevision = 0xff;
  } else {
    Svr = PcdGet32 (PcdSocSvr);
    if (SVR_MAJOR (Svr) == 2) {
      Table->OemRevision = 0x01;
    }
  }

  return  EFI_SUCCESS;
}

/**
  Fixup the Acpi Table based on running SOC's properties.

  @param[in]  Table   The Acpi Table to fixup.

  @return EFI_SUCCESS       Successfully fixed up the ACPI Table
  @return EFI_DEVICE_ERROR  Could not fix the ACPI Table
**/
EFI_STATUS
AcpiPlatformFixup (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER      *TableHeader
  )
{
  EFI_STATUS   Status;

  if (PcdGetBool (PcdDynamicIortTable))
  {
      Status = UpdateMcfg ((EFI_ACPI_DESCRIPTION_HEADER*)PcdGet64 (PcdDynamicMcfgTablePtr));
      Status |= UpdateDsdtPcie ((EFI_ACPI_DESCRIPTION_HEADER*)PcdGet64 (PcdDynamicDsdtTablePtr));
      return Status;
  }

  switch (TableHeader->Signature) {
    case EFI_ACPI_6_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE:
      Status = UpdateMcfg (TableHeader);
      break;

    case EFI_ACPI_6_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
      Status = UpdateDsdtPcie (TableHeader);
      break;

    default:
      Status = EFI_SUCCESS;
  }

  return Status;
}
