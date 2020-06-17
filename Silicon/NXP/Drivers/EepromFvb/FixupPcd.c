/** @file

  Update the patched PCDs to their correct value

  Copyright (c) 2020, Linaro Ltd. All rights reserved.
  Copyright 2020 NXP.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/**
 * Patch the relevant PCDs of the RPMB driver with the correct address of the
 * allocated memory
 *
**/
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MmServicesTableLib.h>
#include <Library/PcdLib.h>

#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/SmmFirmwareVolumeBlock.h>

#include "EepromFvb.h"

EFI_STATUS
EFIAPI
FixPcdMemory (
  VOID
  )
{
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *FvbProtocol;
  MEM_INSTANCE                        *Instance;
  EFI_STATUS                          Status;
  //
  // Locate SmmFirmwareVolumeBlockProtocol
  //

  Status = gMmst->MmLocateProtocol (
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    NULL,
                    (VOID **) &FvbProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  Instance = INSTANCE_FROM_FVB_THIS(FvbProtocol);
  // Set the updated PCDs
  PatchPcdSet32 (PcdFlashNvStorageVariableBase, Instance->MemBaseAddress);
  PatchPcdSet32 (PcdFlashNvStorageFtwWorkingBase, Instance->MemBaseAddress +
    PcdGet32 (PcdFlashNvStorageVariableSize));
  PatchPcdSet32 (PcdFlashNvStorageFtwSpareBase, Instance->MemBaseAddress +
    PcdGet32 (PcdFlashNvStorageVariableSize) +
    PcdGet32 (PcdFlashNvStorageFtwWorkingSize));

  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageVariableBase: 0x%lx\n",
    __FUNCTION__, PcdGet32 (PcdFlashNvStorageVariableBase)));
  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageFtwWorkingBase: 0x%lx\n",
    __FUNCTION__, PcdGet32 (PcdFlashNvStorageFtwWorkingBase)));
  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageFtwSpareBase: 0x%lx\n",
    __FUNCTION__, PcdGet32 (PcdFlashNvStorageFtwSpareBase)));

  return Status;
}
