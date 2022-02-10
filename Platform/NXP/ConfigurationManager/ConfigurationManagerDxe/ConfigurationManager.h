/** @file

  Copyright (c) 2020 Puresoftware Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#ifndef CONFIGURATION_MANAGER_H__
#define CONFIGURATION_MANAGER_H__

/** The configuration manager version
*/
#define CONFIGURATION_MANAGER_REVISION CREATE_REVISION (0, 0)

/** The OEM ID
*/
#define CFG_MGR_OEM_ID    { 'N', 'X', 'P', ' ', ' ', ' ' }

/** A helper macro for populating the GIC CPU information
 */
#define GICC_ENTRY(                                                   \
    CPUInterfaceNumber,                                               \
    Mpidr,                                                            \
    PmuIrq,                                                           \
    VGicIrq,                                                          \
    EnergyEfficiency                                                  \
    ) {                                                               \
  CPUInterfaceNumber,       /* UINT32  CPUInterfaceNumber         */  \
  CPUInterfaceNumber,       /* UINT32  AcpiProcessorUid           */  \
  EFI_ACPI_6_2_GIC_ENABLED, /* UINT32  Flags                      */  \
  0,                        /* UINT32  ParkingProtocolVersion     */  \
  PmuIrq,                   /* UINT32  PerformanceInterruptGsiv   */  \
  0,                        /* UINT64  ParkedAddress              */  \
  GICC_BASE,                /* UINT64  PhysicalBaseAddress        */  \
  GICV_BASE,                /* UINT64  GICV                       */  \
  GICH_BASE,                /* UINT64  GICH                       */  \
  VGicIrq,                  /* UINT32  VGICMaintenanceInterrupt   */  \
  0,                        /* UINT64  GICRBaseAddress            */  \
  Mpidr,                    /* UINT64  MPIDR                      */  \
  EnergyEfficiency          /* UINT8   ProcessorPowerEfficiency   */  \
}

/** A helper macro for returning configuration manager objects
*/
#define HANDLE_CM_OBJECT(ObjId, CmObjectId, Object, ObjectCount)      \
  case ObjId: {                                                       \
    CmObject->ObjectId = CmObjectId;                                  \
    CmObject->Size = sizeof (Object);                                 \
    CmObject->Data = (VOID*)&Object;                                  \
    CmObject->Count = ObjectCount;                                    \
    DEBUG ((                                                          \
      DEBUG_INFO,                                                     \
      #CmObjectId ": Ptr = 0x%p, Size = %d, Count = %d\n",            \
      CmObject->Data,                                                 \
      CmObject->Size,                                                 \
      CmObject->Count                                                 \
      ));                                                             \
    break;                                                            \
  }

/** A helper macro for returning configuration manager objects
    referenced by token
*/
#define HANDLE_CM_OBJECT_REF_BY_TOKEN(                                      \
          ObjId,                                                            \
          CmObjectId,                                                       \
          Object,                                                           \
          ObjectCount,                                                      \
          Token,                                                            \
          HandlerProc                                                       \
          )                                                                 \
  case ObjId: {                                                             \
    CmObject->ObjectId = CmObjectId;                                        \
    if (Token == CM_NULL_TOKEN) {                                           \
      CmObject->Size = sizeof (Object);                                     \
      CmObject->Data = (VOID*)&Object;                                      \
      CmObject->Count = ObjectCount;                                        \
      DEBUG ((                                                              \
        DEBUG_INFO,                                                         \
        #CmObjectId ": Ptr = 0x%p, Size = %d, Count = %d\n",                \
        CmObject->Data,                                                     \
        CmObject->Size,                                                     \
        CmObject->Count                                                     \
        ));                                                                 \
    } else {                                                                \
      Status = HandlerProc (This, CmObjectId, Token, CmObject);             \
      DEBUG ((                                                              \
        DEBUG_INFO,                                                         \
        #CmObjectId ": Token = 0x%p, Ptr = 0x%p, Size = %d, Count = %d\n",  \
        (VOID*)Token,                                                       \
        CmObject->Data,                                                     \
        CmObject->Size,                                                     \
        CmObject->Count                                                     \
        ));                                                                 \
    }                                                                       \
    break;                                                                  \
  }

/** A helper macro for returning configuration manager objects referenced
    by token when the entire platform repository is in scope and the
    CM_NULL_TOKEN value is not allowed.
*/
#define HANDLE_CM_OBJECT_SEARCH_PLAT_REPO(                                  \
          ObjId,                                                            \
          CmObjectId,                                                       \
          Token,                                                            \
          HandlerProc                                                       \
          )                                                                 \
  case ObjId: {                                                             \
    CmObject->ObjectId = CmObjectId;                                        \
    if (Token == CM_NULL_TOKEN) {                                           \
      Status = EFI_INVALID_PARAMETER;                                       \
      DEBUG ((                                                              \
        DEBUG_ERROR,                                                        \
        #ObjId ": CM_NULL_TOKEN value is not allowed when searching"        \
        " the entire platform repository.\n"                                \
        ));                                                                 \
    } else {                                                                \
      Status = HandlerProc (This, CmObjectId, Token, CmObject);             \
      DEBUG ((                                                              \
        DEBUG_INFO,                                                         \
        #ObjId ": Token = 0x%p, Ptr = 0x%p, Size = %d, Count = %d\n",       \
        (VOID*)Token,                                                       \
        CmObject->Data,                                                     \
        CmObject->Size,                                                     \
        CmObject->Count                                                     \
        ));                                                                 \
    }                                                                       \
    break;                                                                  \
  }

/** The number of ACPI tables to install
*/
#define PLAT_ACPI_TABLE_COUNT   10

/** The number of SMBIOS tables to install
*/
#define PLAT_SMBIOS_TABLE_COUNT 10

/** A structure describing the platform configuration
    manager repository information
*/
typedef struct PlatformRepositoryInfo {
  /// Configuration Manager Information
  CM_STD_OBJ_CONFIGURATION_MANAGER_INFO     CmInfo;

  /// List of ACPI tables
  CM_STD_OBJ_ACPI_TABLE_INFO                CmAcpiTableList[PLAT_ACPI_TABLE_COUNT];

  /// List of SMBIOS tables
  CM_STD_OBJ_SMBIOS_TABLE_INFO              CmSmbiosTableList[PLAT_SMBIOS_TABLE_COUNT];

  /// Boot architecture information
  CM_ARM_BOOT_ARCH_INFO                     BootArchInfo;

  /// Power management profile information
  CM_ARM_POWER_MANAGEMENT_PROFILE_INFO      PmProfileInfo;

  /// Generic timer information
  CM_ARM_GENERIC_TIMER_INFO                 GenericTimerInfo;

  /// Generic timer block information
  CM_ARM_GTBLOCK_INFO                       GTBlockInfo[PLAT_GTBLOCK_COUNT];

  /// Generic timer frame information
  CM_ARM_GTBLOCK_TIMER_FRAME_INFO           GTBlock0TimerInfo[PLAT_GTFRAME_COUNT];

  /// Watchdog information
  CM_ARM_GENERIC_WATCHDOG_INFO              Watchdog;

  /// GIC CPU interface information
  CM_ARM_GICC_INFO                          GicCInfo[PLAT_CPU_COUNT];

  /// GIC distributor information
  CM_ARM_GICD_INFO                          GicDInfo;

  /// GIC Redistributor information
  CM_ARM_GIC_REDIST_INFO                    GicRedistInfo;

  /// GIC ITS information
  CM_ARM_GIC_ITS_INFO                       GicItsInfo;

  /// PCI configuration space information
  CM_ARM_PCI_CONFIG_SPACE_INFO              PciConfigInfo[PLAT_PCI_CONFG_COUNT];

  /// Serial port information for serial port console redirection port
  CM_ARM_SERIAL_PORT_INFO                   SpcrSerialPort;

  /// Serial port information for the DBG2 UART port
  CM_ARM_SERIAL_PORT_INFO                   DbgSerialPort;

  /// ITS Group node
  CM_ARM_ITS_GROUP_NODE                     ItsGroupInfo[PLAT_ITS_GROUP_MIN];

  /// ITS Identifier array
  CM_ARM_ITS_IDENTIFIER                     ItsIdentifierArray[PLAT_ITS_IDENTIFIER_MIN];

  /// Named component node array
  CM_ARM_NAMED_COMPONENT_NODE               NamedComponentInfo[PLAT_NAMED_COMPONENT_COUNT];

  /// Array of DeviceID mapping
  CM_ARM_ID_MAPPING                         IdMappingArray[PLAT_ID_MAPPING_COUNT];

  /// PCI Root complex node
  CM_ARM_ROOT_COMPLEX_NODE                  RootComplexInfo[PLAT_ROOT_COMPLEX_COUNT];

  /// SMMUv1/2 node
  CM_ARM_SMMUV1_SMMUV2_NODE                 SmmuV1SmmuV2Info[PLAT_SMMUV1_SMMUV2_COUNT];

  // SMMU Interupt Contexts
  CM_ARM_SMMU_INTERRUPT                     InterruptContextArray[PLAT_SMMU_INTERRUPT_COUNT];

  // SMMU Interupt Contexts
  CM_ARM_SMMU_INTERRUPT                     PmuInterruptArray[PLAT_PMU_INTERRUPT_COUNT];

  /// Type 0 Table System bios info
  CM_ARM_SYSTEM_BIOS_TYPE0_INFO             Type0SystemBiosInfo;

  /// Type 1 Table System info
  CM_ARM_SYSTEM_TYPE1_INFO                  Type1SystemInfo;

  /// Type 3 Table System Chassis info
  CM_ARM_SYSTEM_CHASSIS_TYPE3_INFO          Type3SystemChassisInfo;

  /// Type 4 Processor Attributes info
  CM_ARM_PROCESSOR_ATTR_TYPE4_INFO          Type4ProcessorAttrInfo;

  /// Type 7 CPU Cache info
  CM_ARM_CPU_CACHE_DEVICE_TYPE7_INFO        Type7CpuCacheDeviceInfo;

  /// Type 9 Table System slot info
  CM_ARM_SYSTEM_SLOT_TYPE9_INFO             Type9SystemSlotInfo[PLAT_SYSTEM_SLOT_SIZE];

  /// Type16 Table memory array info
  CM_ARM_MEMORY_ARRAY_TYPE16_INFO           Type16MemoryArray;

  /// Type 17 Table for memory devices
  CM_ARM_MEMORY_DEVICE_TYPE17_INFO          Type17MemoryDeviceInfo;

  /// Type19 Table memory mapped address
  CM_ARM_MEM_MAP_ADDRESS_TYPE19_INFO        Type19MemoryMappedAddressInfo;

  /// Type32 Table system boot info
  CM_ARM_SYSTEM_BOOT_TYPE32_INFO            Type32SystemBootInfo;

  /// SSDT FixUp information
  CM_ARM_SSDT_FIXUP_INFO                    SsdtFixupInfo;

  /// PPTT Proc Hierarchy Info
  CM_ARM_PROC_HIERARCHY_INFO                PpttProcHeirInfo[PLAT_PROC_HEIR_NODES];

  /// PPTT ARM Cache Info
  CM_ARM_CACHE_INFO                         PpttCacheInfo[PLAT_CACHE_NODES];

  /// CM Object Reference
  CM_ARM_OBJ_REF                            PpttObjRefToken[PLAT_REF_TOKENS];

  /// Fsl Board Revision
  UINT32                                    FslBoardRevision;
} EDKII_PLATFORM_REPOSITORY_INFO;

/*
 * GTDT_GTIMER_FLAGS
 * IT trigger (Level/Edge- Bit 0) and Polarity (Low/High) Bit 1
 * Set bit-0 is 0 (Level trigger), Bit 1 1 (Active low)
 */
#define GTDT_GTIMER_FLAGS           (EFI_ACPI_6_1_GTDT_TIMER_FLAG_TIMER_INTERRUPT_POLARITY)

/*
 * Timer Frame IT High Level triggered
 * IT trigger (Level/Edge- Bit 0) and Polarity (Low/High) Bit 1
 * Set  bit-0 is 0 (Level trigger), Bit 1 0 (Active High)
 */
#define GTDT_FRAME_FLAGS 0

/*
 * Timer frame status
 * Access - Secure or non secure <-- Bit 0
 * State - Save (meaning always on) or Lose Context  <-- Bit 1
 * Set Bit 0 1 as Secure and Bit 1 zero as lose context
*/
#define GTDT_FRAME_COMMON_FLAGS EFI_ACPI_6_1_GTDT_GT_BLOCK_COMMON_FLAG_SECURE_TIMER

/*
 * Watchdog flags
 * IT trigger (Level/Edge- Bit 0), Polarity (Low/High) Bit 1, Secured Bit 2
 * Set Level trigger (Bit 0 as 0)
 * Active High (Bit 1 as 0)
 * Non secure (Bit 2 as 0)
 */
#define SBSA_WATCHDOG_FLAGS 0
#define SBSA_SEC_WATCHDOG_FLAGS EFI_ACPI_6_1_GTDT_GT_BLOCK_COMMON_FLAG_SECURE_TIMER

#define GT_BLOCK_FRAME_RES_BASE  MAX_UINT64

#endif // CONFIGURATION_MANAGER_H__
