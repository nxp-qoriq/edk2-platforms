/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Chassis.h>
#include <Library/AcpiPlatformLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/NxpMcfgTable.h>

#define AML_BUFFER_OP                0x11
#define AML_RESOURCE_BUS             0x2

/**
  Fixup the DSDT Table based on running SOC's properties.

  @param[in]  Table   The DSDR Table to fixup.

  @return EFI_SUCCESS       Successfully fixed up the ACPI Table
**/
STATIC
EFI_STATUS
UpdateDsdtPcie (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  EFI_ACPI_COMMON_HEADER                 *TableHeader;
  EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR *WordBusRsc;
  UINT8                                  *DataPtr;

  TableHeader = (EFI_ACPI_COMMON_HEADER *)Table;

  if (PcdGetBool (PcdPciHideRootPort)) {
    for (DataPtr = (UINT8 *)(TableHeader + 1);
         DataPtr < (UINT8 *) ((UINT8 *) TableHeader + TableHeader->Length - 4);
         DataPtr++) {
      if (CompareMem(DataPtr, "RBUF", 4) == 0) {
        DataPtr += 4; // Skip RBUF
        if (*DataPtr == AML_BUFFER_OP) {
          DataPtr += 4; // Skip Resource Type, Length etc.
          WordBusRsc = (EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR *)DataPtr;
          if (WordBusRsc->GenFlag == AML_RESOURCE_BUS) {
            WordBusRsc->AddrRangeMin = SwapBytes16 (0x1);
            WordBusRsc->AddrLen = SwapBytes16 (0xff);
            DataPtr = DataPtr + sizeof (EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR);
            *(DataPtr) = 0x0;
          }
        }
      }
    }
  }

  return  EFI_SUCCESS;
}

/**
  Fixup the Mcfg Table based on running SOC's properties.

  @param[in]  Table   The Mcfg Table to fixup.

  @return EFI_SUCCESS       Successfully fixed up the ACPI Table
  @return EFI_DEVICE_ERROR  Could not fix the ACPI Table
**/
STATIC
EFI_STATUS
UpdateMcfg (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  UINT32 Svr;
  NXP_EFI_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_TABLE *Mcfg;
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE *McfgNode;
  UINTN Index;

  if (PcdGetBool (PcdPciHideRootPort)) {
    Mcfg = (NXP_EFI_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_TABLE *)Table;
    Index = 0;
    while (Index < ARRAY_SIZE(Mcfg->Config_Structure)) {
      McfgNode = &(Mcfg->Config_Structure[Index++]);
      McfgNode->StartBusNumber = 0x1;
    }
    Table->OemRevision = 0xff;
  } else {
    Svr = PcdGet32 (PcdSocSvr);
    if (SVR_MAJOR (Svr) == 2) {
      Table->OemRevision = 0x01;
    }
  }

  return  EFI_SUCCESS;
}

/**
  Fixup the Acpi Table based on running SOC's properties.

  @param[in]  Table   The Acpi Table to fixup.

  @return EFI_SUCCESS       Successfully fixed up the ACPI Table
  @return EFI_DEVICE_ERROR  Could not fix the ACPI Table
**/
EFI_STATUS
AcpiPlatformFixup (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER      *TableHeader
  )
{
  EFI_STATUS   Status;

  switch (TableHeader->Signature) {
    case EFI_ACPI_6_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE:
      Status = UpdateMcfg (TableHeader);
      break;

    case EFI_ACPI_6_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
      Status = UpdateDsdtPcie (TableHeader);
      break;

    default:
      Status = EFI_SUCCESS;
  }

  return Status;
}
