/** @NxpIoRemappingTable
*  I/O Remapping Table (Iort)
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware
*
*  SPDX-License-Identifier: BSD-2-Clause
*
**/
#ifndef __NXP_IO_REMAPPING_TABLE_H__
#define __NXP_IO_REMAPPING_TABLE_H__

#include <IndustryStandard/IoRemappingTable.h>

#pragma pack(1)

typedef struct
{
  EFI_ACPI_6_0_IO_REMAPPING_ITS_NODE          ItsNode;
  UINT32                                      ItsIdentifiers[1];
} NXP_EFI_ACPI_6_0_IO_REMAPPING_ITS_NODE;

typedef struct
{
  EFI_ACPI_6_0_IO_REMAPPING_RC_NODE         PciRcNode;
  //
  // 32 mappings per PCIE controller
  //
  EFI_ACPI_6_0_IO_REMAPPING_ID_TABLE        PciRcIdMapping[32];
} NXP_EFI_ACPI_6_0_IO_REMAPPING_RC_NODE;

typedef struct
{
  EFI_ACPI_6_0_IO_REMAPPING_SMMU_NODE         SmmuNode;
  EFI_ACPI_6_0_IO_REMAPPING_SMMU_INT          ContextInterrupt[64];
  EFI_ACPI_6_0_IO_REMAPPING_SMMU_INT          PmuInterrupt[10];
  //
  // 32 mappings per PCIE controller + 2 mapping for MC bus
  //
  EFI_ACPI_6_0_IO_REMAPPING_ID_TABLE          SmmuID[66];
} NXP_EFI_ACPI_6_0_IO_REMAPPING_SMMU_NODE;

typedef struct
{
  EFI_ACPI_6_0_IO_REMAPPING_NAMED_COMP_NODE   NamedNode;
  UINT8                                       ObjectName[20];
  EFI_ACPI_6_0_IO_REMAPPING_ID_TABLE          NamedID[2];
} NXP_EFI_ACPI_6_0_IO_REMAPPING_NAMED_COMP_NODE;

typedef struct
{
  EFI_ACPI_6_0_IO_REMAPPING_TABLE                  Header;
  NXP_EFI_ACPI_6_0_IO_REMAPPING_SMMU_NODE          SmmuNode;
  NXP_EFI_ACPI_6_0_IO_REMAPPING_RC_NODE            PciRcNode[3];
  NXP_EFI_ACPI_6_0_IO_REMAPPING_NAMED_COMP_NODE    NamedCompNode[9];
} NXP_EFI_ACPI_6_0_IO_REMAPPING_TABLE;


#endif
