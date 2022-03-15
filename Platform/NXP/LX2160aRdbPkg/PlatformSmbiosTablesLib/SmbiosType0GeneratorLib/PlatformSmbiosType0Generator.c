/** @file
  Platform Smbios Type0 Table Generator

  Copyright (c) 2020, Puresoftware Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <IndustryStandard/SmBios.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <PiDxe.h>
#include <Protocol/Smbios.h>

// Module specific include files.
#include <ConfigurationManagerObject.h>
#include <ConfigurationManagerHelper.h>
#include <Library/TableHelperLib.h>
#include <Protocol/ConfigurationManagerProtocol.h>

#include <Protocol/DynamicTableFactoryProtocol.h>
#include <SmbiosTableGenerator.h>
#include <SmbiosTableHelper.h>

#define DEFAULT_TYPE0_STRINGS                \
  "EFI Development Kit II / NXP\0"   /* Vendor */      \
  "EDK II\0"                         /* BiosVersion */ \
  __DATE__"  "__TIME__"\0"           /* BiosReleaseDate */

#pragma pack()

STATIC CHAR8 *mArmType0;

STATIC ARM_TYPE0 mArmDefaultType0 = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_BIOS_INFORMATION,   /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE0),        /* UINT8 Length */
      SMBIOS_HANDLE_PI_RESERVED
    },
  },
  /* Text strings (unformatted area) */
  DEFAULT_TYPE0_STRINGS
};

/** This macro expands to a function that retrieves System bios
    (Type 0) Information from the Configuration Manager.
*/
GET_OBJECT_LIST (
  EObjNameSpaceArm,
  EArmObjSystemBiosInfoType0,
  CM_ARM_SYSTEM_BIOS_TYPE0_INFO
  );

/** Updates the information in the Type 0 Table.

  @param [in]  CfgMgrProtocol Pointer to the Configuration Manager
                              Protocol Interface.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object was not found.
  @retval EFI_BAD_BUFFER_SIZE   The size returned by the Configuration
                                Manager is less than the Object size for the
                                requested object.
**/
STATIC
EFI_STATUS
EFIAPI
AddSystemBiosType0Info (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST CfgMgrProtocol
)
{
  EFI_STATUS                          Status = EFI_SUCCESS;
  CM_ARM_SYSTEM_BIOS_TYPE0_INFO     * Type0Info;
  CHAR8                             * Str;
  UINT8                               TotalSize;

  ASSERT (CfgMgrProtocol != NULL);

  Status = GetEArmObjSystemBiosInfoType0 (
      CfgMgrProtocol,
      CM_NULL_TOKEN,
      &Type0Info,
      NULL
      );
  if (EFI_ERROR (Status)) {
    DEBUG ((
          DEBUG_ERROR,
          "ERROR: SMBIOS: Failed to get Type0 table. Status = %r\n",
          Status
          ));
  }

  mArmDefaultType0.Base.Vendor                                  = Type0Info->Vendor;
  mArmDefaultType0.Base.BiosVersion                             = Type0Info->BiosVersion;
  mArmDefaultType0.Base.BiosSegment                             = Type0Info->BiosSegment;
  mArmDefaultType0.Base.BiosReleaseDate                         = Type0Info->BiosReleaseDate;
  mArmDefaultType0.Base.BiosSize                                = Type0Info->BiosSize;
  mArmDefaultType0.Base.BiosCharacteristics                     = Type0Info->BiosCharacteristics;
  mArmDefaultType0.Base.BIOSCharacteristicsExtensionBytes[0]    = Type0Info->BIOSCharacteristicsExtensionBytes[0];
  mArmDefaultType0.Base.BIOSCharacteristicsExtensionBytes[1]    = Type0Info->BIOSCharacteristicsExtensionBytes[1];
  mArmDefaultType0.Base.SystemBiosMajorRelease                  = Type0Info->SystemBiosMajorRelease;
  mArmDefaultType0.Base.SystemBiosMinorRelease                  = Type0Info->SystemBiosMinorRelease;
  mArmDefaultType0.Base.EmbeddedControllerFirmwareMajorRelease  = Type0Info->EmbeddedControllerFirmwareMajorRelease;
  mArmDefaultType0.Base.EmbeddedControllerFirmwareMinorRelease  = Type0Info->EmbeddedControllerFirmwareMinorRelease;

  //
  // Allocate a new pool and popluate table data and its strings
  //
  TotalSize = mArmDefaultType0.Base.Hdr.Length + (3*SMBIOS_STRING_MAX_LENGTH);
  mArmType0 = AllocateZeroPool (TotalSize);
  if (mArmType0 == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
  }
  if (Status == EFI_SUCCESS) {
    CopyMem (mArmType0, &mArmDefaultType0, mArmDefaultType0.Base.Hdr.Length);

    //
    // Update BIOS Major and Minor Version
    //
    ((ARM_TYPE0 *)mArmType0)->Base.SystemBiosMajorRelease  = (UINT8) ((FixedPcdGet32 ( PcdFirmwareRevision ) >> 16) & 0xFF);
    ((ARM_TYPE0 *)mArmType0)->Base.SystemBiosMinorRelease  = (UINT8) (FixedPcdGet32 ( PcdFirmwareRevision ) & 0xFF);

    //
    // Append string pack
    //
    Str = mArmType0 + mArmDefaultType0.Base.Hdr.Length;

    //
    // Bios Vendor
    //
    AsciiStrCpyS (Str, SMBIOS_STRING_MAX_LENGTH, DEFAULT_TYPE0_STRINGS);
    Str += AsciiStrSize(Str);

    //
    // Bios Version from PcdFirmwareVersionString
    //
    UnicodeStrToAsciiStrS((CHAR16*)FixedPcdGetPtr (PcdFirmwareVersionString), Str, SMBIOS_STRING_MAX_LENGTH);
    Str += AsciiStrSize(Str);

    //
    // Bios Release Date
    //
    AsciiStrCpyS ((CHAR8 *)Str, SMBIOS_STRING_MAX_LENGTH, __DATE__" "__TIME__"\0");
    Str += AsciiStrSize(Str);

    //
    // Double NULL at the end of unformatted string
    //
    *Str = 0;
  }

  return Status;
}

/** Construct the Default Type0 table.

  This function invokes the Configuration Manager protocol interface
  to get the required hardware information for generating the SMBIOS
  table.

  If this function allocates any resources then they must be freed
  in the FreeXXXXTableResources function.

  @param [in]  This             Pointer to the table generator.
  @param [in]  SmbiosTableInfo  Pointer to the ACPI Table Info.
  @param [in]  CfgMgrProtocol   Pointer to the Configuration Manager
                                Protocol Interface.
  @param [out] Table            Pointer to the constructed SMBIOS Table.

  @retval EFI_SUCCESS           Table generated successfully.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object was not found.
  @retval EFI_BAD_BUFFER_SIZE   The size returned by the Configuration
                                Manager is less than the Object size for the
                                requested object.
**/
STATIC
EFI_STATUS
EFIAPI
BuildType0Table (
  IN  CONST SMBIOS_TABLE_GENERATOR                  *       This,
  IN        CM_STD_OBJ_SMBIOS_TABLE_INFO            * CONST SmbiosTableInfo,
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL    * CONST CfgMgrProtocol,
  OUT       SMBIOS_STRUCTURE                       **       Table
  )
{
  EFI_STATUS  Status;

  ASSERT (This != NULL);
  ASSERT (SmbiosTableInfo != NULL);
  ASSERT (CfgMgrProtocol != NULL);
  ASSERT (Table != NULL);
  ASSERT (SmbiosTableInfo->TableGeneratorId == This->GeneratorID);

  *Table = NULL;

  // Update BootArch Info
  Status = AddSystemBiosType0Info (CfgMgrProtocol);
  if (EFI_ERROR (Status)) {
    goto error_handler;
  }

  *Table = (SMBIOS_STRUCTURE*)mArmType0;

error_handler:
  return Status;
}

/** Free resources used by the SMBIOS Type0 table.

  This function frees the memory allocated for the table
  if PcdFirmwareVersionString is valid.

  @param [in]  This             Pointer to the table generator.
  @param [in]  SmbiosTableInfo  Pointer to the ACPI Table Info.
  @param [in]  CfgMgrProtocol   Pointer to the Configuration Manager
                                Protocol Interface.
  @param [out] Table            Pointer to the constructed SMBIOS Table.

  @retval EFI_SUCCESS           Table resources freed successfully.

**/
STATIC
EFI_STATUS
EFIAPI
FreeType0Table (
  IN  CONST SMBIOS_TABLE_GENERATOR                  *       This,
  IN  CONST CM_STD_OBJ_SMBIOS_TABLE_INFO            * CONST SmbiosTableInfo,
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL    * CONST CfgMgrProtocol,
  IN        SMBIOS_STRUCTURE                       **       Table
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  FreePool(mArmType0);

  return Status;
}

/** The interface for the System Bios Table Generator.
*/
STATIC
CONST
SMBIOS_TABLE_GENERATOR PlatformSmbiosType0Generator = {
  // Generator ID
  CREATE_STD_SMBIOS_TABLE_GEN_ID (EStdSmbiosTableIdType00),
  // Generator Description
  L"SMBIOS.STD.TYPE0.GENERATOR",
  // Type
  0,
  // Build Table function
  BuildType0Table,
  // Extended Build Table function
  NULL,
  // Free Resource handle
  FreeType0Table
};

/** Register the Generator with the BIOS Table Factory.

  @param [in]  ImageHandle  The handle to the image.
  @param [in]  SystemTable  Pointer to the System Table.

  @retval EFI_SUCCESS           The Generator is registered.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_ALREADY_STARTED   The Generator for the Table ID
                                is already registered.
**/
EFI_STATUS
EFIAPI
SmbiosDefaultType0LibConstructor (
  IN CONST EFI_HANDLE                ImageHandle,
  IN       EFI_SYSTEM_TABLE  * CONST SystemTable
  )
{
  EFI_STATUS  Status;
  Status = RegisterSmbiosTableGenerator (&PlatformSmbiosType0Generator);
  DEBUG ((DEBUG_INFO, "DefaultType0  : Register Generator. Status = %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/** Deregister the Generator from the BIOS Table Factory.

  @param [in]  ImageHandle  The handle to the image.
  @param [in]  SystemTable  Pointer to the System Table.

  @retval EFI_SUCCESS           The Generator is deregistered.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The Generator is not registered.
**/
EFI_STATUS
EFIAPI
SmbiosDefaultType0LibDestructor (
  IN CONST EFI_HANDLE                ImageHandle,
  IN       EFI_SYSTEM_TABLE  * CONST SystemTable
  )
{
  EFI_STATUS  Status;
  Status = DeregisterSmbiosTableGenerator (&PlatformSmbiosType0Generator);
  DEBUG ((DEBUG_INFO, "DefaultType0  : Deregister Generator. Status = %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  return Status;
}
