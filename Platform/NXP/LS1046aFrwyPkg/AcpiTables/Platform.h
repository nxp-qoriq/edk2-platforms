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

#ifndef _LS1046AFRWY_PLATFORM_H_
#define _LS1046AFRWY_PLATFORM_H_

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

// PCLK : Dynamic Clock
#define DCFG_BASE   0x1EE0000  /* Device configuration data Base Address */
#define DCFG_LEN    0xFFF      /* Device configuration data length */

// DSPI
#define SPI0_BASE 0x2100000
#define SPI1_BASE 0x2110000
#define SPI2_BASE 0x2120000
#define SPI_LEN   0x10000
#define SPI_IT    58

//Quad SPI
#define QSPI0_BASE 0x1550000    /* QSPI CSSR Base Address */
#define QSPI_LEN   0x10000      /* QSPI CSSR Memory length */
#define QSPI_IT    131          /* QSPI Interrupt Identifier */
#define QSPIMM_BASE 0x40000000  /* QSPI Fixed Memory Map Base Address */
#define QSPIMM_LEN  0x20000000  /* QSPI Fixed Memory Map Length */

// I2C
#define I2C0_BASE 0x2180000
#define I2C1_BASE 0x2190000
#define I2C2_BASE 0x21A0000
#define I2C3_BASE 0x21B0000
#define I2C_LEN   0x10000
#define I2C0_IT   88
#define I2C1_IT   89
#define I2C2_IT   90
#define I2C3_IT   91

// TMU
#define TMU_BASE                 0x1F00000   // TMU CCSR space base address
#define TMU_LEN                  0x10000     // TMU CCSR space length
#define TMU_TEMP_RANGE_0         0x000B0000  // Temp range register 0
#define TMU_TEMP_RANGE_1         0x0009002A  // Temp range register 1
#define TMU_TEMP_RANGE_2         0x0006004C  // Temp range register 2
#define TMU_TEMP_RANGE_3         0x00070062  // Temp range register 3
#define TMU_SITES_MAX            16          // Max TMU remote sensor// Using 5
#define TMU_ACTIVE               0           // Cooling policy active
#define TMU_PASSIVE              1           // Cooling policy passive
#define TMU_PASSIVE_THRESHOLD    3582        // Passive Threshold: 84.8 degree C
#define TMU_CRITICAL_THRESHOLD   3682        // Critical Threshold: 94.8 degree C
#define TMU_TIER_DISABLE_ALL     0x00000000  // Disable all interrupts
#define TMU_TIDR_DISABLE_ALL     0x00000000  // Clear all interrupts
#define TMU_TMR_DISABLE          0x00000000  // Disable TMU IP Monitoring mode
#define TMU_TMR_SITES_ENABLE     0x0000F800  // Enable Monitoring Sites in TMU
#define TMU_TMR_ENABLE           0x8000F800  // Enable TMU IP in Monitoring mode
#define TMU_TMTMIR_DEFAULT       0x00000007  // Monitoring interval ~1.79seconds
#define TMU_THERMAL_COFFICIENT_1 1           // Thermal cofficient 1
#define TMU_THERMAL_COFFICIENT_2 1           // Thermal cofficient 2
#define TMU_TZ_POLLING_PERIOD    150         // ThermalZone polling frequency ~15 seconds
#define TMU_TZ_SAMPLING_PERIOD   50          // ThermalZone sampling frequency ~5 seconds
#define TMU_ENGINEERING_MODE     0x8000007F  // TEUMR0 register

/*Calibration data group 1 : TTR0CR, 12 points ar 0°C*/
#define TMU_POINT_1_0_TEMP_CFG     0x00000000
#define TMU_POINT_1_0_SENSOR_CFG   0x00000023
#define TMU_POINT_1_1_TEMP_CFG     0x00000001
#define TMU_POINT_1_1_SENSOR_CFG   0x00000029
#define TMU_POINT_1_2_TEMP_CFG     0x00000002
#define TMU_POINT_1_2_SENSOR_CFG   0x0000002F
#define TMU_POINT_1_3_TEMP_CFG     0x00000003
#define TMU_POINT_1_3_SENSOR_CFG   0x00000036
#define TMU_POINT_1_4_TEMP_CFG     0x00000004
#define TMU_POINT_1_4_SENSOR_CFG   0x0000003C
#define TMU_POINT_1_5_TEMP_CFG     0x00000005
#define TMU_POINT_1_5_SENSOR_CFG   0x00000042
#define TMU_POINT_1_6_TEMP_CFG     0x00000006
#define TMU_POINT_1_6_SENSOR_CFG   0x00000049
#define TMU_POINT_1_7_TEMP_CFG     0x00000007
#define TMU_POINT_1_7_SENSOR_CFG   0x0000004F
#define TMU_POINT_1_8_TEMP_CFG     0x00000008
#define TMU_POINT_1_8_SENSOR_CFG   0x00000055
#define TMU_POINT_1_9_TEMP_CFG     0x00000009
#define TMU_POINT_1_9_SENSOR_CFG   0x0000005C
#define TMU_POINT_1_10_TEMP_CFG     0x0000000A
#define TMU_POINT_1_10_SENSOR_CFG   0x00000062
#define TMU_POINT_1_11_TEMP_CFG     0x0000000B
#define TMU_POINT_1_11_SENSOR_CFG   0x00000068

/*Calibration data group 2 : TTR1CR, 10 points at 42°C*/
#define TMU_POINT_2_0_TEMP_CFG     0x00010000
#define TMU_POINT_2_0_SENSOR_CFG   0x00000022
#define TMU_POINT_2_1_TEMP_CFG     0x00010001
#define TMU_POINT_2_1_SENSOR_CFG   0x0000002A
#define TMU_POINT_2_2_TEMP_CFG     0x00010002
#define TMU_POINT_2_2_SENSOR_CFG   0x00000032
#define TMU_POINT_2_3_TEMP_CFG     0x00010003
#define TMU_POINT_2_3_SENSOR_CFG   0x0000003A
#define TMU_POINT_2_4_TEMP_CFG     0x00010004
#define TMU_POINT_2_4_SENSOR_CFG   0x00000042
#define TMU_POINT_2_5_TEMP_CFG     0x00010005
#define TMU_POINT_2_5_SENSOR_CFG   0x0000004A
#define TMU_POINT_2_6_TEMP_CFG     0x00010006
#define TMU_POINT_2_6_SENSOR_CFG   0x00000052
#define TMU_POINT_2_7_TEMP_CFG     0x00010007
#define TMU_POINT_2_7_SENSOR_CFG   0x0000005A
#define TMU_POINT_2_8_TEMP_CFG     0x00010008
#define TMU_POINT_2_8_SENSOR_CFG   0x00000062
#define TMU_POINT_2_9_TEMP_CFG     0x00010009
#define TMU_POINT_2_9_SENSOR_CFG   0x0000006A

/*Calibration data group 3 : TTR2CR, 7 points at 76°C */
#define TMU_POINT_3_0_TEMP_CFG     0x00020000
#define TMU_POINT_3_0_SENSOR_CFG   0x00000021
#define TMU_POINT_3_1_TEMP_CFG     0x00020001
#define TMU_POINT_3_1_SENSOR_CFG   0x0000002B
#define TMU_POINT_3_2_TEMP_CFG     0x00020002
#define TMU_POINT_3_2_SENSOR_CFG   0x00000035
#define TMU_POINT_3_3_TEMP_CFG     0x00020003
#define TMU_POINT_3_3_SENSOR_CFG   0x0000003E
#define TMU_POINT_3_4_TEMP_CFG     0x00020004
#define TMU_POINT_3_4_SENSOR_CFG   0x00000048
#define TMU_POINT_3_5_TEMP_CFG     0x00020005
#define TMU_POINT_3_5_SENSOR_CFG   0x00000052
#define TMU_POINT_3_6_TEMP_CFG     0x00020006
#define TMU_POINT_3_6_SENSOR_CFG   0x0000005C

/*Calibration data group 4 : TTR3CR, 8 points at 98°C */
#define TMU_POINT_4_0_TEMP_CFG     0x00030000
#define TMU_POINT_4_0_SENSOR_CFG   0x00000011
#define TMU_POINT_4_1_TEMP_CFG     0x00030001
#define TMU_POINT_4_1_SENSOR_CFG   0x0000001A
#define TMU_POINT_4_2_TEMP_CFG     0x00030002
#define TMU_POINT_4_2_SENSOR_CFG   0x00000024
#define TMU_POINT_4_3_TEMP_CFG     0x00030003
#define TMU_POINT_4_3_SENSOR_CFG   0x0000002E
#define TMU_POINT_4_4_TEMP_CFG     0x00030004
#define TMU_POINT_4_4_SENSOR_CFG   0x00000038
#define TMU_POINT_4_5_TEMP_CFG     0x00030005
#define TMU_POINT_4_5_SENSOR_CFG   0x00000042
#define TMU_POINT_4_6_TEMP_CFG     0x00030006
#define TMU_POINT_4_6_SENSOR_CFG   0x0000004C
#define TMU_POINT_4_7_TEMP_CFG     0x00030007
#define TMU_POINT_4_7_SENSOR_CFG   0x00000056

#endif
