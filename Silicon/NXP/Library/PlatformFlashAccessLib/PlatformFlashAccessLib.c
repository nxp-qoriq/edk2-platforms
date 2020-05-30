/** @file
  Platform flash device access library for NXP Platforms.

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/FirmwareVolumeBlock.h>

/**
  Gets firmware volume block handle by given address.

  This function gets firmware volume block handle whose
  address range contains the parameter Address.

  @param[in]  Address    Address which should be contained
                         by returned FVB handle.
  @param[out] FvbHandle  Pointer to FVB handle for output.

  @retval EFI_SUCCESS    FVB handle successfully returned.
  @retval EFI_NOT_FOUND  Failed to find FVB handle by address.

**/
STATIC
EFI_STATUS
GetFvbByAddress (
  IN  EFI_PHYSICAL_ADDRESS                Address,
  OUT EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  **OutFvb,
  OUT EFI_PHYSICAL_ADDRESS                *FvbBaseAddress
  )
{
  EFI_STATUS                          Status;
  EFI_HANDLE                          *HandleBuffer;
  UINTN                               HandleCount;
  UINTN                               Index;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb;
  EFI_FIRMWARE_VOLUME_HEADER          *FwVolHeader;
  EFI_FVB_ATTRIBUTES_2                Attributes;

  //
  // Locate all handles with Firmware Volume Block protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolumeBlockProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  //
  // Traverse all the handles, searching for the one containing parameter Address
  //
  for (Index = 0; Index < HandleCount; Index += 1) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolumeBlockProtocolGuid,
                    (VOID **) &Fvb
                    );
    if (EFI_ERROR (Status)) {
      Status = EFI_NOT_FOUND;
      break;
    }
    //
    // Checks if the address range of this handle contains parameter Address
    //
    Status = Fvb->GetPhysicalAddress (Fvb, FvbBaseAddress);
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Check if this FVB is writable: DXE core produces FVB protocols for
    // firmware volumes as well.
    //
    Status = Fvb->GetAttributes (Fvb, &Attributes);
    if (EFI_ERROR (Status) || !(Attributes & EFI_FVB2_WRITE_STATUS)) {
      DEBUG ((DEBUG_INFO,
        "%a: ignoring read-only FVB protocol implementation\n",
        __FUNCTION__));
      continue;
    }

    FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *) ((UINTN) FixedPcdGet64(PcdFlashNvStorageVariableBase64));
    if ((Address >= *FvbBaseAddress) && (Address <= (*FvbBaseAddress + FwVolHeader->FvLength))) {
      *OutFvb  = Fvb;
      Status   = EFI_SUCCESS;
      break;
    }

    Status = EFI_NOT_FOUND;
  }

  FreePool (HandleBuffer);
  return Status;
}

/**
  Perform flash write operation.

  @param[in] FirmwareType      The type of firmware.
  @param[in] FlashAddress      The address of flash device to be accessed.
  @param[in] FlashAddressType  The type of flash device address.
  @param[in] Buffer            The pointer to the data buffer.
  @param[in] Length            The length of data buffer in bytes.
  @param[in] Progress          A function used report the progress of the
                               firmware update.  This is an optional parameter
                               that may be NULL.
  @param[in] StartPercentage   The start completion percentage value that may
                               be used to report progress during the flash
                               write operation.
  @param[in] EndPercentage     The end completion percentage value that may
                               be used to report progress during the flash
                               write operation.

  @retval EFI_SUCCESS           The operation returns successfully.
  @retval EFI_WRITE_PROTECTED   The flash device is read only.
  @retval EFI_UNSUPPORTED       The flash device access is unsupported.
  @retval EFI_INVALID_PARAMETER The input parameter is not valid.
**/
EFI_STATUS
EFIAPI
PerformFlashWriteWithProgress (
  IN PLATFORM_FIRMWARE_TYPE                         FirmwareType,
  IN EFI_PHYSICAL_ADDRESS                           FlashAddress,
  IN FLASH_ADDRESS_TYPE                             FlashAddressType,
  IN VOID                                           *Buffer,
  IN UINTN                                          Length,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb;
  EFI_STATUS                          Status;
  UINTN                               BlockSize;
  UINTN                               NumberOfBlocks;
  EFI_LBA                             Lba;
  EFI_PHYSICAL_ADDRESS                FvbBaseAddress;
  UINTN                               NumBytes;
  UINTN                               Offset;
  UINTN                               StartOffset;
  UINT8                               *PtrToBuffer;

  if (FlashAddressType != FlashAddressTypeAbsoluteAddress) {
    DEBUG ((DEBUG_ERROR, "%a: only FlashAddressTypeAbsoluteAddress supported\n",
      __FUNCTION__));

    return EFI_INVALID_PARAMETER;
  }

  if (FirmwareType != PlatformFirmwareTypeSystemFirmware) {
    DEBUG ((DEBUG_ERROR,
      "%a: only PlatformFirmwareTypeSystemFirmware supported\n",
      __FUNCTION__));

    return EFI_INVALID_PARAMETER;
  }

  //
  // Locate the gEfiFirmwareVolumeBlockProtocolGuid implementation
  // that covers the system firmware
  //
  Fvb = NULL;
  Status = GetFvbByAddress (FlashAddress, &Fvb, &FvbBaseAddress);
  if (EFI_ERROR (Status) || Fvb == NULL) {
    DEBUG ((DEBUG_ERROR,
      "%a: failed to locate FVB handle for address 0x%llx - %r\n",
      __FUNCTION__, FlashAddress, Status));
    return Status;
  }

  Status = Fvb->GetBlockSize(Fvb, 0, &BlockSize, &NumberOfBlocks);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to get FVB blocksize - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  if ((Length % BlockSize) != 0) {
    DEBUG ((DEBUG_ERROR,
      "%a: Length 0x%lx is not a multiple of the blocksize 0x%lx\n",
      __FUNCTION__, Length, BlockSize));
    return EFI_INVALID_PARAMETER;
  }

  Lba = (FlashAddress - FvbBaseAddress) / BlockSize;
  if (Lba > NumberOfBlocks - 1) {
    DEBUG ((DEBUG_ERROR,
      "%a: flash device with non-uniform blocksize not supported\n",
      __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  PtrToBuffer = (UINT8*) Buffer;
  StartOffset = FlashAddress - FvbBaseAddress;
  Offset = StartOffset % BlockSize;
  NumBytes = MIN (Length, (BlockSize - Offset));
  while (Length > 0) {
    //
    // Write the new data
    //
    DEBUG ((DEBUG_INFO, "%a: writing 0x%llx bytes at LBA 0x%lx Offset 0x%lx\n",
      __FUNCTION__, NumBytes, Lba, Offset));

    Status = Fvb->Write (Fvb, Lba, Offset, &NumBytes, PtrToBuffer);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,
        "%a: write of LBA 0x%lx failed - %r (NumBytes == 0x%lx)\n",
        __FUNCTION__, Lba, Status, NumBytes));
    }

    Length -= NumBytes;
    PtrToBuffer += NumBytes;
    Lba = (StartOffset + (PtrToBuffer - (UINT8*) Buffer)) / BlockSize;
    Offset = (StartOffset + (PtrToBuffer - (UINT8*) Buffer)) % BlockSize;
    NumBytes = MIN (Length, (BlockSize - Offset));
  }

  return EFI_SUCCESS;
}
