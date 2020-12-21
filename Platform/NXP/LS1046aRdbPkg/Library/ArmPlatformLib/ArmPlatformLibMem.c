/** @file
*
*  Copyright 2019-2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Soc.h>

#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS          (7 + FixedPcdGet32 (PcdNumPciController))

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR describing a Physical-to-
                                    Virtual Memory mapping. This array must be ended by a zero-filled
                                    entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  UINTN                            Index;
  UINT32                           I;
  ARM_MEMORY_REGION_DESCRIPTOR     *VirtualMemoryTable;

  Index = 0;

  ASSERT (VirtualMemoryMap != NULL);

  VirtualMemoryTable = AllocatePool (sizeof (ARM_MEMORY_REGION_DESCRIPTOR) *
                                     MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

  if (VirtualMemoryTable == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Error: Failed AllocatePool()\n", __FUNCTION__));
    return;
  }

  VirtualMemoryTable[Index].PhysicalBase = LS1046A_DRAM0_PHYS_ADDRESS;
  VirtualMemoryTable[Index].VirtualBase  = LS1046A_DRAM0_PHYS_ADDRESS;
  VirtualMemoryTable[Index].Length       = LS1046A_DRAM0_SIZE;
  VirtualMemoryTable[Index++].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  VirtualMemoryTable[Index].PhysicalBase = LS1046A_DRAM1_PHYS_ADDRESS;
  VirtualMemoryTable[Index].VirtualBase  = LS1046A_DRAM1_PHYS_ADDRESS;
  VirtualMemoryTable[Index].Length       = LS1046A_DRAM1_SIZE;
  VirtualMemoryTable[Index++].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  // CCSR Space
  VirtualMemoryTable[Index].PhysicalBase = LS1046A_CCSR_PHYS_ADDRESS;
  VirtualMemoryTable[Index].VirtualBase  = LS1046A_CCSR_PHYS_ADDRESS;
  VirtualMemoryTable[Index].Length       = LS1046A_CCSR_SIZE;
  VirtualMemoryTable[Index++].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // QSPI
  VirtualMemoryTable[Index].PhysicalBase = LS1046A_QSPI0_PHYS_ADDRESS;
  VirtualMemoryTable[Index].VirtualBase  = LS1046A_QSPI0_PHYS_ADDRESS;
  VirtualMemoryTable[Index].Length       = LS1046A_QSPI0_SIZE;
  VirtualMemoryTable[Index++].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

 // QMAN SWP
  VirtualMemoryTable[Index].PhysicalBase = LS1046A_QMAN_SW_PORTAL_PHYS_ADDRESS;
  VirtualMemoryTable[Index].VirtualBase  = LS1046A_QMAN_SW_PORTAL_PHYS_ADDRESS;
  VirtualMemoryTable[Index].Length       = LS1046A_QMAN_SW_PORTAL_SIZE;
  VirtualMemoryTable[Index++].Attributes   = 0 ;//ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;

  // BMAN SWP
  VirtualMemoryTable[Index].PhysicalBase = LS1046A_BMAN_SW_PORTAL_PHYS_ADDRESS;
  VirtualMemoryTable[Index].VirtualBase  = LS1046A_BMAN_SW_PORTAL_PHYS_ADDRESS;
  VirtualMemoryTable[Index].Length       = LS1046A_BMAN_SW_PORTAL_SIZE;
  VirtualMemoryTable[Index++].Attributes   = 0; //ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;

  // PCIe Space
  for (I = 0; I < FixedPcdGet32 (PcdNumPciController); I++) {
    VirtualMemoryTable[Index].PhysicalBase = LS1046A_PCI1_PHYS_ADDRESS + I * LS1046A_PCI_SIZE;
    VirtualMemoryTable[Index].VirtualBase  = LS1046A_PCI1_PHYS_ADDRESS + I * LS1046A_PCI_SIZE;
    VirtualMemoryTable[Index].Length       = LS1046A_PCI_SIZE;
    VirtualMemoryTable[Index++].Attributes   = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  }

  // End of Table
  ZeroMem (&VirtualMemoryTable[Index], sizeof (ARM_MEMORY_REGION_DESCRIPTOR));

  ASSERT (Index < MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

  *VirtualMemoryMap = VirtualMemoryTable;
}
