/** @file
  Sample ACPI Platform Driver

  Copyright (c) 2008 - 2011, Intel Corporation. All rights reserved.<BR>
  Copyright 2019-2020 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>

#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/AcpiPlatformLib.h>

#include <IndustryStandard/Acpi.h>

/**
  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param  Instance      Return pointer to the first instance of the protocol

  @return EFI_SUCCESS           The function completed successfully.
  @return EFI_NOT_FOUND         The protocol could not be located.
  @return EFI_OUT_OF_RESOURCES  There are not enough resources to find the protocol.

**/
EFI_STATUS
LocateFvInstanceWithTables (
  OUT EFI_FIRMWARE_VOLUME2_PROTOCOL **Instance
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  EFI_FV_FILETYPE               FileType;
  UINT32                        FvStatus;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINTN                         Size;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FvInstance;

  FvStatus = 0;

  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiFirmwareVolume2ProtocolGuid,
                   NULL,
                   &NumberOfHandles,
                   &HandleBuffer
                   );
  if (EFI_ERROR (Status)) {
    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }



  //
  // Looking for FV with ACPI storage file
  //

  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiFirmwareVolume2ProtocolGuid,
                     (VOID**) &FvInstance
                     );
    ASSERT_EFI_ERROR (Status);

    //
    // See if it has the ACPI storage file
    //
    Status = FvInstance->ReadFile (
                           FvInstance,
                           (EFI_GUID*)PcdGetPtr (PcdAcpiTableStorageFile),
                           NULL,
                           &Size,
                           &FileType,
                           &Attributes,
                           &FvStatus
                           );

    //
    // If we found it, then we are done
    //
    if (Status == EFI_SUCCESS) {
      *Instance = FvInstance;
      break;
    }
  }

  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //

  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

  return Status;
}


/**
  This function calculates and updates an UINT8 checksum.

  @param  Buffer          Pointer to buffer to checksum
  @param  Size            Number of bytes to checksum

**/
VOID
AcpiPlatformChecksum (
  IN UINT8      *Buffer,
  IN UINTN      Size
  )
{
  UINTN ChecksumOffset;

  ChecksumOffset = OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum);

  //
  // Set checksum to 0 first
  //
  Buffer[ChecksumOffset] = 0;

  //
  // Update checksum value
  //
  Buffer[ChecksumOffset] = CalculateCheckSum8(Buffer, Size);
}

STATIC
VOID
OnRootBridgesConnected (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                     Status;
  INT32                          TableCnt;
  INT32                          TableItr;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTable;
  EFI_ACPI_COMMON_HEADER         *CurrentTable[5];
  UINTN                          TableHandle[5];
  UINTN                          TableSize;

  //
  // Find the AcpiTable protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID**)&AcpiTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AcpiPlatformDxe: Failed to get ACPI Table Protocol\n"));
  }

  if (PcdGetBool (PcdDynamicIortTable)) {
    CurrentTable[0] = (EFI_ACPI_COMMON_HEADER *)PcdGet64 (PcdDynamicIortTablePtr);
    CurrentTable[1] = (EFI_ACPI_COMMON_HEADER *)PcdGet64 (PcdDynamicMcfgTablePtr);
    CurrentTable[2] = (EFI_ACPI_COMMON_HEADER *)PcdGet64 (PcdDynamicDsdtTablePtr);
    TableCnt = 3;
  } else {
    CurrentTable[0] = (EFI_ACPI_COMMON_HEADER *)PcdGet64 (PcdDynamicIortTablePtr);
    TableCnt = 1;
  }

  for (TableItr = 0; TableItr < TableCnt; TableItr++) {

    TableSize = ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable[TableItr])->Length;
    //
    // Checksum ACPI table
    //
    AcpiPlatformChecksum ((UINT8*)CurrentTable[TableItr], TableSize);

    //
    // Install ACPI table
    //
    Status = AcpiTable->InstallAcpiTable (
        AcpiTable,
        CurrentTable[TableItr],
        TableSize,
        &TableHandle[TableItr]
        );

    if (EFI_ERROR(Status) && (TableItr == 0)) {
      DEBUG ((DEBUG_ERROR, "AcpiPlatformDxe: Install IORT Table - Failed.\n"));
    } else if (EFI_ERROR(Status) && (TableItr == 1)) {
      DEBUG ((DEBUG_ERROR, "AcpiPlatformDxe: Install MCFG Table - Failed.\n"));
    } else if (EFI_ERROR(Status) && (TableItr == 2)) {
      DEBUG ((DEBUG_ERROR, "AcpiPlatformDxe: Install DSDT Table - Failed.\n"));
    }

    //
    // Free memory allocated by ReadSection
    //
    //gBS->FreePool ((UINT8*)CurrentTable[TableItr]);
  }
}

/**
  Entrypoint of Acpi Platform driver.

  @param  ImageHandle
  @param  SystemTable

  @return EFI_SUCCESS
  @return EFI_LOAD_ERROR
  @return EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
AcpiPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTable;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  INTN                           Instance;
  EFI_ACPI_COMMON_HEADER         *CurrentTable;
  UINTN                          TableHandle;
  UINT32                         FvStatus;
  UINTN                          TableSize;
  UINTN                          Size;
  EFI_ACPI_DESCRIPTION_HEADER    *TableHeader;
  EFI_EVENT                      RootBridgesConnected;

  Instance     = 0;
  CurrentTable = NULL;
  TableHandle  = 0;

  if (PcdGetBool (PcdDynamicIortTable)) {
    Status = AcpiPlatformFixup (NULL);
    Status |= gBS->CreateEventEx (
                   EVT_NOTIFY_SIGNAL,
                   TPL_CALLBACK,
                   OnRootBridgesConnected,
                   NULL,
                   &gRootBridgesConnectedEventGroupGuid,
                   &RootBridgesConnected
                   );

    if (EFI_ERROR(Status)) {
      return EFI_ABORTED;
    }

    return EFI_SUCCESS;
  }

  //
  // Find the AcpiTable protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID**)&AcpiTable);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  //
  // Locate the firmware volume protocol
  //
  Status = LocateFvInstanceWithTables (&FwVol);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }
  //
  // Read tables from the storage file.
  //
  while (Status == EFI_SUCCESS) {

    Status = FwVol->ReadSection (
                      FwVol,
                      (EFI_GUID*)PcdGetPtr (PcdAcpiTableStorageFile),
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID**) &CurrentTable,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR(Status)) {
      //
      // Add the table
      //
      TableHandle = 0;

      TableSize = ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->Length;
      ASSERT (Size >= TableSize);

      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER*) (CurrentTable);

      Status = AcpiPlatformFixup (TableHeader);
      if (EFI_ERROR(Status)) {
        gBS->FreePool (CurrentTable);
        return EFI_ABORTED;
      }

      if (TableHeader->Signature == EFI_ACPI_6_2_IO_REMAPPING_TABLE_SIGNATURE) {
        PcdSet64 (PcdIortTablePtr, (UINT64)CurrentTable);
        //
        // Delay installing the ACPI tables until root bridges are
        // connected. The entry point's return status will only reflect the callback
        // setup. (Note that we're a DXE_DRIVER; our entry point function is invoked
        // strictly before BDS is entered and can connect the root bridges.)
        //
        Status = gBS->CreateEventEx (
                       EVT_NOTIFY_SIGNAL,
                       TPL_CALLBACK,
                       OnRootBridgesConnected,
                       NULL,
                       &gRootBridgesConnectedEventGroupGuid,
                       &RootBridgesConnected
                       );
        //
        // Increment the instance
        //
        Instance++;
        CurrentTable = NULL;
        continue;
      }
      //
      // Checksum ACPI table
      //
      AcpiPlatformChecksum ((UINT8*)CurrentTable, TableSize);

      //
      // Install ACPI table
      //
      Status = AcpiTable->InstallAcpiTable (
                            AcpiTable,
                            CurrentTable,
                            TableSize,
                            &TableHandle
                            );

      //
      // Free memory allocated by ReadSection
      //
      gBS->FreePool (CurrentTable);

      if (EFI_ERROR(Status)) {
        return EFI_ABORTED;
      }

      //
      // Increment the instance
      //
      Instance++;
      CurrentTable = NULL;
    }
  }

  return EFI_SUCCESS;
}

