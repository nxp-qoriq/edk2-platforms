/** @file
*
*  LS1046 defines used by ACPI tables
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _LS1046ARDB_PLATFORM_H_
#define _LS1046ARDB_PLATFORM_H_

#include <AcpiTableInclude/ls1046a.h>

#define EFI_ACPI_6_0_GICC_INTERFACES   {                                                                                              \
  EFI_ACPI_6_0_GICC_STRUCTURE_INIT(0, 0, GET_MPID(0, 0),EFI_ACPI_6_0_GIC_ENABLED, 138, GICC_BASE, GICV_BASE, GICH_BASE, 0x19, 0, 0),  \
  EFI_ACPI_6_0_GICC_STRUCTURE_INIT(0, 1, GET_MPID(0, 1),EFI_ACPI_6_0_GIC_ENABLED, 139, GICC_BASE, GICV_BASE, GICH_BASE, 0x19, 0, 0),  \
  EFI_ACPI_6_0_GICC_STRUCTURE_INIT(0, 2, GET_MPID(0, 2),EFI_ACPI_6_0_GIC_ENABLED, 127, GICC_BASE, GICV_BASE, GICH_BASE, 0x19, 0, 0),  \
  EFI_ACPI_6_0_GICC_STRUCTURE_INIT(0, 3, GET_MPID(0, 3),EFI_ACPI_6_0_GIC_ENABLED, 129, GICC_BASE, GICV_BASE, GICH_BASE, 0x19, 0, 0),  \
}

//Mcfg
#define MCFG_CONFIG_SET_SIZE  3
#define MCFG_CONFIG_REG_SET   {                                   \
                                {                                 \
                                  LS1046A_PCI_SEG0_CONFIG_BASE,   \
                                  LS1046A_PCI_SEG0,               \
                                  LS1046A_PCI_SEG_BUSNUM_MIN,     \
                                  LS1046A_PCI_SEG_BUSNUM_MAX,     \
                                  EFI_ACPI_RESERVED_DWORD,        \
                                },                                \
                                {                                 \
                                  LS1046A_PCI_SEG1_CONFIG_BASE,   \
                                  LS1046A_PCI_SEG1,               \
                                  LS1046A_PCI_SEG_BUSNUM_MIN,     \
                                  LS1046A_PCI_SEG_BUSNUM_MAX,     \
                                  EFI_ACPI_RESERVED_DWORD,        \
                                },                                \
                                {                                 \
                                  LS1046A_PCI_SEG2_CONFIG_BASE,   \
                                  LS1046A_PCI_SEG2,               \
                                  LS1046A_PCI_SEG_BUSNUM_MIN,     \
                                  LS1046A_PCI_SEG_BUSNUM_MAX,     \
                                  EFI_ACPI_RESERVED_DWORD,        \
                                }                                 \
                              }

#endif
