/** @file
  System Firmware descriptor producer.

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PiPei.h>
#include <Guid/EdkiiSystemFmpCapsule.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DxeServicesLib.h>
#include <Protocol/FirmwareManagement.h>

/**
  Entrypoint for SystemFirmwareDescriptor.

  @param[in]  ImageHandle  The firmware allocated handle for the EFI image.
  @param[in]  SystemTable  A pointer to the EFI System Table..

  @retval EFI_SUCCESS      FMP Descriptor installed.
**/
EFI_STATUS
EFIAPI
SystemFirmwareDescriptorEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                              Status;
  EDKII_SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR  *Descriptor;
  UINTN                                   Size;
  UINTN                                   Index;

  //
  // Search RAW section.
  //
  Index = 0;
  while (TRUE) {
    Status = GetSectionFromFfs(EFI_SECTION_RAW, Index, (VOID **)&Descriptor, &Size);
    if (EFI_ERROR(Status)) {
      // Should not happen, must something wrong in FDF.
      ASSERT(FALSE);
      return EFI_NOT_FOUND;
    }
    if (Descriptor->Signature == EDKII_SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR_SIGNATURE) {
      break;
    }
    Index++;
  }

  DEBUG((DEBUG_INFO, "EDKII_SYSTEM_FIRMWARE_IMAGE_DESCRIPTOR size - 0x%x\n",
    Descriptor->Length));

  Size = Descriptor->Length;
  PcdSetPtrS (PcdEdkiiSystemFirmwareImageDescriptor, &Size, Descriptor);

  return EFI_SUCCESS;
}
