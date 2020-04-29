/** @file
*  Memory Mapped Configuration Space Access (MCFG) Table
*
*  Copyright 2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef __NXP_MCFG_REMAPPING_TABLE_H__
#define __NXP_MCFG_REMAPPING_TABLE_H__

#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>

#pragma pack(1)

typedef struct {
   EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER Acpi_Table_Mcfg;
   EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE Config_Structure[2];
} NXP_EFI_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_TABLE;

#pragma pack()
#endif
