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
#define CONFIGURATION_MANAGER_REVISION CREATE_REVISION (1, 0)

/** The OEM ID
*/
#define CFG_MGR_OEM_ID    { 'N', 'X', 'P', 'L', 'T', 'D' }

#define CFG_MGR_TABLE_ID  SIGNATURE_64 ('L','X','2','1','6','0','A',' ')

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

/** The number of CPUs
*/
#define PLAT_CPU_COUNT          16

/** The number of ACPI tables to install
*/
#define PLAT_ACPI_TABLE_COUNT   10


/** A structure describing the platform configuration
    manager repository information
*/
typedef struct PlatformRepositoryInfo {
  /// Configuration Manager Information
  CM_STD_OBJ_CONFIGURATION_MANAGER_INFO     CmInfo;

  /// List of ACPI tables
  CM_STD_OBJ_ACPI_TABLE_INFO                CmAcpiTableList[PLAT_ACPI_TABLE_COUNT];

  /// Boot architecture information
  CM_ARM_BOOT_ARCH_INFO                     BootArchInfo;

  /// Power management profile information
  CM_ARM_POWER_MANAGEMENT_PROFILE_INFO      PmProfileInfo;

  /// LX2 Board Revision
  UINT32                                    Lx2160aRevision;
} EDKII_PLATFORM_REPOSITORY_INFO;

#endif // CONFIGURATION_MANAGER_H__
