/** @file
*
*  Platform specific Acpi Fixup
*
*  Copyright 2019-2020 NXP
*
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*
**/

#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Chassis.h>
#include <Library/AcpiPlatformLib.h>
#include <Library/DebugLib.h>

#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/SysEepromLib.h>

#define AML_BUFFER_OP                0x11
#define AML_RESOURCE_BUS             0x2

/**
  Fetch the mac address from eeprom, if fails to read then generate random
  address and fix the address dsdt propery in loop.

  @param[in]      AcpiTableProtocol   Protocol handle.
  @param[in]      ChildHandle         Child acpi object handle.
  @param[in]      DevType             Type of dev node - Qman(0),Bman(1) etc.
  @param[in]      DevID               ID of dev node - QMN0,QMN1,QMN2...etc.

  @retval         EFI_SUCCESS         DSDT found and processed successfully.
**/
static
EFI_STATUS
_FixMacAddress(
    IN EFI_ACPI_SDT_PROTOCOL  *AcpiTableProtocol,
    IN EFI_ACPI_HANDLE        ChildHandle,
    IN EFI_ACPI_HANDLE        CurrentHandle,
    IN UINTN                  DevType,
    IN UINTN                  DevID
    )
{
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST UINT8         *Data;
  CONST VOID          *Buffer;
  UINTN               DataSize;
  UINTN               Counter;
  UINT8               MacAddr[30];

  DBG("Fetch MAC address for ethernet port [%d]\n", DevID);

  //Update the MAC
  Status = MacReadFromEeprom (DevID, MacAddr);
  if (EFI_ERROR (Status)) {
    Status = GenerateMacAddress (DevID, MacAddr);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error getting mac address status %r\n",
           Status));
    return Status;
  }

  for (Counter = 0; Counter < 6; Counter++) {
    Status = AcpiTableProtocol->GetOption(CurrentHandle, 1, &DataType, &Buffer, &DataSize);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Data = Buffer;
    DBG("_DSD Child Subnode Store OpCode 0x%02X 0x%02X %02X DataType 0x%X\n",
        DataSize, Data[0], DataSize > 1 ? Data[1] : 0, DataType);

    if (DataType != EFI_ACPI_DATA_TYPE_UINT) {
      return Status;
    }

    // only need one byte.
    // FIXME: Assume the  CPU is little endian
    Status = AcpiTableProtocol->SetOption(CurrentHandle, 1, (VOID*)&MacAddr[Counter], sizeof(UINT8));
    DBG("Fixing MAC for DevID [%d] at Offset [%d] Address [%02x] Status %r\n",
        DevID, Counter, MacAddr[Counter],Status);
    if (EFI_ERROR(Status)) {
      break;
    }
    Status = AcpiTableProtocol->GetChild(ChildHandle, &CurrentHandle);
    if (EFI_ERROR(Status) || CurrentHandle == NULL) {
      break;
    }
  }
  DEBUG((DEBUG_ERROR, "Fixing Port %d MAC %02x:%02x:%02x:%02x:%02x:%02x %r\n",
        DevID,
        MacAddr[0],
        MacAddr[1],
        MacAddr[2],
        MacAddr[3],
        MacAddr[4],
        MacAddr[5],
        Status
        ));

  return Status;
}

/**
  Get hold of Dev handle of each child ACPI objects of the specified table pointed by table handle.
  Start processing the Dev node of child object in loop until table reaches end.
  Skip all non-dev nodes types and just walk down with in device node only.
  Try to match the "_HID" of each dev node with QBMAN node HIDs.
  Once the dev node of interest is found then again walk down the _DSD node of the dev node,
  Open package handle to look for property of interest.
  One the string property match and found just replace/oveerwrite the UINT32 value.

  @param[in]      AcpiTableProtocol   Protocol handle.
  @param[in]      ChildHandle         Child acpi object handle.
  @param[in,out]  Found               True if node found, else false.
  @param[in]      DevType             Type of dev node - Qman(0),Bman(1) etc.
  @param[in]      DevID               ID of dev node - QMN0,QMN1,QMN2...etc.

  @retval         EFI_SUCCESS         DSDT found and processed successfully.
**/
static
EFI_STATUS
_SearchReplacePackageAddress (
    IN EFI_ACPI_SDT_PROTOCOL  *AcpiTableProtocol,
    IN EFI_ACPI_HANDLE        ChildHandle,
    IN UINTN                  Level,
    IN OUT BOOLEAN            *Found,
    IN UINTN                  DevType,
    IN UINTN                  DevID
    )
{
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST UINT8         *Data;
  CONST VOID          *Buffer;
  UINTN               DataSize;
  EFI_ACPI_HANDLE     CurrentHandle;
  EFI_ACPI_HANDLE     NextHandle;
  CHAR8               *AcpiKey;

  DBG("In Level:%d\n", Level);
  Status = EFI_SUCCESS;
  for (CurrentHandle = NULL; ;) {
    Status = AcpiTableProtocol->GetChild(ChildHandle, &CurrentHandle);
    if (Level != 3 && (EFI_ERROR(Status) || CurrentHandle == NULL)) {
      break;
    }

    Status = AcpiTableProtocol->GetOption(CurrentHandle, 0, &DataType, &Buffer, &DataSize);
    Data = Buffer;
    DBG("_DSD Child Subnode Store Op Code 0x%02X 0x%02X %02X\n",
        DataSize, Data[0], DataSize > 1 ? Data[1] : 0);

    if (Level < 2 && Data[0] != AML_PACKAGE_OP) {
      continue;
    }

    if (Level == 2 && Data[0] == AML_STRING_PREFIX) {
      Status = AcpiTableProtocol->GetOption(CurrentHandle, 1, &DataType, &Buffer, &DataSize);
      if (EFI_ERROR(Status)) {
        break;
      }

      DBG("_DSD Child Subnode Store Op Code 0x%02X 0x%02X %02X\n",
          DataSize, Data[0], DataSize > 1 ? Data[1] : 0);

      Data = Buffer;
      if (DevType == MAC_DEV) {
        AcpiKey = ACPI_MAC_ADD_KEY;
      } else {
        AcpiKey = ACPI_RESERVED_MEM_KEY;
      }
      if ((DataType != EFI_ACPI_DATA_TYPE_STRING) || (AsciiStrCmp((CHAR8 *) Data, AcpiKey) != 0)) {
        continue;
      }
      DBG("_DSD Key Type %d. Found Matching key for DevType %d DevID %d\n", DataType, DevType, DevID);

      //
      // We found the node.
      //
      *Found = TRUE;
      continue;
    }

    if (Level == 3 && *Found) {
      if (DevType == MAC_DEV) {
        Status = _FixMacAddress(AcpiTableProtocol, ChildHandle, CurrentHandle, DevType, DevID);
      }
      // We are done updating the package
      break;
    }

    if (Level > 3) {
      break;
    }

    // Search next package if required
    AcpiTableProtocol->Open((VOID *) Buffer, &NextHandle);
    Status = _SearchReplacePackageAddress(AcpiTableProtocol, NextHandle, Level + 1, Found, DevType, DevID);
    AcpiTableProtocol->Close(NextHandle);
    if (!EFI_ERROR(Status)) {
      break;
    }
  }

  return Status;
}

/**
  Helper function that will invoke the actual search and replace logic for
  the dsdt properties.

  @param[in]      AcpiTableProtocol Protocol handle.
  @param[in]      ChildHandle       Child acpi object handle.
  @param[in]      DevType           Type of dev node - Qman(0),Bman(1) etc.
  @param[in,out]  DevID             ID of dev node - QMN0,QMN1,QMN2...etc.

  @retval         EFI_SUCCESS       DSDT found and processed successfully.
 **/
static
EFI_STATUS
SearchReplacePackageAddress (
    IN EFI_ACPI_SDT_PROTOCOL  *AcpiTableProtocol,
    IN EFI_ACPI_HANDLE        ChildHandle,
    IN UINTN                  DevType,
    IN UINTN                  DevID
    )
{
  BOOLEAN Found = FALSE;
  UINTN Level = 0;

  return _SearchReplacePackageAddress(AcpiTableProtocol, ChildHandle, Level, &Found, DevType, DevID);
}

/**
  Get hold of Dev handle of each child ACPI objects of the specified table pointed by table handle.
  Start processing the Dev node of child object in loop until table reaches end.
  Skip all non-dev nodes types and just walk down with in device node only.
  Try to match the "_HID" of each dev node with QBMAN node HIDs.
  Once the dev node of interest is found then fetch the dev node id to which update is applicable.

  @param[in]      AcpiTableProtocol Protocol handle.
  @param[in]      ChildHandle       Child acpi object handle.
  @param[in]      DevType           Type of dev node - Qman(0),Bman(1) etc.
  @param[in,out]  DevID             ID of dev node - QMN0,QMN1,QMN2...etc.

  @retval         EFI_SUCCESS       DSDT found and processed successfully.
 **/
static
EFI_STATUS
GetMemoryRegion (
    EFI_ACPI_SDT_PROTOCOL   *AcpiTableProtocol,
    EFI_ACPI_HANDLE         ChildHandle,
    UINTN                   DevType,
    UINTN                   *DevID
    )
{
  EFI_STATUS Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CHAR8               Data[5];
  CHAR8               *Node;
  CONST VOID          *Buffer;
  UINTN               DataSize;

  // Get NameString QMNx
  Status = AcpiTableProtocol->GetOption (ChildHandle, 1, &DataType, &Buffer, &DataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a:%d] Get NameString failed: %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  CopyMem (Data, Buffer, 4);
  DBG("Size %p Data [%02x] [%02x] [%02x] [%02x]\n", DataSize, Data[0], Data[1], Data[2], Data[3]);
  DBG("Size %p Data [%c] [%c] [%c] [%c]\n", DataSize, Data[0], Data[1], Data[2], Data[3]);

  if (DevType == BMAN_DEV) {
    Node = "BMN0";
  } else if (DevType == QMAN_DEV) {
    Node = "QMN0";
  } else if (DevType == MAC_DEV) {
    Node = "MAC0";
  } else {
    Node = "\0";
  }

  Data[4] = '\0';
  if (DataSize != 4 ||
      AsciiStrnCmp (Node, Data, 3) != 0 ||
      Data[3] > '9' || Data[3] < '0') {
    DEBUG ((DEBUG_ERROR, "[%a:%d] The NameString %a is not QBman dev node\n", __FUNCTION__, __LINE__, Data));
    return EFI_INVALID_PARAMETER;
  }

  *DevID = Data[3] - '0';
  DBG("Size %p Data [%c %c %c %c] DEV_TYPE [%d] DEV_ID [%d]\n", DataSize, Data[0], Data[1], Data[2], Data[3], DevType, *DevID);
  return EFI_SUCCESS;
}

/**
  Get hold of Dev handle of each child ACPI objects of the specified table pointed by table handle.
  Start processing the Dev node of child object in loop until table reaches end.
  Skip all non-dev nodes types and just walk down with in device node only.
  Try to match the "_HID" of each dev node with QBMAN node HIDs.
  Once the dev node of interest is found then again walk down the _DSD node of the dev node,
  Open package handle to look for property of interest.

  @param[in]  AcpiTableProtocol Protocol handle.
  @param[in]  ChildHandle       Child acpi object handle.

  @retval     EFI_SUCCESS       DSDT found and processed successfully.
 **/
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
  BOOLEAN             Found = FALSE;
  UINTN               DevID = 0;
  UINTN               DevType = MAX_DEV;
  EFI_ACPI_HANDLE     PkgHandle;

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

  //
  // Walk the device type node
  //
  for (DevHandle = NULL; ; ) {
    Status = AcpiTableProtocol->GetChild(ChildHandle, &DevHandle);
    if (EFI_ERROR(Status) || DevHandle == NULL) {
      break;
    }

    //
    // Search for _HID with Qman/Bman ID
    //
    Status = AcpiTableProtocol->GetOption(DevHandle, 0, &DataType, &Buffer, &DataSize);
    if (EFI_ERROR(Status)) {
      break;
    }

    Data = Buffer;
    DBG("Data Type 0x%02X %02X\n", Data[0], DataSize > 1 ? Data[1] : 0);
    if (DataSize == 1 && Data[0] == AML_NAME_OP) {
      Status = AcpiTableProtocol->GetOption(DevHandle, 1, &DataType, &Buffer, &DataSize);
      if (EFI_ERROR(Status)) {
        break;
      }

      Data = Buffer;
      if (DataType == EFI_ACPI_DATA_TYPE_NAME_STRING) {
        if (AsciiStrnCmp((CHAR8 *) Data, "_HID", 4) == 0) {
          EFI_ACPI_HANDLE ValueHandle;

          Status = AcpiTableProtocol->GetOption(DevHandle, 2, &DataType, &Buffer, &DataSize);
          if (EFI_ERROR(Status)) {
            break;
          }

          if (DataType != EFI_ACPI_DATA_TYPE_CHILD) {
            continue;
          }

          AcpiTableProtocol->Open((VOID *) Buffer, &ValueHandle);
          Status = AcpiTableProtocol->GetOption(ValueHandle, 1, &DataType, &Buffer, &DataSize);

          Data = Buffer;

          if (EFI_ERROR(Status) ||
              DataType != EFI_ACPI_DATA_TYPE_STRING ) {
            AcpiTableProtocol->Close(ValueHandle);
            Found = FALSE;
            continue;
          }
          DBG("[%a:%d] - _HID = %a\n", __FUNCTION__, __LINE__, Data);
          if (AsciiStrCmp((CHAR8 *) Data, ACPI_QMAN_HID) == 0) {
            DBG("[%a:%d] - Found Qman Dev Node  _HID = %a\n", __FUNCTION__, __LINE__, Data);
            DevType = QMAN_DEV;
          } else if (AsciiStrCmp((CHAR8 *) Data, ACPI_BMAN_HID) == 0) {
            DBG("[%a:%d] - Found Bman Dev Node  _HID = %a\n", __FUNCTION__, __LINE__, Data);
            DevType = BMAN_DEV;
          } else if (AsciiStrCmp((CHAR8 *) Data, ACPI_MAC_HID) == 0) {
            DBG("[%a:%d] - Found Mac Dev Node  _HID = %a\n", __FUNCTION__, __LINE__, Data);
            DevType = MAC_DEV;
          } else {
            Found = FALSE;
            continue;
          }
          AcpiTableProtocol->Close(ValueHandle);
          Status = GetMemoryRegion (AcpiTableProtocol, ChildHandle, DevType, &DevID);
          if (EFI_ERROR (Status)) {
            Found = FALSE;
            continue;
          }
          Found = TRUE;

        } else if (Found == TRUE && AsciiStrnCmp((CHAR8 *) Data, "_DSD", 4) == 0) {
          //
          // Patch address
          //
          Status = AcpiTableProtocol->GetOption(DevHandle, 2, &DataType, &Buffer, &DataSize);
          if (EFI_ERROR(Status)) {
            break;
          }

          if (DataType != EFI_ACPI_DATA_TYPE_CHILD) {
            continue;
          }

          //
          // Open package data
          //
          AcpiTableProtocol->Open((VOID *) Buffer, &PkgHandle);
          Status = AcpiTableProtocol->GetOption(PkgHandle, 0, &DataType, &Buffer, &DataSize);

          Data = Buffer;
          DBG("_DSD Subnode Store Op Code 0x%02X %02X\n",
              Data[0], DataSize > 1 ? Data[1] : 0);

          //
          // Walk the _DSD node
          //
          if (DataSize == 1 && Data[0] == AML_PACKAGE_OP) {
            Status = SearchReplacePackageAddress(AcpiTableProtocol, PkgHandle, DevType, DevID);
          }

          AcpiTableProtocol->Close(PkgHandle);
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Checks Scope(_SB) at first. If found true then start processing child
  handle other wise go ahead with device handle.

  @param[in]  AcpiTableProtocol Protocol handle.
  @param[in]  ChildHandle       Child acpi object handle.

  @retval     TRUE/FALSE        TRUE if _SB scope else FALSE.
 **/
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

/**
  Get hold of Dev handle of each child ACPI objects of the specified table pointed by table handle.
  Start processing the Dev node of child object in loop until table reaches end.

  @param[in]  AcpiTableProtocol Protocol handle.
  @param[in]  ChildHandle       Child acpi object handle.

  @retval     EFI_SUCCESS       DSDT found and processed successfully.
 **/
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

/**
  Calculate the checksum and update the offset for table handle.

  @param[in,out]  TableHandle   Table handle.

  @retval         void          Do not return anything.
 **/
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
  Get hold of child ACPI objects of the specified table pointed by table handle.
  Start processing the child object in loop until table reaches end.

  @param[in]  AcpiTableProtocol Protocol handle.
  @param[in]  TableHandle       Table handle.

  @retval     EFI_SUCCESS       DSDT found and processed successfully.
 **/
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
  DBG ("[%a:%d] - TableHandle=%p\n", __FUNCTION__, __LINE__, TableHandle);
  for (ChildHandle = NULL; ; ) {
    Status = AcpiTableProtocol->GetChild(TableHandle, &ChildHandle);
    DBG ("[%a:%d] - Child=%p, %r\n", __FUNCTION__, __LINE__, ChildHandle, Status);
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
      Status = UpdateDsdtPcie ((EFI_ACPI_DESCRIPTION_HEADER*)PcdGet64 (PcdDynamicDsdtTablePtr));
      return Status;
  }

  switch (TableHeader->Signature) {
    case EFI_ACPI_6_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
      Status = UpdateDsdtPcie (TableHeader);
      break;

    default:
      Status = EFI_SUCCESS;
  }

  return Status;
}
