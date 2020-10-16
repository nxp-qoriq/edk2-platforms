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

// Platform specific info needed by Configuration Manager

#define CFG_MGR_TABLE_ID  SIGNATURE_64 ('L','S','1','0','4','6',' ',' ')

#define UART1_BASE                0x21D0600
#define UART1_IT                  87

#define PLAT_PCI_SEG0_CONFIG_BASE   LS1046A_PCI_SEG0_CONFIG_BASE
#define PLAT_PCI_SEG0               LS1046A_PCI_SEG0
#define PLAT_PCI_SEG_BUSNUM_MIN     LS1046A_PCI_SEG_BUSNUM_MIN
#define PLAT_PCI_SEG_BUSNUM_MAX     LS1046A_PCI_SEG_BUSNUM_MAX
#define PLAT_PCI_SEG1_CONFIG_BASE   LS1046A_PCI_SEG1_CONFIG_BASE
#define PLAT_PCI_SEG1               LS1046A_PCI_SEG1
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
#define PLAT_PCI_CONFG_COUNT    3

#define PLAT_ITS_IDENTIFIER_MIN     1
#define PLAT_ITS_GROUP_MIN          1
#define PLAT_ITS_GROUP_COUNT        0
#define PLAT_ITS_IDENTIFIER_COUNT   0

#define PLAT_NAMED_COMPONENT_COUNT  3
#define PLAT_ID_MAPPING_COUNT       6+1 // 1 additional mapping, will be fixed dynamically
#define PLAT_ROOT_COMPLEX_COUNT     3
#define PLAT_SMMUV1_SMMUV2_COUNT    1
#define PLAT_SMMU_INTERRUPT_COUNT   32
#define PLAT_PMU_INTERRUPT_COUNT    0

#define PLAT_WATCHDOG_COUNT           0
#define PLAT_GIC_REDISTRIBUTOR_COUNT  0
#define PLAT_GIC_ITS_COUNT            0

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

#define PLAT_ITS_IDENTIFIER_ARRAY_INFO      \
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
    { /* Additional Mapping */                                                                                                  \
      0x100,                                                                                                                    \
      0,                                                                                                                        \
      0x040E,                                                                                                                   \
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
      1+1,                                                                                                                      \
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
      1,                                                                                                                        \
      (CM_OBJECT_TOKEN)((UINT8*)&FslPlatformRepositoryInfo + OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, IdMappingArray[2+1])),  \
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

#endif
