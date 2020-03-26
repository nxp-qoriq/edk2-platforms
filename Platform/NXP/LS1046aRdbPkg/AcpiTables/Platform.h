/** @file
*
*  LS1046 defines used by ACPI tables
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on ARM/JunoPkg/AcpiTables/AcpiTables.inf
*
**/

#ifndef _LS1046ARDB_PLATFORM_H_
#define _LS1046ARDB_PLATFORM_H_

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_ARM_OEM_ID             'N','X','P',' ',' ',' '   // OEMID
#define EFI_ACPI_ARM_OEM_TABLE_ID       SIGNATURE_64('L','S','1','0','4','6',' ',' ') // OEM table id, putting SOC name here
#define EFI_ACPI_ARM_OEM_REVISION       0x00000000
#define EFI_ACPI_ARM_CREATOR_ID         SIGNATURE_32('A','R','M', ' ')
#define EFI_ACPI_ARM_CREATOR_REVISION   0x20151124

// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
#define ARM_ACPI_HEADER(Signature, Type, Revision) {              \
    Signature,                      /* UINT32  Signature */       \
    sizeof (Type),                  /* UINT32  Length */          \
    Revision,                       /* UINT8   Revision */        \
    0,                              /* UINT8   Checksum */        \
    { EFI_ACPI_ARM_OEM_ID },        /* UINT8   OemId[6] */        \
    EFI_ACPI_ARM_OEM_TABLE_ID,      /* UINT64  OemTableId */      \
    EFI_ACPI_ARM_OEM_REVISION,      /* UINT32  OemRevision */     \
    EFI_ACPI_ARM_CREATOR_ID,        /* UINT32  CreatorId */       \
    EFI_ACPI_ARM_CREATOR_REVISION   /* UINT32  CreatorRevision */ \
  }

// Gic
#define GIC_VERSION 2
#define GICD_BASE   0x1410000
#define GICC_BASE   0x142f000
#define GICH_BASE   0x1440000
#define GICV_BASE   0x1460000

// UART
#define UART0_BASE               0x21C0500
#define UART0_IT                 86
#define UART0_LENGTH             0x100
#define SPCR_FLOW_CONTROL_NONE   0

// Timer
#define TIMER_BLOCK_COUNT            1
#define TIMER_FRAME_COUNT            4
#define TIMER_WATCHDOG_COUNT         1
#define TIMER_BASE_ADDRESS           0x23E0000 // a.k.a CNTControlBase
#define TIMER_READ_BASE_ADDRESS      0x23F0000 // a.k.a CNTReadBase
#define TIMER_SEC_IT                 29
#define TIMER_NON_SEC_IT             30
#define TIMER_VIRT_IT                27
#define TIMER_HYP_IT                 26
#define TIMER_FRAME0_IT              78
#define TIMER_FRAME1_IT              79
#define TIMER_FRAME2_IT              92

#define DEFAULT_PLAT_FREQ            700000000

//WDT
#define WDT0_BASE    0x2ad0000
#define WDT0_LENGTH  0x10000
#define WDT0_IT      115
// Mcfg
#define LS1046A_PCI_SEG0_CONFIG_BASE 0x4000000000
#define LS1046A_PCI_SEG0             0x0
#define LS1046A_PCI_SEG_BUSNUM_MIN   0x0
#define LS1046A_PCI_SEG_BUSNUM_MAX   0xff
#define LS1046A_PCI_SEG1_CONFIG_BASE 0x4800000000
#define LS1046A_PCI_SEG2_CONFIG_BASE 0x5000000000
#define LS1046A_PCI_SEG1             0x1
#define LS1046A_PCI_SEG2             0x2

// Pci
#define LS1046A_PCI_SEG0_CONFIG_BASE_MAX 0x400fffffff
#define LS1046A_PCI_SEG0_CONFIG_SIZE     0x10000000
#define LS1046A_PCI_SEG_BUSNUM_RANGE     0x100
#define LS1046A_PCI_SEG0_MMIO64_MIN      0x40000000
#define LS1046A_PCI_SEG0_MMIO64_MAX      0x7fffffff
#define LS1046A_PCI_SEG0_MMIO64_XLATE    0x4000000000
#define LS1046A_PCI_SEG0_MMIO64_SIZE     0x40000000
#define LS1046A_PCI_SEG0_INTA            142
#define LS1046A_PCI_SEG0_RC_CONFIG_BASE  0x3400000
#define LS1046A_PCI_SEG0_RC_CONFIG_SIZE  0x100000

#define LS1046A_PCI_SEG1_CONFIG_BASE_MAX 0x480fffffff
#define LS1046A_PCI_SEG1_CONFIG_SIZE     0x10000000
#define LS1046A_PCI_SEG1_MMIO64_MIN      0x40000000
#define LS1046A_PCI_SEG1_MMIO64_MAX      0x7fffffff
#define LS1046A_PCI_SEG1_MMIO64_XLATE    0x4800000000
#define LS1046A_PCI_SEG1_MMIO64_SIZE     0x40000000
#define LS1046A_PCI_SEG1_INTA            152
#define LS1046A_PCI_SEG1_RC_CONFIG_BASE  0x3500000
#define LS1046A_PCI_SEG1_RC_CONFIG_SIZE  0x100000

#define LS1046A_PCI_SEG2_CONFIG_BASE_MAX 0x500fffffff
#define LS1046A_PCI_SEG2_CONFIG_SIZE     0x10000000
#define LS1046A_PCI_SEG2_MMIO64_MIN      0x40000000
#define LS1046A_PCI_SEG2_MMIO64_MAX      0x7fffffff
#define LS1046A_PCI_SEG2_MMIO64_XLATE    0x5000000000
#define LS1046A_PCI_SEG2_MMIO64_SIZE     0x40000000
#define LS1046A_PCI_SEG2_INTA            186
#define LS1046A_PCI_SEG2_RC_CONFIG_BASE  0x3600000
#define LS1046A_PCI_SEG2_RC_CONFIG_SIZE  0x100000

//USB
#define USB0_BASE 0x2f00000
#define USB1_BASE 0x3000000
#define USB_LEN   0x10000
#define USB0_IT   92
#define USB1_IT   93

#endif
