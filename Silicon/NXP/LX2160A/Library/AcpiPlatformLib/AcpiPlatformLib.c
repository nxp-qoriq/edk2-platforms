/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Chassis.h>
#include <Library/AcpiPlatformLib.h>
#include <Library/DebugLib.h>

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
  UINT32  Svr;

  Svr = PcdGet32 (PcdSocSvr);
  if (SVR_MAJOR (Svr) == 2) {
    Table->OemRevision = 0x01;
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

  switch (TableHeader->Signature) {
    case EFI_ACPI_6_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE:
      Status = UpdateMcfg (TableHeader);
      break;

    default:
      Status = EFI_SUCCESS;
  }

  return Status;
}
