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

// Platform specific info needed by Configuration Manager

#define CFG_MGR_TABLE_ID  SIGNATURE_64 ('L','S','1','0','4','6',' ',' ')

#define UART1_BASE                0x21D0600
#define UART1_IT                  87

#define PLAT_PCI_SEG0               LS1046A_PCI_SEG0
#define PLAT_PCI_SEG1_CONFIG_BASE   LS1046A_PCI_SEG1_CONFIG_BASE
#define PLAT_PCI_SEG1               LS1046A_PCI_SEG1
#define PLAT_PCI_SEG_BUSNUM_MIN     LS1046A_PCI_SEG_BUSNUM_MIN
#define PLAT_PCI_SEG_BUSNUM_MAX     LS1046A_PCI_SEG_BUSNUM_MAX
#define PLAT_PCI_SEG2_CONFIG_BASE   LS1046A_PCI_SEG2_CONFIG_BASE
#define PLAT_PCI_SEG2               LS1046A_PCI_SEG2

#define PLAT_GIC_VERSION            GIC_VERSION
#define PLAT_GICD_BASE              GICD_BASE
#define PLAT_GICI_BASE              GICI_BASE
#define PLAT_GICR_BASE              GICR_BASE
#define PLAT_GICR_LEN               GICR_LEN
#define PLAT_GICC_BASE              GICC_BASE
#define PLAT_GICH_BASE              GICH_BASE
#define PLAT_GICV_BASE              GICV_BASE

#define PLAT_CPU_COUNT          4
#define PLAT_GTBLOCK_COUNT      0
#define PLAT_GTFRAME_COUNT      0
#define PLAT_PCI_CONFG_COUNT    2

#define PLAT_ITS_IDENTIFIER_MIN     1
#define PLAT_ITS_GROUP_MIN          1
#define PLAT_ITS_GROUP_COUNT        0
#define PLAT_ITS_IDENTIFIER_COUNT   0

#define PLAT_NAMED_COMPONENT_COUNT  3
#define PLAT_ID_MAPPING_COUNT       (6+1) // HACK: 1 additional mapping (0x040d) is created
#define PLAT_ROOT_COMPLEX_COUNT     3
#define PLAT_SMMUV1_SMMUV2_COUNT    1
#define PLAT_SMMU_INTERRUPT_COUNT   32
#define PLAT_PMU_INTERRUPT_COUNT    0

#define PLAT_WATCHDOG_COUNT           0
#define PLAT_GIC_REDISTRIBUTOR_COUNT  0
#define PLAT_GIC_ITS_COUNT            0

//PPTT Table Info

#define PLAT_PROC_HEIR_NODES     6
#define PLAT_CACHE_NODES         9
#define PLAT_REF_TOKENS          9

#define L2CACHE_ATRRIBUTES  (EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE |         \
                             (EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_UNIFIED << 2) |     \
                             (EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK << 4))

#define DCACHE_ATRRIBUTES   (EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ_WRITE |         \
                             (EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_DATA << 2) |        \
                             (EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK << 4))

#define ICACHE_ATRRIBUTES   (EFI_ACPI_6_3_CACHE_ATTRIBUTES_ALLOCATION_READ |               \
                             (EFI_ACPI_6_3_CACHE_ATTRIBUTES_CACHE_TYPE_INSTRUCTION << 2) | \
                             (EFI_ACPI_6_3_CACHE_ATTRIBUTES_WRITE_POLICY_WRITE_BACK << 4))

#define  PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE0    (EFI_ACPI_6_3_PPTT_PACKAGE_PHYSICAL |                \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_ID_INVALID << 1) |     \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD << 2) |  \
                                                  (EFI_ACPI_6_3_PPTT_NODE_IS_NOT_LEAF << 3) |         \
                                                  (EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL << 4))

#define PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE1     (EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL |            \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_ID_INVALID << 1) |     \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD << 2) |  \
                                                  (EFI_ACPI_6_3_PPTT_NODE_IS_NOT_LEAF << 3) |         \
                                                  (EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL << 4))

#define  PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE2    (EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL |            \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID << 1) |       \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD << 2) |  \
                                                  (EFI_ACPI_6_3_PPTT_NODE_IS_LEAF << 3) |             \
                                                  (EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL << 4))

#define PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE3     (EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL |            \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID << 1) |       \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD << 2) |  \
                                                  (EFI_ACPI_6_3_PPTT_NODE_IS_LEAF << 3) |             \
                                                  (EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL << 4))

#define PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE4     (EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL |            \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID << 1) |       \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD << 2) |  \
                                                  (EFI_ACPI_6_3_PPTT_NODE_IS_LEAF << 3) |             \
                                                  (EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL << 4))

#define PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE5     (EFI_ACPI_6_3_PPTT_PACKAGE_NOT_PHYSICAL |            \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_ID_VALID << 1) |       \
                                                  (EFI_ACPI_6_3_PPTT_PROCESSOR_IS_NOT_THREAD << 2) |  \
                                                  (EFI_ACPI_6_3_PPTT_NODE_IS_LEAF << 3) |             \
                                                  (EFI_ACPI_6_3_PPTT_IMPLEMENTATION_IDENTICAL << 4))

/* GIC CPU Interface information
   GIC_ENTRY (CPUInterfaceNumber, Mpidr, PmuIrq, VGicIrq, EnergyEfficiency)
 */
#define PLAT_GIC_CPU_INTERFACE    {                          \
             GICC_ENTRY (0,  GET_MPID (0, 0), 138, 0x19, 0), \
             GICC_ENTRY (1,  GET_MPID (0, 1), 139, 0x19, 0), \
             GICC_ENTRY (2,  GET_MPID (0, 2), 127, 0x19, 0), \
             GICC_ENTRY (3,  GET_MPID (0, 3), 129, 0x19, 0), \
}

#define PLAT_WATCHDOG_INFO                    \
  {                                           \
  }                                           \

#define PLAT_TIMER_BLOCK_INFO                 \
  {                                           \
  }                                           \

#define PLAT_TIMER_FRAME_INFO                 \
  {                                           \
  }                                           \

#define PLAT_GIC_DISTRIBUTOR_INFO                                      \
  {                                                                    \
    PLAT_GICD_BASE,                  /* UINT64  PhysicalBaseAddress */ \
    0,                               /* UINT32  SystemVectorBase */    \
    PLAT_GIC_VERSION                 /* UINT8   GicVersion */          \
  }                                                                    \

#define PLAT_GIC_REDISTRIBUTOR_INFO                                    \
  {                                                                    \
  }                                                                    \

#define PLAT_GIC_ITS_INFO                                              \
  {                                                                    \
  }                                                                    \

#define PLAT_MCFG_INFO                \
  {                                   \
    {                                 \
      PLAT_PCI_SEG1_CONFIG_BASE,      \
      PLAT_PCI_SEG1,                  \
      PLAT_PCI_SEG_BUSNUM_MIN,        \
      PLAT_PCI_SEG_BUSNUM_MAX,        \
    },                                \
    {                                 \
      PLAT_PCI_SEG2_CONFIG_BASE,      \
      PLAT_PCI_SEG2,                  \
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
    EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_16550           \
  }                                                                               \

#define PLAT_DBG2_INFO                                                            \
  {                                                                               \
    UART1_BASE,                                                                   \
    UART1_IT,                                                                     \
    115200,                                                                       \
    0,                                                                            \
    EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_16550_SUBSET_COMPATIBLE_WITH_MS_DBGP_SPEC   \
  }                                                                               \

#define PLAT_SSDT_FIXUP_INFO                                                      \
  {                                                                               \
    0x00,                                                                         \
    0x00,                                                                         \
    0x00,                                                                         \
    0x00                                                                          \
  }                                                                               \

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
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    },                                        \
    {                                         \
      174,                                    \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL   \
    }                                         \
  }                                           \

#define PMU_INTERRUPT_CONTEXT_ARRAY           \
  {                                           \
  }                                           \

#define PLAT_REF_NODES_INFO                                                                                                   \
  {                                                                                                                           \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[0]))},   \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[1]))},   \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[2]))},   \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[3]))},   \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[4]))},   \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[5]))},   \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[6]))},   \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[7]))},   \
    {(CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[8]))}    \
  }

#define PLAT_ARM_CACHE_INFO                                                                                                 \
  {                                                                                                                         \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[0])), \
      CM_NULL_TOKEN,                                                                                                        \
      SIZE_2MB,                                                                                                             \
      1024,                                                                                                                 \
      16,                                                                                                                   \
      (UINT8)L2CACHE_ATRRIBUTES,                                                                                            \
      64                                                                                                                    \
    },                                                                                                                      \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[1])), \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[2])), \
      SIZE_32KB,                                                                                                            \
      128,                                                                                                                  \
      2,                                                                                                                    \
      (UINT8)DCACHE_ATRRIBUTES,                                                                                             \
      64                                                                                                                    \
    },                                                                                                                      \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[2])), \
      CM_NULL_TOKEN,                                                                                                        \
      3*SIZE_16KB,                                                                                                          \
      192,                                                                                                                  \
      3,                                                                                                                    \
      (UINT8)ICACHE_ATRRIBUTES,                                                                                             \
      64                                                                                                                    \
    },                                                                                                                      \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[3])), \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[4])), \
      SIZE_32KB,                                                                                                            \
      128,                                                                                                                  \
      2,                                                                                                                    \
      (UINT8)DCACHE_ATRRIBUTES,                                                                                             \
      64                                                                                                                    \
    },                                                                                                                      \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[4])), \
      CM_NULL_TOKEN,                                                                                                        \
      3*SIZE_16KB,                                                                                                          \
      192,                                                                                                                  \
      3,                                                                                                                    \
      (UINT8)ICACHE_ATRRIBUTES,                                                                                             \
      64                                                                                                                    \
    },                                                                                                                      \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[5])), \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[6])), \
      SIZE_32KB,                                                                                                            \
      128,                                                                                                                  \
      2,                                                                                                                    \
      (UINT8)DCACHE_ATRRIBUTES,                                                                                             \
      64                                                                                                                    \
    },                                                                                                                      \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[6])), \
      CM_NULL_TOKEN,                                                                                                        \
      3*SIZE_16KB,                                                                                                          \
      192,                                                                                                                  \
      3,                                                                                                                    \
      (UINT8)ICACHE_ATRRIBUTES,                                                                                             \
      64                                                                                                                    \
    },                                                                                                                      \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[7])), \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[8])), \
      SIZE_32KB,                                                                                                            \
      128,                                                                                                                  \
      2,                                                                                                                    \
      (UINT8)DCACHE_ATRRIBUTES,                                                                                             \
      64                                                                                                                    \
    },                                                                                                                      \
    {                                                                                                                       \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[8])), \
      CM_NULL_TOKEN,                                                                                                        \
      3*SIZE_16KB,                                                                                                          \
      192,                                                                                                                  \
      3,                                                                                                                    \
      (UINT8)ICACHE_ATRRIBUTES,                                                                                             \
      64                                                                                                                    \
    }                                                                                                                       \
  }                                                                                                                         \

#define PLAT_PROC_HIERARCHY_INFO                                                                                                  \
  {                                                                                                                               \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[0])),    \
      (UINT32)PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE0,                                                                               \
      CM_NULL_TOKEN,                                                                                                              \
      CM_NULL_TOKEN,                                                                                                              \
      0,                                                                                                                          \
      CM_NULL_TOKEN                                                                                                               \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[1])),    \
      (UINT32) PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE1,                                                                              \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[0])),    \
      CM_NULL_TOKEN,                                                                                                              \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[0]))        \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[2])),    \
      (UINT32) PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE2,                                                                              \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[1])),    \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, GicCInfo[0])),            \
      2,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[1]))        \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[3])),    \
      (UINT32) PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE3,                                                                              \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[1])),    \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, GicCInfo[1])),            \
      2,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[3]))        \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[4])),    \
      (UINT32) PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE4,                                                                              \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[1])),    \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, GicCInfo[2])),            \
      2,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[5]))        \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[5])),    \
      (UINT32) PPTT_STRUCTURE_PROCESSOR_FLAGS_NODE5,                                                                              \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttProcHeirInfo[1])),    \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, GicCInfo[3])),            \
      2,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PpttCacheInfo[7]))        \
    }                                                                                                                             \
  }

#define PLAT_IORT_NAMED_COMPONENT_INFO                                                                                            \
  {                                                                                                                               \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[0])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[3+1])),    \
      0,  /* Flags */                                                                                                             \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,  /* Cache Coherent */                                                                    \
      0,  /* Alloc Hint */                                                                                                        \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      40, /* Address Size Limit */                                                                                                \
      "\\_SB_.USB0"                                                                                                               \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[1])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[4+1])),    \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,  /* Cache Coherent */                                                                    \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      40,                                                                                                                         \
      "\\_SB_.USB1"                                                                                                               \
    },                                                                                                                            \
    {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, NamedComponentInfo[2])),  \
      1,                                                                                                                          \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[5+1])),    \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                          \
      0,                                                                                                                          \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                   \
      40,                                                                                                                         \
      "\\_SB_.SAT0",                                                                                                              \
    }                                                                                                                             \
  }                                                                                                                               \

#define PLAT_IORT_ID_MAPPING_INFO                                                                                               \
  {                                                                                                                             \
    {                                                                                                                           \
      0,                                                                                                                        \
      0,                                                                                                                        \
      0x040b,                                                                                                                   \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      0,                                                                                                                        \
      0,                                                                                                                        \
      0x040d,                                                                                                                   \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      0,                                                                                                                        \
      0,                                                                                                                        \
      0x040f,                                                                                                                   \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      0                                                                                                                         \
    },                                                                                                                          \
    { /* Additional Mapping */                                                                                                  \
      0x100,                                                                                                                    \
      0,                                                                                                                        \
      0x040d,                                                                                                                   \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      0                                                                                                                         \
    },                                                                                                                          \
    {                                                                                                                           \
      0xc00 | NXP_USB0_STREAM_ID,                                                                                               \
      0,                                                                                                                        \
      0xc00 | NXP_USB0_STREAM_ID,                                                                                               \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      0xc00 | NXP_USB1_STREAM_ID,                                                                                               \
      0,                                                                                                                        \
      0xc00 | NXP_USB1_STREAM_ID,                                                                                               \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    },                                                                                                                          \
    {                                                                                                                           \
      0xc00 | NXP_SATA0_STREAM_ID,                                                                                              \
      0,                                                                                                                        \
      0xc00 | NXP_SATA0_STREAM_ID,                                                                                              \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),  \
      EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE                                                                                     \
    }                                                                                                                           \
  }                                                                                                                             \

#define PLAT_IORT_ROOT_COMPLEX_INFO                                                                                             \
{                                                                                                                               \
  /* node 1 info */                                                                                                             \
  {                                                                                                                             \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, RootComplexInfo[0])),   \
      1,                                                                                                                        \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[0])),    \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                        \
      0,                                                                                                                        \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                 \
      EFI_ACPI_IORT_ROOT_COMPLEX_ATS_UNSUPPORTED,                                                                               \
      PLAT_PCI_SEG0,  /* refer to Pci.asl */                                                                                    \
  },                                                                                                                            \
    /* node 2 info */                                                                                                           \
    {                                                                                                                           \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, RootComplexInfo[1])),   \
      1,                                                                                                                        \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[1])),    \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                        \
      0,                                                                                                                        \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                 \
      EFI_ACPI_IORT_ROOT_COMPLEX_ATS_UNSUPPORTED,                                                                               \
      PLAT_PCI_SEG1,  /* refer to Pci.asl */                                                                                    \
    },                                                                                                                          \
    /* node 3 info */                                                                                                           \
    {                                                                                                                           \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, RootComplexInfo[2])),   \
      1+1,                                                                                                                      \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[2])),    \
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,                                                                                        \
      0,                                                                                                                        \
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM | EFI_ACPI_IORT_MEM_ACCESS_FLAGS_DACS,                                                 \
      EFI_ACPI_IORT_ROOT_COMPLEX_ATS_UNSUPPORTED,                                                                               \
      PLAT_PCI_SEG2,  /* refer to Pci.asl */                                                                                    \
    }                                                                                                                           \
}                                                                                                                               \

#define PLAT_IORT_SMMU_NODE_INFO                                                                                                    \
{                                                                                                                                   \
  {                                                                                                                                 \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, SmmuV1SmmuV2Info[0])),      \
      0,                                                                                                                            \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[0])),        \
      0x9000000,                                                                                                                    \
      0x400000,                                                                                                                     \
      EFI_ACPI_IORT_SMMUv1v2_MODEL_MMU500,                                                                                          \
      EFI_ACPI_IORT_SMMUv1v2_FLAG_DVM | EFI_ACPI_IORT_SMMUv1v2_FLAG_COH_WALK,                                                       \
      32,                                                                                                                           \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, InterruptContextArray[0])), \
      0,                                                                                                                            \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, PmuInterruptArray[0])),     \
      174,                                                                                                                          \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL,                                                                                        \
      174,                                                                                                                          \
      EFI_ACPI_IORT_SMMUv1v2_INT_FLAG_LEVEL                                                                                         \
  }                                                                                                                                 \
}                                                                                                                                   \

// Platform specific smbios defines for Dynamic consumed by Configuration Manager

#define PLAT_SMBIOS_UUID  \
  { 0x2e1887cd, 0xc0c1, 0xea11, { 0xde,0xb3,0x02,0x42,0xac,0x13,0x00,0x04 }}  /* UUID */
#define PLAT_SMBIOS_ENCLOSURE_TYPE      23    /* enclosure type (rack mount) */
#define PLAT_SMBIOS_CPU_MAX_SPEED       1800  /* max speed */
#define PLAT_SMBIOS_CPU_CUR_SPEED       1600  /* current speed */
#define PLAT_SMBIOS_CPU_MAX_COUNT       4     /* core count in socket */
#define PLAT_SMBIOS_CPU_ENABLE_COUNT    4     /* enabled core count in socket */
#define PLAT_SMBIOS_MAX_L2_CACHE        0x800 /* 2 MB max L2 cache */
#define PLAT_SMBIOS_INSTALLED_L2_CACHE  0x800 /* 2 MB installed L2 Cache */
#define PLAT_SYSTEM_SLOT_SIZE           2
#define PLAT_SMBIOS_PCI_1_EXP_GEN_TYPE  SlotTypePciExpressGen3X1
#define PLAT_SMBIOS_PCI_1_BUS_WIDTH     SlotDataBusWidth1X
#define PLAT_SMBIOS_PCI_2_EXP_GEN_TYPE  SlotTypePciExpressGen3X1
#define PLAT_SMBIOS_PCI_2_BUS_WIDTH     SlotDataBusWidth1X
#define PLAT_SMBIOS_PCI_1_BUS_NUM       0x0
#define PLAT_SMBIOS_PCI_2_BUS_NUM       0x1
#define PLAT_SMBIOS_MEMORY_SIZE         0x400000  /* 4GB */
#define PLAT_SMBIOS_MEMORY_FORM         0x05      /* Chip */
#define PLAT_SMBIOS_MEMORY_SET          1         /* not part of a set */
#define PLAT_SMBIOS_MEMORY_TYPE         MemoryTypeSdram /* SDRAM */
#define PLAT_SMBIOS_MEMOPRY_TYPE_DETAIL {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0} /* unkmown */
#define PLATFORM_SMBIOS_MEMORY_SPEED    2100  /* 2100Mhz DDR */
#define PLAT_SMBIOS_MEMORY_START        0x80000000 /* starting addr of first 32GB */
#define PLAT_SMBIOS_PROD_NAME           2
#define PLAT_SMBIOS_SYSTEM_SLOT_INFO        \
  {                                         \
    {                                       \
      4,                                    \
      PLAT_SMBIOS_PCI_1_EXP_GEN_TYPE,       \
      PLAT_SMBIOS_PCI_1_BUS_WIDTH,          \
      SlotUsageAvailable,                   \
      SlotLengthShort,                      \
      0,                 /* SlotId */       \
      {1},               /* unknown */      \
      {1,0,0},           /* PME and SMBUS */\
      0x0,               /* Segment */      \
      PLAT_SMBIOS_PCI_1_BUS_NUM,            \
      0x0                /* DevFunc */      \
    },                                      \
    {                                       \
      4,                                    \
      PLAT_SMBIOS_PCI_2_EXP_GEN_TYPE,       \
      PLAT_SMBIOS_PCI_2_BUS_WIDTH,          \
      SlotUsageAvailable,                   \
      SlotLengthShort,                      \
      0,                                    \
      {1},                                  \
      {1,0,0},                              \
      0x1,                                  \
      PLAT_SMBIOS_PCI_2_BUS_NUM,            \
      0x0                                   \
    }                                       \
  }                                         \

#endif
