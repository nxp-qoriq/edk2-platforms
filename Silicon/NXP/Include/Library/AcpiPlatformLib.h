/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ACPI_PLATFORM_LIB_H
#define _ACPI_PLATFORM_LIB_H

#include <IndustryStandard/Acpi.h>
#include <Uefi.h>

#include <Protocol/AcpiSystemDescriptionTable.h>

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

//
// DPAA1 DSDT Fixups
//
//#define ACPI_DEBUG
#ifdef ACPI_DEBUG
#define DBG(arg...) DEBUG((DEBUG_ERROR,## arg))
#else
#define DBG(arg...)
#endif

#define EFI_ACPI_MAX_NUM_TABLES   20
#define DSDT_SIGNATURE            0x54445344
#define ACPI_BMAN_HID             "NXP0021"
#define ACPI_QMAN_HID             "NXP0028"
#define ACPI_MAC_HID              "NXP0025"
#define ACPI_RESERVED_MEM_KEY     "mem-base"
#define ACPI_MAC_ADD_KEY          "mac-address"

enum dev_id_type {
  QMAN_DEV,
  BMAN_DEV,
  MAC_DEV,
  MAX_DEV,
};

EFI_STATUS
PlatformProcessDSDT (
    EFI_ACPI_SDT_PROTOCOL *AcpiTableProtocol,
    EFI_ACPI_HANDLE TableHandle
    );

void
PlatformAcpiCheckSum (
    IN OUT  EFI_ACPI_SDT_HEADER *Table
    );

#endif
