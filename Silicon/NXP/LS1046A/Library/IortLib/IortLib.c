/** @IortLib.c
  IORT Library for LS1046A-RDB/FRWY board, containing functions to
  configure IORT table parameters

  Copyright 2020 NXP
  Copyright 2020 Puresoftware

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/IortLib.h>

#include <Library/NxpIoRemappingTable.h>

/**
  ID mappings represent the formula by which an ID from a source is converted to
  an ID in a destination.

  This function doesn't check the table for memory footprint. i.e. it's assumed
  that the memory for ID mapping is avaliable in IORT table.

  @param[in] CurrentTable  IORT table to fixup
  @param[in] InputId       Input Id to ITS block
  @param[in] OutputId      Output Id from ITS block

  @retval    IORT_SUCCESS  IORT table fixed up successfully
**/
UINT32
SetItsIdMapping (
    VOID    *CurrentTable,
    UINT32  InputId,
    UINT32  OutputId
    )
{
  NXP_EFI_ACPI_6_0_IO_REMAPPING_TABLE      *Iort = NULL;
  NXP_EFI_ACPI_6_0_IO_REMAPPING_SMMU_NODE  *SmmuNode;
  EFI_ACPI_6_0_IO_REMAPPING_ID_TABLE       *IdMapping;

  Iort = (NXP_EFI_ACPI_6_0_IO_REMAPPING_TABLE *)CurrentTable;

  // find Smmu Node
  SmmuNode = &(Iort->SmmuNode);

  IdMapping = &(SmmuNode->SmmuID[SmmuNode->SmmuNode.Node.NumIdMappings]);

  IdMapping->InputBase = InputId;
  IdMapping->NumIds = 0;
  IdMapping->OutputBase = OutputId;

  SmmuNode->SmmuNode.Node.NumIdMappings += 1;

  return IORT_SUCCESS;
}

/**
  ID mappings represent the formula by which an ID from a source is converted to
  an ID in a destination.

  This function doesn't check the table for memory footprint. i.e. it's assumed
  that the memory for ID mapping is avaliable in IORT table.

  @param[in] CurrentTable  IORT table to fixup
  @param[in] SegmentNumber SegmentNumber corresponding to PCIE controller. Used
                           to find PCIE controller structure in Iort->PciRcNode
                           array.
  @param[in] InputId       The device as identified by BusDeviceFunc Triplet
  @param[in] OutputId      StreamId assigned to the Pcie device.

  @retval IORT_SUCCESS     IORT table fixed up successfully
  @retval IORT_NOT_FOUND   Controller node not found in IORT table
**/
UINT32
SetIommuIdMapping (
    VOID    *CurrentTable,
    UINTN   SegmentNumber,
    UINT32  InputId,
    UINT32  OutputId
    )
{
  NXP_EFI_ACPI_6_0_IO_REMAPPING_TABLE    *Iort = NULL;
  NXP_EFI_ACPI_6_0_IO_REMAPPING_RC_NODE  *PciRcNode;
  EFI_ACPI_6_0_IO_REMAPPING_ID_TABLE     *IdMapping;
  UINTN                                  Index;

  Iort = (NXP_EFI_ACPI_6_0_IO_REMAPPING_TABLE *)CurrentTable;

  // find Pcie Rc Node
  Index = 0;
  PciRcNode = Iort->PciRcNode;
  while (Index < ARRAY_SIZE(Iort->PciRcNode)) {
    if (PciRcNode->PciRcNode.PciSegmentNumber == SegmentNumber) {
      break;
    }
    PciRcNode = &(Iort->PciRcNode[++Index]);
  }

  if (Index == ARRAY_SIZE(Iort->PciRcNode)) {
    DEBUG ((DEBUG_ERROR, "Pcie node with Segment number %d not found in IORT table\n", SegmentNumber));
    return IORT_NODE_NOT_FOUND;
  }

  IdMapping = &(PciRcNode->PciRcIdMapping[PciRcNode->PciRcNode.Node.NumIdMappings]);

  IdMapping->InputBase = InputId;
  IdMapping->NumIds = 0;
  IdMapping->OutputBase = FixedPcdGet16 (PcdPcieTbuMask) | OutputId;
  IdMapping->OutputReference = OFFSET_OF (NXP_EFI_ACPI_6_0_IO_REMAPPING_TABLE, SmmuNode);

  PciRcNode->PciRcNode.Node.NumIdMappings += 1;

  return IORT_SUCCESS;
}
