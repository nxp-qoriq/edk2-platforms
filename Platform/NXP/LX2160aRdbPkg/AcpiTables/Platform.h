/** @file
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*  Copyright 2018-2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under Platform/ARM/JunoPkg/AcpiTables/
*
**/


#ifndef _LX2160ARDB_PLATFORM_H_
#define _LX2160ARDB_PLATFORM_H_

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_ARM_OEM_ID             'N','X','P',' ',' ',' '   // OEMID
#define EFI_ACPI_ARM_OEM_TABLE_ID       SIGNATURE_64('L','X','2','1','6','0',' ',' ') // OEM table id, putting SOC name here
#define EFI_ACPI_ARM_OEM_REVISION       0x00000000
#define EFI_ACPI_ARM_CREATOR_ID         SIGNATURE_32('I','N','T','L')
#define EFI_ACPI_ARM_CREATOR_REVISION   0x20151124

//
//Dbg2 Table Information
//
#define NUM_DEBUG_DEVICE_INFO    1
#define NUM_GENERIC_ADDRESS      1
#define NAMESPACE_STRING_SIZE    12

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

// CPU
#define CPU_CLK1CSR_BASE        0x1370000
#define CPU_CLK2CSR_BASE        0x1370020
#define CPU_CLK3CSR_BASE        0x1370040
#define CPU_CLK4CSR_BASE        0x1370060
#define CPU_CLK5CSR_BASE        0x1370080
#define CPU_CLK6CSR_BASE        0x13700A0
#define CPU_CLK7CSR_BASE        0x13700C0
#define CPU_CLK8CSR_BASE        0x13700E0
#define CPU_CGA_PLL_DIV_1       0x00000000
#define CPU_CGA_PLL_DIV_2       0x08000000
#define CPU_CGA_PLL_DIV_4       0x10000000
#define CPU_CGB_PLL_DIV_1       0x20000000
#define CPU_CGB_PLL_DIV_2       0x28000000
#define CPU_CGB_PLL_DIV_4       0x30000000
#define CPU_PLL_TRNS_LATENCY    0
#define CPU_PWR_DISSP_PLL_DIV_1 200
#define CPU_PWR_DISSP_PLL_DIV_2 100
#define CPU_PWR_DISSP_PLL_DIV_4 50
#define CPU_FREQ_100_PRCNT      100
#define CPU_FREQ_50_PRCNT       50
#define CPU_FREQ_25_PRCNT       25
#define CPU_DWORD_ACCESS        3
#define CPU_NUM_ENTRIES         5
#define CPU_SW_ANY_CORD         0xFD
#define CPU_NUM_PROCESSOR       2
#define CPU_DOMAIN_0            0
#define CPU_DOMAIN_1            1
#define CPU_DOMAIN_2            2
#define CPU_DOMAIN_3            3
#define CPU_DOMAIN_4            4
#define CPU_DOMAIN_5            5
#define CPU_DOMAIN_6            6
#define CPU_DOMAIN_7            7

// PCLK
#define DCFG_BASE   0x1E00000
#define DCFG_LEN    0x1FFFF

// Gic
#define GIC_VERSION 3
#define GICD_BASE   0x6000000
#define GICI_BASE   0x6020000
#define GICR_BASE   0x06200000
#define GICR_LEN    0x200000
#define GICC_BASE   0x0c0c0000
#define GICH_BASE   0x0c0d0000
#define GICV_BASE   0x0c0e0000

// UART
#define UART0_BASE               0x21C0000
#define UART1_BASE               0x21D0000
#define UART2_BASE               0x21E0000
#define UART3_BASE               0x21F0000
#define UART0_IT                 64
#define UART1_IT                 65
#define UART2_IT                 104
#define UART3_IT                 105
#define UART_LEN                 0x10000
#define SPCR_FLOW_CONTROL_NONE   0

// I2C
#define I2C0_BASE          0x2000000
#define I2C1_BASE          0x2010000
#define I2C2_BASE          0x2020000
#define I2C3_BASE          0x2030000
#define I2C4_BASE          0x2040000
#define I2C5_BASE          0x2050000
#define I2C6_BASE          0x2060000
#define I2C7_BASE          0x2070000
#define I2C_LEN            0x10000
#define I2C0_IT            66
#define I2C1_IT            66
#define I2C2_IT            67
#define I2C3_IT            67
#define I2C4_IT            106
#define I2C5_IT            106
#define I2C6_IT            107
#define I2C7_IT            107
#define I2C0_MUX_CHANNEL_1 1
#define I2C0_MUX_CHANNEL_3 3

// Sata
#define SATA0_BASE 0x3200000
#define SATA1_BASE 0x3210000
#define SATA2_BASE 0x3220000
#define SATA3_BASE 0x3230000
#define SATA_LEN   0x10000
#define SATA0_IT_1 165
#define SATA0_IT_2 166
#define SATA0_IT_3 167
#define SATA1_IT_1 168
#define SATA1_IT_2 169
#define SATA1_IT_3 170
#define SATA2_IT_1 129
#define SATA2_IT_2 130
#define SATA2_IT_3 131
#define SATA3_IT_1 132
#define SATA3_IT_2 133
#define SATA3_IT_3 134

//USB
#define USB0_BASE 0x3100000
#define USB1_BASE 0x3110000
#define USB_LEN   0x10000
#define USB0_IT   112
#define USB1_IT   113

// TMU
#define TMU_BASE                  0x1f80000   // TMU CCSR space base address
#define TMU_LEN                   0x10000     // TMU CCSR space length
#define TMU_TEMP_RANGE_0          0x800000E6  // 230K
#define TMU_TEMP_RANGE_1          0x8001017D  // 381K
#define TMU_SITES_MAX             7           // Max TMU remote sensor
#define TMU_ACTIVE                0           // Cooling policy active
#define TMU_PASSIVE               1           // Cooling policy passive
#define TMU_PASSIVE_THERSHOLD     3582        // Passive Thershold (85C)
#define TMU_CRITICAL_THERSHOLD    3682        // Critical Thershold (95C)
#define TMU_ACTIVE_LOW_THERSHOLD  3232        // Active low Thershold (50C)
#define TMU_ACTIVE_HIGH_THERSHOLD 3432        // Active high Thershold (70C)
#define TMU_TIER_DISABLE_ALL      0x00        // Disable all interrupt
#define TMU_TIDR_DISABLE_ALL      0x00        // Clear all interrupt
#define TMU_TMR_DISABLE           0x00        // Disable TMU IP Monitoring mode
#define TMU_TMR_ENABLE            0x80000000  // Enable TMU IP in monitoring mode
#define TMU_TMTMIR_DEFAULT        0x00000007  // Monitoring interval ~2second
#define TMU_THERMAL_COFFICIENT_1  1           // Thermal cofficient 1 value as per the TMU datasheet
#define TMU_THERMAL_COFFICIENT_2  1           // Thermal cofficient 2 value as per the TMU datasheet
#define TMU_TZ_POLLING_PERIOD     150         // ThermalZone polling frequency ~15second
#define TMU_TZ_SAMPLING_PERIOD    50          // ThermalZone sampling frequency ~5second
#define TMU_SENSOR_ENABLE_ALL     0x0000007F  // Enable all sensor for active monitoring
#define TMU_POINT_0_TEMP_CFG      0x00000000  // TMU calibration data, point 0 @230K(-43)
#define TMU_POINT_0_SENSOR_CFG    0x00000035  // TMU calibration data, point 0 @230K(-43)
#define TMU_POINT_1_TEMP_CFG      0x00000001  // TMU calibration data, point 1 @381K(108C)
#define TMU_POINT_1_SENSOR_CFG    0x00000154  // TMU calibration data, point 0 @381K(108C)
#define TMU_SENSOR_READ_ADJUST    0x0000000E  // TMU sensor reading adjustment
#define TMU_ENGINEERING_MODE_0    0x51009C00  // TEUMR0 register
#define TMU_ENGINEERING_MODE_2    0x0800FFFE  // TEUMR2 register
#define TMU_FAN_1                 1           // FAN 1
#define TMU_FAN_2                 2           // FAN 2
#define TMU_FAN_3                 3           // FAN 3
#define TMU_FAN_4                 4           // FAN 4
#define TMU_FAN_5                 5           // FAN 5
#define TMU_FAN_OFF_SPEED         0x80        // FAN off speed value at 50% PWM (Default)
#define TMU_FAN_LOW_SPEED         0xBF        // FAN low speed value at 75% PWM
#define TMU_FAN_HIGH_SPEED        0xF0        // FAN high speed value at 95% PWM

// DSPI
#define SPI0_BASE 0x2100000
#define SPI1_BASE 0x2110000
#define SPI2_BASE 0x2120000
#define SPI_LEN   0x10000
#define SPI_IT    58

//Flex SPI
#define FSPI_STATUS 0x01
#define FSPI0_BASE 0x20c0000
#define FSPI_LEN   0x10000
#define FSPI_IT    57
#define FSPIMM_BASE 0x20000000
#define FSPIMM_LEN  0x10000000

// SD
#define SDC0_BASE 0x2140000
#define SDC1_BASE 0x2150000
#define SDC_LEN   0x10000
#define SDC0_IT   60
#define SDC1_IT   95

// MDIO
#define MDI0_BASE 0x8b96000
#define MDI0_IT   122
#define MDI1_BASE 0x8b97000
#define MDI1_IT   123
#define MDI_LEN   0x1000
#define AQR_PHY4_IT 34
#define AQR_PHY5_IT 35


// Timer
#define TIMER_BLOCK_COUNT            1
#define TIMER_FRAME_COUNT            4
#define TIMER_WATCHDOG_COUNT         1
#define TIMER_BASE_ADDRESS           0x23E0000 // a.k.a CNTControlBase
#define TIMER_READ_BASE_ADDRESS      0x23F0000 // a.k.a CNTReadBase
#define TIMER_GT_BLOCK_0_ADDRESS     0x2890000 // a.k.a CNTCTLBase (Secure)
#define TIMER_GT_BASE_0_ADDRESS      0x28A0000  // a.k.a CNTBase0
#define TIMER_GT_BASE_1_ADDRESS      0x28B0000  // a.k.a CNTBase1
#define TIMER_GT_BASE_2_ADDRESS      0x28C0000  // a.k.a CNTBase2
#define TIMER_GT_BASE_3_ADDRESS      0x28D0000  // a.k.a CNTBase3
#define TIMER_GT_BASE_0_EL0_ADDRESS  0x28E0000  // a.k.a CNTBase0EL0
#define TIMER_GT_BASE_2_EL0_ADDRESS  0x28F0000  // a.k.a CNTBase2EL0
#define TIMER_WDT0_REFRESH_BASE      0x2390000
#define TIMER_WDT0_CONTROL_BASE      0x23A0000
#define TIMER_SEC_IT                 29
#define TIMER_NON_SEC_IT             30
#define TIMER_VIRT_IT                27
#define TIMER_HYP_IT                 26
#define TIMER_FRAME0_IT              78
#define TIMER_FRAME1_IT              79
#define TIMER_FRAME2_IT              92
#define TIMER_FRAME3_IT              93
#define TIMER_WDT0_IT                91

#define DEFAULT_PLAT_FREQ            700000000

// Mcfg
#define LX2160A_PCI_SEG0_CONFIG_BASE 0x9000000000
#define LX2160A_PCI_SEG0             0x2
#define LX2160A_PCI_SEG_BUSNUM_MIN   0x0
#define LX2160A_PCI_SEG_BUSNUM_MAX   0xff
#define LX2160A_PCI_SEG1_CONFIG_BASE 0xA000000000
#define LX2160A_PCI_SEG1             0x4

// Pci
#define LX2160A_PCI_SEG0_CONFIG_BASE_MAX 0x900fffffff
#define LX2160A_PCI_SEG0_CONFIG_SIZE     0x10000000
#define LX2160A_PCI_SEG_BUSNUM_RANGE     0x100
#define LX2160A_PCI_SEG0_MMIO32_MIN      0x40000000
#define LX2160A_PCI_SEG0_MMIO32_MAX      0xffffffff
#define LX2160A_PCI_SEG0_MMIO32_XLATE    0x9000000000
#define LX2160A_PCI_SEG0_MMIO32_SIZE     0xc0000000
#define LX2160A_PCI_SEG0_MMIO64_MIN      0x9400000000
#define LX2160A_PCI_SEG0_MMIO64_MAX      0x97ffffffff
#define LX2160A_PCI_SEG0_MMIO64_SIZE     0x400000000
#define LX2160A_PCI_SEG0_INTA            151
#define LX2160A_PCI_SEG0_INTB            152
#define LX2160A_PCI_SEG0_INTC            153
#define LX2160A_PCI_SEG0_INTD            154
#define LX2160A_PCI_SEG0_RC_CONFIG_BASE  0x3600000
#define LX2160A_PCI_SEG0_RC_CONFIG_SIZE  0x100000
#define LX2160A_PCI_SEG1_CONFIG_BASE_MAX 0xA00fffffff
#define LX2160A_PCI_SEG1_CONFIG_SIZE     0x10000000
#define LX2160A_PCI_SEG1_MMIO32_MIN      0x40000000
#define LX2160A_PCI_SEG1_MMIO32_MAX      0xffffffff
#define LX2160A_PCI_SEG1_MMIO32_XLATE    0xA000000000
#define LX2160A_PCI_SEG1_MMIO32_SIZE     0xc0000000
#define LX2160A_PCI_SEG1_MMIO64_MIN      0xa400000000
#define LX2160A_PCI_SEG1_MMIO64_MAX      0xa7ffffffff
#define LX2160A_PCI_SEG1_MMIO64_SIZE     0x400000000
#define LX2160A_PCI_SEG1_INTA            161
#define LX2160A_PCI_SEG1_INTB            162
#define LX2160A_PCI_SEG1_INTC            163
#define LX2160A_PCI_SEG1_INTD            164
#define LX2160A_PCI_SEG1_RC_CONFIG_BASE  0x3800000
#define LX2160A_PCI_SEG1_RC_CONFIG_SIZE  0x100000

// IO
#define LX2160A_PCI_SEG0_IO64_MIN        0x00000000
#define LX2160A_PCI_SEG0_IO64_MAX        0x0000FFFF
#define LX2160A_PCI_SEG0_IO64_SIZE       0x10000
#define LX2160A_PCI_SEG0_IO64_XLATE      0x9010000000
#define LX2160A_PCI_SEG1_IO64_MIN        0x00000000
#define LX2160A_PCI_SEG1_IO64_MAX        0x0000FFFF
#define LX2160A_PCI_SEG1_IO64_SIZE       0x10000
#define LX2160A_PCI_SEG1_IO64_XLATE      0xA010000000

//Ftm
#define FTM_BASE 0x2800000
#define FTM_LEN  0x10000
#define FTM_IT   76

//Rcpm
#define RCPM_BASE 0x1e34040
#define RCPM_LEN  0x1000

// Stream IDs
#define NXP_DPAA2_STREAM_ID_START       23
#define NXP_DPAA2_STREAM_ID_COUNT       40
#define NXP_USB0_STREAM_ID              1
#define NXP_USB1_STREAM_ID              2
#define NXP_SDMMC0_STREAM_ID            3
#define NXP_SDMMC1_STREAM_ID            4
#define NXP_SATA0_STREAM_ID             5
#define NXP_SATA1_STREAM_ID             6
#define NXP_SATA2_STREAM_ID             7
#define NXP_SATA3_STREAM_ID             8
#define NXP_MC_LX2160A_STREAM_ID        0x4000

// Platform specific info needed by Configuration Manager

#define CFG_MGR_TABLE_ID  SIGNATURE_64 ('L','X','2','1','6','0',' ',' ')

#define PLAT_ITS_IDENTIFIER_MIN     1
#define PLAT_ITS_GROUP_MIN          1
#define PLAT_ITS_GROUP_COUNT        PLAT_ITS_GROUP_MIN
#define PLAT_ITS_IDENTIFIER_COUNT   PLAT_ITS_IDENTIFIER_MIN

#define PLAT_NAMED_COMPONENT_COUNT  9
#define PLAT_ID_MAPPING_COUNT       16
#define PLAT_ROOT_COMPLEX_COUNT     2
#define PLAT_SMMUV1_SMMUV2_COUNT    1
#define PLAT_SMMU_INTERRUPT_COUNT   64
#define PLAT_PMU_INTERRUPT_COUNT    10

#define PLAT_PCI_SEG0_CONFIG_BASE   LX2160A_PCI_SEG0_CONFIG_BASE
#define PLAT_PCI_SEG0               LX2160A_PCI_SEG0
#define PLAT_PCI_SEG_BUSNUM_MIN     LX2160A_PCI_SEG_BUSNUM_MIN
#define PLAT_PCI_SEG_BUSNUM_MAX     LX2160A_PCI_SEG_BUSNUM_MAX
#define PLAT_PCI_SEG1_CONFIG_BASE   LX2160A_PCI_SEG1_CONFIG_BASE
#define PLAT_PCI_SEG1               LX2160A_PCI_SEG1

#define PLAT_GIC_VERSION            GIC_VERSION
#define PLAT_GICD_BASE              GICD_BASE
#define PLAT_GICI_BASE              GICI_BASE
#define PLAT_GICR_BASE              GICR_BASE
#define PLAT_GICR_LEN               GICR_LEN
#define PLAT_GICC_BASE              GICC_BASE
#define PLAT_GICH_BASE              GICH_BASE
#define PLAT_GICV_BASE              GICV_BASE

#define PLAT_CPU_COUNT          16
#define PLAT_GTBLOCK_COUNT      1
#define PLAT_GTFRAME_COUNT      4
#define PLAT_PCI_CONFG_COUNT    2

#define PLAT_WATCHDOG_COUNT           1
#define PLAT_GIC_REDISTRIBUTOR_COUNT  1
#define PLAT_GIC_ITS_COUNT            1

/* GIC CPU Interface information
   GIC_ENTRY (CPUInterfaceNumber, Mpidr, PmuIrq, VGicIrq, EnergyEfficiency)
 */
#define PLAT_GIC_CPU_INTERFACE    {                         \
             GICC_ENTRY (0,  GET_MPID (0, 0), 23, 0x19, 0), \
             GICC_ENTRY (1,  GET_MPID (0, 1), 23, 0x19, 0), \
             GICC_ENTRY (2,  GET_MPID (1, 0), 23, 0x19, 0), \
             GICC_ENTRY (3,  GET_MPID (1, 1), 23, 0x19, 0), \
             GICC_ENTRY (4,  GET_MPID (2, 0), 23, 0x19, 0), \
             GICC_ENTRY (5,  GET_MPID (2, 1), 23, 0x19, 0), \
             GICC_ENTRY (6,  GET_MPID (3, 0), 23, 0x19, 0), \
             GICC_ENTRY (7,  GET_MPID (3, 1), 23, 0x19, 0), \
             GICC_ENTRY (8,  GET_MPID (4, 0), 23, 0x19, 0), \
             GICC_ENTRY (9,  GET_MPID (4, 1), 23, 0x19, 0), \
             GICC_ENTRY (10, GET_MPID (5, 0), 23, 0x19, 0), \
             GICC_ENTRY (11, GET_MPID (5, 1), 23, 0x19, 0), \
             GICC_ENTRY (12, GET_MPID (6, 0), 23, 0x19, 0), \
             GICC_ENTRY (13, GET_MPID (6, 1), 23, 0x19, 0), \
             GICC_ENTRY (14, GET_MPID (7, 0), 23, 0x19, 0), \
             GICC_ENTRY (15, GET_MPID (7, 1), 23, 0x19, 0)  \
}

#define PLAT_ITS_GROUP_NODE_INFO                                                                                                \
{                                                                                                                               \
  {                                                                                                                             \
    /* Reference token for this Iort node */                                                                                    \
    (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, ItsGroupInfo[0])),        \
    /* The number of ITS identifiers in the ITS node.*/                                                                         \
    PLAT_ITS_IDENTIFIER_COUNT,                                                                                                  \
    /* Reference token for the ITS identifier array */                                                                          \
    (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, ItsIdentifierArray[0])),  \
  }                                                                                                                             \
}                                                                                                                               \

#define PLAT_ITS_IDENTIFIER_ARRAY_INFO        \
  {                                           \
    {                                         \
      /* The ITS Identifier */                \
      0                                       \
    }                                         \
  }                                           \


// Interrupt Context Array
#define SMMU_INTERRUPT_CONTEXT_ARRAY          \
  {                                           \
    {                                         \
      178,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      179,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      180,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      181,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      182,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      183,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      184,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      185,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      186,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      187,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      188,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      189,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      190,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      191,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      192,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      193,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      194,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      195,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      196,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      197,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      198,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      199,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      200,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      201,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      202,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      203,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      204,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      205,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      206,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      207,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      208,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      209,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      210,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      211,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      212,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      213,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      214,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      215,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      216,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      217,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      218,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      219,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      220,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      221,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      222,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      223,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      224,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      225,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      226,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      227,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      228,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      229,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      230,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      231,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      232,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      233,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      234,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      235,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      236,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      237,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      238,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      239,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      240,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      241,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    }                                         \
  }                                           \

#define PMU_INTERRUPT_CONTEXT_ARRAY           \
  {                                           \
    {                                         \
      243,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      244,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      245,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      246,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      247,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      248,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      249,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      250,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      251,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      252,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    }                                         \
  }                                           \

// watchdogs
#define PLAT_WATCHDOG_INFO                    \
  {                                           \
      TIMER_WDT0_CONTROL_BASE,                \
      TIMER_WDT0_REFRESH_BASE,                \
      TIMER_WDT0_IT,                          \
      SBSA_WATCHDOG_FLAGS                     \
  }                                           \

#define PLAT_TIMER_BLOCK_INFO                                           \
  {                                                                     \
    {                                                                   \
      TIMER_GT_BLOCK_0_ADDRESS,                                         \
      PLAT_GTFRAME_COUNT,                                               \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo +            \
        OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, GTBlock0TimerInfo))  \
    }                                                                   \
  }                                                                     \

#define PLAT_TIMER_FRAME_INFO                                            \
  {                                                                      \
    {                                                                    \
      0,                             /* UINT8 GTFrameNumber */           \
      TIMER_GT_BASE_0_ADDRESS,       /* UINT64 CntBaseX */               \
      TIMER_GT_BASE_0_EL0_ADDRESS,   /* UINT64 CntEL0BaseX */            \
      TIMER_FRAME0_IT,               /* UINT32 GTxPhysicalTimerGSIV */   \
      GTDT_FRAME_FLAGS,              /* UINT32 GTxPhysicalTimerFlags */  \
      TIMER_FRAME0_IT,               /* UINT32 GTxVirtualTimerGSIV */    \
      GTDT_FRAME_FLAGS,              /* UINT32 GTxVirtualTimerFlags */   \
      0                              /* UINT32 GTxCommonFlags */         \
    }, /* Gtdt.Frames[0] */                                              \
    {                                                                    \
      1,                             /* UINT8 GTFrameNumber */           \
      TIMER_GT_BASE_1_ADDRESS,       /* UINT64 CntBaseX */               \
      GT_BLOCK_FRAME_RES_BASE,       /* UINT64 CntEL0BaseX */            \
      TIMER_FRAME1_IT,               /* UINT32 GTxPhysicalTimerGSIV */   \
      GTDT_FRAME_FLAGS,              /* UINT32 GTxPhysicalTimerFlags */  \
      0,                             /* UINT32 GTxVirtualTimerGSIV */    \
      0,                             /* UINT32 GTxVirtualTimerFlags */   \
      GTDT_FRAME_COMMON_FLAGS        /* UINT32 GTxCommonFlags */         \
    }, /* Gtdt.Frames[1] */                                              \
    {                                                                    \
      2,                              /* UINT8 GTFrameNumber */          \
      TIMER_GT_BASE_2_ADDRESS,        /* UINT64 CntBaseX */              \
      TIMER_GT_BASE_2_EL0_ADDRESS,    /* UINT64 CntEL0BaseX */           \
      TIMER_FRAME2_IT,                /* UINT32 GTxPhysicalTimerGSIV */  \
      GTDT_FRAME_FLAGS,               /* UINT32 GTxPhysicalTimerFlags */ \
      0,                              /* UINT32 GTxVirtualTimerGSIV */   \
      0,                              /* UINT32 GTxVirtualTimerFlags */  \
      GTDT_FRAME_COMMON_FLAGS         /* UINT32 GTxCommonFlags */        \
    },/* Gtdt.Frames[2] */                                               \
    {                                                                   \
      3,                             /* UINT8 GTFrameNumber */          \
      TIMER_GT_BASE_3_ADDRESS,       /* UINT64 CntBaseX */              \
      GT_BLOCK_FRAME_RES_BASE,       /* UINT64 CntEL0BaseX */           \
      TIMER_FRAME3_IT,               /* UINT32 GTxPhysicalTimerGSIV */  \
      GTDT_FRAME_FLAGS,              /* UINT32 GTxPhysicalTimerFlags */ \
      0,                             /* UINT32 GTxVirtualTimerGSIV */   \
      0,                             /* UINT32 GTxVirtualTimerFlags */  \
      GTDT_FRAME_COMMON_FLAGS        /* UINT32 GTxCommonFlags */        \
    }, /* Gtdt.Frames[3] */                                             \
  }                                                                     \

#define PLAT_GIC_DISTRIBUTOR_INFO                                      \
  {                                                                    \
    PLAT_GICD_BASE,                  /* UINT64  PhysicalBaseAddress */ \
    0,                               /* UINT32  SystemVectorBase */    \
    PLAT_GIC_VERSION                 /* UINT8   GicVersion */          \
  }                                                                    \

#define PLAT_GIC_REDISTRIBUTOR_INFO                                    \
  {                                                                    \
    PLAT_GICR_BASE,      /* UINT64 DiscoveryRangeBaseAddress */        \
    PLAT_GICR_LEN        /* UINT32 DiscoveryRangeLength */             \
  }                                                                    \

#define PLAT_GIC_ITS_INFO                                                    \
  {                                                                          \
    0,                   /* UINT32 GIC ITS ID */                             \
    PLAT_GICI_BASE,      /* UINT64 The 64-bit physical address for ITS */    \
    0                    /* UINT32 Populate the GIC ITS affinity in SRAT. */ \
  }                                                                          \

#define PLAT_MCFG_INFO                \
  {                                   \
    {                                 \
      PLAT_PCI_SEG0_CONFIG_BASE,      \
      PLAT_PCI_SEG0,                  \
      PLAT_PCI_SEG_BUSNUM_MIN,        \
      PLAT_PCI_SEG_BUSNUM_MAX,        \
    },                                \
    {                                 \
      PLAT_PCI_SEG1_CONFIG_BASE,      \
      PLAT_PCI_SEG1,                  \
      PLAT_PCI_SEG_BUSNUM_MIN,        \
      PLAT_PCI_SEG_BUSNUM_MAX,        \
    }                                 \
  }                                   \

#define PLAT_SPCR_INFO                                                            \
  {                                                                               \
    UART0_BASE,                                                                   \
    UART0_IT,                                                                     \
    115200,                                                                       \
    0,                                                                            \
    EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_ARM_PL011_UART  \
  }                                                                               \

#define PLAT_DBG2_INFO                                                            \
  {                                                                               \
    UART1_BASE,                                                                   \
    UART1_IT,                                                                     \
    115200,                                                                       \
    175000000,                                                                    \
    EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_ARM_SBSA_GENERIC_UART                       \
  }                                                                               \

#define PLAT_SSDT_FIXUP_INFO                                                      \
  {                                                                               \
    0x00,                                                                         \
    0x00,                                                                         \
    0x00,                                                                         \
    AQR_PHY4_IT                                                                   \
  }                                                                               \

#define PLAT_IORT_NAMED_COMPONENT_INFO                                                                                            \
  {                                                                                                                               \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[0])),  \
      2,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[0])),      \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      48,                                                                                                                         \
      "\\_SB_.MCE0"                                                                                                               \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[1])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[2])),      \
      0,                                                                                                                          \
      0,                                                                                                                          \
      0,                                                                                                                          \
      0,                                                                                                                          \
      40,                                                                                                                         \
      "\\_SB_.USB0"                                                                                                               \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[2])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[3])),      \
      0,                                                                                                                          \
      0,                                                                                                                          \
      0,                                                                                                                          \
      0,                                                                                                                          \
      40,                                                                                                                         \
      "\\_SB_.USB1"                                                                                                               \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[3])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[4])),      \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      48,                                                                                                                         \
      "\\_SB_.SDC0",                                                                                                              \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[4])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[5])),      \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      48,                                                                                                                         \
      "\\_SB_.SDC1",                                                                                                              \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[5])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[6])),      \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      48,                                                                                                                         \
      "\\_SB_.SAT0",                                                                                                              \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[6])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[7])),      \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      48,                                                                                                                         \
      "\\_SB_.SAT1",                                                                                                              \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[7])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[8])),      \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      48,                                                                                                                         \
      "\\_SB_.SAT2",                                                                                                              \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[8])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[9])),      \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      48,                                                                                                                         \
      "\\_SB_.SAT3",                                                                                                              \
    }                                                                                                                             \
  }                                                                                                                               \

#define PLAT_IORT_ID_MAPPING_INFO                                                                                               \
  {                                                                                                                             \
    {                                                                                                                           \
      NXP_MC_LX2160A_STREAM_ID,                                                                                                 \
      0,                                                                                                                        \
      NXP_MC_LX2160A_STREAM_ID,                                                                                                 \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_DPAA2_STREAM_ID_START,                                                                                                \
      NXP_DPAA2_STREAM_ID_COUNT - 1,                                                                                            \
      NXP_DPAA2_STREAM_ID_START,                                                                                                \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_USB0_STREAM_ID,                                                                                                       \
      0,                                                                                                                        \
      NXP_USB0_STREAM_ID,                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_USB1_STREAM_ID,                                                                                                       \
      0,                                                                                                                        \
      NXP_USB1_STREAM_ID,                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_SDMMC0_STREAM_ID,                                                                                                     \
      0,                                                                                                                        \
      NXP_SDMMC0_STREAM_ID,                                                                                                     \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_SDMMC1_STREAM_ID,                                                                                                     \
      0,                                                                                                                        \
      NXP_SDMMC1_STREAM_ID,                                                                                                     \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_SATA0_STREAM_ID,                                                                                                      \
      0,                                                                                                                        \
      NXP_SATA0_STREAM_ID,                                                                                                      \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_SATA1_STREAM_ID,                                                                                                      \
      0,                                                                                                                        \
      NXP_SATA1_STREAM_ID,                                                                                                      \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_SATA2_STREAM_ID,                                                                                                      \
      0,                                                                                                                        \
      NXP_SATA2_STREAM_ID,                                                                                                      \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_SATA3_STREAM_ID,                                                                                                      \
      0,                                                                                                                        \
      NXP_SATA3_STREAM_ID,                                                                                                      \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      0,                                                                                                                        \
      0,                                                                                                                        \
      0x1800,                                                                                                                   \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      0,                                                                                                                        \
      0,                                                                                                                        \
      0x2800,                                                                                                                   \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_MC_LX2160A_STREAM_ID,                                                                                                 \
      0,                                                                                                                        \
      NXP_MC_LX2160A_STREAM_ID,                                                                                                 \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, ItsGroupInfo[0])),      \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      NXP_DPAA2_STREAM_ID_START,                                                                                                \
      NXP_DPAA2_STREAM_ID_COUNT - 1,                                                                                            \
      NXP_DPAA2_STREAM_ID_START,                                                                                                \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, ItsGroupInfo[0])),      \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      0x1800,                                                                                                                   \
      0,                                                                                                                        \
      0x1800,                                                                                                                   \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, ItsGroupInfo[0])),      \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      0x2800,                                                                                                                   \
      0,                                                                                                                        \
      0x2800,                                                                                                                   \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, ItsGroupInfo[0])),      \
      0                                                                                                                         \
    }                                                                                                                           \
  }                                                                                                                             \

#define PLAT_IORT_ROOT_COMPLEX_INFO                                                                                             \
{                                                                                                                               \
  /* node 1 info */                                                                                                             \
  {                                                                                                                             \
    (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, RootComplexInfo[0])),     \
      1,                                                                                                                        \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[10])),   \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                        \
      0,                                                                                                                        \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                 \
      EFI_ACPI_IORT_ROOT_COMPLEX_ATS_UNSUPPORTED,                                                                               \
      PLAT_PCI_SEG0,  /* refer to Pci.asl */                                                                                    \
  },                                                                                                                            \
    /* node 2 info */                                                                                                           \
  {                                                                                                                             \
    (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, RootComplexInfo[1])),     \
    1,                                                                                                                          \
    (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[11])),     \
    EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
    0,                                                                                                                          \
    EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
    EFI_ACPI_IORT_ROOT_COMPLEX_ATS_UNSUPPORTED,                                                                                 \
    PLAT_PCI_SEG1,  /* refer to Pci.asl */                                                                                      \
  }                                                                                                                             \
}                                                                                                                               \

#define PLAT_IORT_SMMU_NODE_INFO                                                                                                    \
{                                                                                                                                   \
  {                                                                                                                                 \
    (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),        \
      4,                                                                                                                            \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[12])),       \
      0x5000000,                                                                                                                    \
      0x800000,                                                                                                                     \
      EFI_ACPI_IORT_SMMUv1v2_MODEL_MMU500,                                                                                          \
      EFI_ACPI_IORT_SMMUv1v2_FLAG_DVM | EFI_ACPI_IORT_SMMUv1v2_FLAG_COH_WALK,                                                       \
      64,                                                                                                                           \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, InterruptContextArray[0])), \
      10,                                                                                                                           \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PmuInterruptArray[0])),     \
      47,                                                                                                                           \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL,                                                                                        \
      48,                                                                                                                           \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL                                                                                         \
  }                                                                                                                                 \
}                                                                                                                                   \

#endif
