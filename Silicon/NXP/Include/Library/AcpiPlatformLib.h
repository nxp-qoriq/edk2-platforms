/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ACPI_PLATFORM_LIB_H
#define _ACPI_PLATFORM_LIB_H

#include <IndustryStandard/Acpi.h>
#include <Uefi.h>

/**
  Fixup the Acpi Table based on running SOC's properties.

  @param[in]  Table   The Acpi Table to fixup.

  @return EFI_SUCCESS       Successfully fixed up the ACPI Table
  @return EFI_DEVICE_ERROR  Could not fix the ACPI Table
**/
EFI_STATUS
AcpiPlatformFixup (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER      *TableHeader
  );

#endif
