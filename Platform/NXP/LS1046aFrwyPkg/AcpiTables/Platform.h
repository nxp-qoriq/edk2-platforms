/** @file
*  LS1046 defines used by ACPI tables
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _LS1046AFRWY_PLATFORM_H_
#define _LS1046AFRWY_PLATFORM_H_

#include <AcpiTableInclude/ls1046a.h>

//Gic
#define EFI_ACPI_6_0_GICC_INTERFACES   {                                                                                              \
  EFI_ACPI_6_0_GICC_STRUCTURE_INIT(0, 0, GET_MPID(0, 0),EFI_ACPI_6_0_GIC_ENABLED, 138, GICC_BASE, GICV_BASE, GICH_BASE, 0x19, 0, 0),  \
  EFI_ACPI_6_0_GICC_STRUCTURE_INIT(0, 1, GET_MPID(0, 1),EFI_ACPI_6_0_GIC_ENABLED, 139, GICC_BASE, GICV_BASE, GICH_BASE, 0x19, 0, 0),  \
  EFI_ACPI_6_0_GICC_STRUCTURE_INIT(0, 2, GET_MPID(0, 2),EFI_ACPI_6_0_GIC_ENABLED, 127, GICC_BASE, GICV_BASE, GICH_BASE, 0x19, 0, 0),  \
  EFI_ACPI_6_0_GICC_STRUCTURE_INIT(0, 3, GET_MPID(0, 3),EFI_ACPI_6_0_GIC_ENABLED, 129, GICC_BASE, GICV_BASE, GICH_BASE, 0x19, 0, 0),  \
}

//Mcfg
#define MCFG_CONFIG_SET_SIZE  2
#define MCFG_CONFIG_REG_SET   {                                   \
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

// TMU
#define TMU_ACTIVE_LOW_THRESHOLD  3232       // Active low Thershold (50C)
#define TMU_ACTIVE_HIGH_THRESHOLD 3432       // Active high Thershold (70C)
#define TMU_FAN_1                 1          // FAN 1
#define TMU_FAN_2                 2          // FAN 2
#define TMU_FAN_3                 3          // FAN 3
#define TMU_FAN_4                 4          // FAN 4
#define TMU_FAN_5                 5          // FAN 5
#define TMU_FAN_OFF_SPEED         0x80       // FAN off speed value at 50% PWM (Default)
#define TMU_FAN_LOW_SPEED         0xBF       // FAN low speed value at 75% PWM
#define TMU_FAN_HIGH_SPEED        0xF0       // FAN high speed value at 95% PWM


#endif
