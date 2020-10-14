/*++ @file  EepromFvb.c

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

 --*/

#include <PiDxe.h>

#include <Guid/SystemNvDataGuid.h>
#include <Guid/VariableFormat.h>
#include <Library/BaseLib.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/MmServicesTableLib.h>

#include <Protocol/I2cMaster.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/SmmFirmwareVolumeBlock.h>

#include "EepromFvb.h"

STATIC EFI_I2C_MASTER_PROTOCOL    *mI2cMaster;

STATIC MEM_INSTANCE  mInstance;

/* FIXME: Since EEPROM is not memory-mapped we will keep this as 0 */
/**
 The GetAttributes() function retrieves the attributes and
 current settings of the block.

 @param This         Indicates the EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Attributes   Pointer to EFI_FVB_ATTRIBUTES_2 in which the attributes and
                     current settings are returned.
                     Type EFI_FVB_ATTRIBUTES_2 is defined in EFI_FIRMWARE_VOLUME_HEADER.

 @retval EFI_SUCCESS The firmware volume attributes were returned.

 **/
STATIC
EFI_STATUS
FvbGetAttributes(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL    *This,
  OUT       EFI_FVB_ATTRIBUTES_2                   *Attributes
  )
{
  *Attributes = EFI_FVB2_READ_ENABLED_CAP   | // Reads may be enabled
                EFI_FVB2_READ_STATUS        | // Reads are currently enabled
                EFI_FVB2_WRITE_STATUS       | // Writes are currently enabled
                EFI_FVB2_WRITE_ENABLED_CAP  | // Writes may be enabled
                EFI_FVB2_MEMORY_MAPPED      | // It is memory mapped
                EFI_FVB2_ERASE_POLARITY;      // After erasure all bits take this value (i.e. '1')

  return EFI_SUCCESS;
}

/**
 The SetAttributes() function sets configurable firmware volume attributes
 and returns the new settings of the firmware volume.


 @param This                     Indicates the EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Attributes               On input, Attributes is a pointer to EFI_FVB_ATTRIBUTES_2
                                 that contains the desired firmware volume settings.
                                 On successful return, it contains the new settings of
                                 the firmware volume.
                                 Type EFI_FVB_ATTRIBUTES_2 is defined in EFI_FIRMWARE_VOLUME_HEADER.

 @retval EFI_SUCCESS             The firmware volume attributes were returned.

 @retval EFI_INVALID_PARAMETER   The attributes requested are in conflict with the capabilities
                                 as declared in the firmware volume header.

 **/
STATIC
EFI_STATUS
FvbSetAttributes(
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL  *This,
  IN OUT    EFI_FVB_ATTRIBUTES_2                 *Attributes
  )
{
  return EFI_SUCCESS;
}

/**
 The GetPhysicalAddress() function retrieves the base address of
 a memory-mapped firmware volume. This function should be called
 only for memory-mapped firmware volumes.

 @param This               Indicates the EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Address            Pointer to a caller-allocated
                           EFI_PHYSICAL_ADDRESS that, on successful
                           return from GetPhysicalAddress(), contains the
                           base address of the firmware volume.

 @retval EFI_SUCCESS       The firmware volume base address was returned.

 @retval EFI_NOT_SUPPORTED The firmware volume is not memory mapped.

 **/
STATIC
EFI_STATUS
FvbGetPhysicalAddress (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL  *This,
  OUT       EFI_PHYSICAL_ADDRESS                 *Address
  )
{
  MEM_INSTANCE *Instance;

  Instance = INSTANCE_FROM_FVB_THIS(This);
  *Address = Instance->MemBaseAddress;

  return EFI_SUCCESS;
}

/**
  Write data to I2C EEPROM.

  @param[in]  SlaveAddress           Logical Address of EEPROM block.
  @param[in]  RegAddress             Register Address in Slave's memory map
  @param[in]  RegAddressWidthInBytes Number of bytes in RegAddress to send to
                                     I2c Slave for simple reads without any
                                     register, make this value = 0
                                     (RegAddress is don't care in that case)
  @param[out] RegValue               Value to be read from I2c slave's regiser
  @param[in]  RegValueNumBytes       Number of bytes to read from I2c slave
                                     register

  @return  EFI_SUCCESS       successfuly read the registers
  @return  EFI_DEVICE_ERROR  There was an error while transferring data through
                             I2c bus
  @return  EFI_NO_RESPONSE   There was no Ack from i2c device
  @return  EFI_TIMEOUT       I2c Bus is busy
  @return  EFI_NOT_READY     I2c Bus Arbitration lost
**/
EFI_STATUS
EFIAPI
EepromWrite (
  IN  UINT32  SlaveAddress,
  IN  UINT64  RegAddress,
  IN  UINT8   RegAddressWidthInBytes,
  IN  UINT8   *RegValue,
  IN  UINT32  RegValueNumBytes
  )
{
  EEPROM_SET_I2C_REQUEST         Op;
  UINT8                          *PtrAddress;
  EFI_STATUS                     Status;
  UINT64                         StartTick;

  PtrAddress = mInstance.ShadowBuffer;

  if ((RegAddressWidthInBytes + RegValueNumBytes) > \
       (BLOCK_SIZE + EEPROM_ADDR_WIDTH_2BYTES)) {
    return EFI_BAD_BUFFER_SIZE;
  }

  Op.OperationCount              = 1;
  Op.Operation[0].Flags          = 0;
  Op.Operation[0].LengthInBytes  = RegAddressWidthInBytes + RegValueNumBytes;
  Op.Operation[0].Buffer         = mInstance.ShadowBuffer;

  // Big Endian format
  while (RegAddressWidthInBytes--) {
    *PtrAddress++ = RegAddress >> (8 * RegAddressWidthInBytes);
  }

  CopyMem (
    PtrAddress,
    (VOID *)RegValue,
    RegValueNumBytes
    );

  Status = mI2cMaster->StartRequest (mI2cMaster, SlaveAddress, &Op, NULL, NULL);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Poll for write Complete. wait for max 10ms as per atmel at24cm02 datasheet
  StartTick = GetPerformanceCounter();

  Op.OperationCount              = 1;
  Op.Operation[0].Flags          = 0;
  Op.Operation[0].LengthInBytes  = 0;
  Op.Operation[0].Buffer         = NULL;
  do {
    Status = mI2cMaster->StartRequest (mI2cMaster, SlaveAddress, &Op, NULL, NULL);
    if (Status == EFI_NO_RESPONSE) {
      continue;
    }
    break;
  } while (GetTimeInNanoSecond (GetPerformanceCounter() - StartTick) < 10000000);

  return Status;
}


/**
  Read data from I2C EEPROM.

  @param[in]  SlaveAddress           Logical Address of EEPROM block.
  @param[in]  RegAddress             Register Address in Slave's memory map
  @param[in]  RegAddressWidthInBytes Number of bytes in RegAddress to send to
                                     I2c Slave for simple reads without any
                                     register, make this value = 0
                                     (RegAddress is don't care in that case)
  @param[out] RegValue               Value to be read from I2c slave's regiser
  @param[in]  RegValueNumBytes       Number of bytes to read from I2c slave
                                     register

  @return  EFI_SUCCESS       successfuly read the registers
  @return  EFI_DEVICE_ERROR  There was an error while transferring data through
                             I2c bus
  @return  EFI_NO_RESPONSE   There was no Ack from i2c device
  @return  EFI_TIMEOUT       I2c Bus is busy
  @return  EFI_NOT_READY     I2c Bus Arbitration lost
**/
EFI_STATUS
EFIAPI
EepromRead (
  IN  UINT32  SlaveAddress,
  IN  UINT64  RegAddress,
  IN  UINT8   RegAddressWidthInBytes,
  IN OUT UINT8   *RegValue,
  IN  UINT32  RegValueNumBytes
  )
{
  EEPROM_GET_I2C_REQUEST  Op;
  UINT8                   *PtrLast, *PtrFirst;
  UINT8                   Temp;

  Op.OperationCount = 2;
  PtrLast = (UINT8 *)&RegAddress + RegAddressWidthInBytes - 1;
  PtrFirst = (UINT8 *)&RegAddress;

  Op.SetAddressOp.Flags = 0;
  Op.SetAddressOp.LengthInBytes = RegAddressWidthInBytes;
  Op.SetAddressOp.Buffer = PtrFirst;

  // Big Endian format
  while (--RegAddressWidthInBytes) {
    Temp = *PtrLast;
    *PtrLast-- = *PtrFirst;
    *PtrFirst++ = Temp;
  }

  Op.GetDataOp.Flags = I2C_FLAG_READ;
  Op.GetDataOp.LengthInBytes = RegValueNumBytes;
  Op.GetDataOp.Buffer = RegValue;

  return mI2cMaster->StartRequest (mI2cMaster, SlaveAddress,
                       (VOID *)&Op, NULL, NULL);
}

STATIC
UINT32
EFIAPI
GetEepromSlaveAddress (
  IN        EFI_LBA     Lba
  )
{
  if (Lba >= 0 && Lba < 256) {
    return EEPROM_VARIABLE_STORE_ADDR;
  } else if (Lba >= 256 && Lba < 512) {
    return EEPROM_FTW_WORKING_SPACE_ADDR;
  } else if (Lba >=512 && Lba < 768 ) {
    return EEPROM_FTW_SPARE_SPACE_ADDR;
  } else {
    return EEPROM_VARIABLE_STORE_MAX;
  }
}

/**
 Reads the specified number of bytes into a buffer from the specified block.

 The Read() function reads the requested number of bytes from the
 requested block and stores them in the provided buffer.
 Implementations should be mindful that the firmware volume
 might be in the ReadDisabled state. If it is in this state,
 the Read() function must return the status code
 EFI_ACCESS_DENIED without modifying the contents of the
 buffer. The Read() function must also prevent spanning block
 boundaries. If a read is requested that would span a block
 boundary, the read must read up to the boundary but not
 beyond. The output parameter NumBytes must be set to correctly
 indicate the number of bytes actually read. The caller must be
 aware that a read may be partially completed.

 @param This                 Indicates the EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Lba                  The starting logical block index from which to read.

 @param Offset               Offset into the block at which to begin reading.

 @param NumBytes             Pointer to a UINTN.
                             At entry, *NumBytes contains the total size of the buffer.
                             At exit, *NumBytes contains the total number of bytes read.

 @param Buffer               Pointer to a caller-allocated buffer that will be used
                             to hold the data that is read.

 @retval EFI_SUCCESS         The firmware volume was read successfully,  and contents are
                             in Buffer.

 @retval EFI_BAD_BUFFER_SIZE Read attempted across an LBA boundary.
                             On output, NumBytes contains the total number of bytes
                             returned in Buffer.

 @retval EFI_ACCESS_DENIED   The firmware volume is in the ReadDisabled state.

 @retval EFI_DEVICE_ERROR    The block device is not functioning correctly and could not be read.

 **/
EFI_STATUS
EFIAPI
FvbRead (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL   *This,
  IN        EFI_LBA                               Lba,
  IN        UINTN                                 Offset,
  IN OUT    UINTN                                 *NumBytes,
  IN OUT    UINT8                                 *Buffer
  )
{
  UINTN                BlockSize;
  EFI_STATUS           Status = EFI_SUCCESS;
  EFI_STATUS           TmpStatus;
  VOID                 *Base;
  UINT64               EepromAddr = 0;
  UINT32               EepromSlaveAddr = 0;
  MEM_INSTANCE         *Instance;

  TmpStatus = EFI_SUCCESS;


  Instance = INSTANCE_FROM_FVB_THIS(This);
  if (Instance->Initialized == FALSE) {
    Status = Instance->Initialize (Instance);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO, "Initialization failed\n"));
      return Status;
    }
  }

  // Cache the block size to avoid de-referencing pointers all the time
  BlockSize = Instance->BlockSize;

  // The read must not span block boundaries.
  // We need to check each variable individually because adding two large values together overflows.
  if (Offset >= BlockSize) {
    *NumBytes = 0;
    return EFI_BAD_BUFFER_SIZE;
  }

  // We must have some bytes to read
  if (*NumBytes == 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  if ((Offset + *NumBytes) > BlockSize) {
    *NumBytes = BlockSize - Offset;
    TmpStatus = EFI_BAD_BUFFER_SIZE;
  }

//  EepromAddr = ((Lba % 16) * BlockSize) + Offset;
  EepromAddr = ((Lba % BLOCKS_IN_ONE_PARTITION) * BlockSize) + Offset;
  EepromSlaveAddr = GetEepromSlaveAddress(Lba);
  if (EepromSlaveAddr == EEPROM_VARIABLE_STORE_MAX) {
    Status = EFI_DEVICE_ERROR;
    goto exit;
  }

#if 0
  DEBUG ((
    DEBUG_INFO,
    "FvbRead(Parameters: Lba=%ld, Offset=0x%x, *NumBytes=0x%x, Buffer @ 0x%08x), BlockSize=0x%x EepromAddr = %lx, EepromSlaveAddr = %x\n",
    Lba, Offset, *NumBytes, Buffer, BlockSize, EepromAddr, EepromSlaveAddr
    ));
#endif

  Status = EepromRead(EepromSlaveAddr, EepromAddr,
                        EEPROM_ADDR_WIDTH_2BYTES, Buffer, *NumBytes);
  if (!EFI_ERROR(Status)) {
    Base = (VOID *)Instance->MemBaseAddress + Lba * BlockSize + Offset;
    // Update the memory copy
    CopyMem (Base, Buffer, *NumBytes);
    return TmpStatus;
  } else {
    DEBUG ((DEBUG_ERROR, "Eeprom_Read returned %r\n", Status));
    Status = EFI_DEVICE_ERROR;
  }
exit:
  return Status;
}

/**
 Writes the specified number of bytes from the input buffer to the block.

 The Write() function writes the specified number of bytes from
 the provided buffer to the specified block and offset. If the
 firmware volume is sticky write, the caller must ensure that
 all the bits of the specified range to write are in the
 EFI_FVB_ERASE_POLARITY state before calling the Write()
 function, or else the result will be unpredictable. This
 unpredictability arises because, for a sticky-write firmware
 volume, a write may negate a bit in the EFI_FVB_ERASE_POLARITY
 state but cannot flip it back again.  Before calling the
 Write() function,  it is recommended for the caller to first call
 the EraseBlocks() function to erase the specified block to
 write. A block erase cycle will transition bits from the
 (NOT)EFI_FVB_ERASE_POLARITY state back to the
 EFI_FVB_ERASE_POLARITY state. Implementations should be
 mindful that the firmware volume might be in the WriteDisabled
 state. If it is in this state, the Write() function must
 return the status code EFI_ACCESS_DENIED without modifying the
 contents of the firmware volume. The Write() function must
 also prevent spanning block boundaries. If a write is
 requested that spans a block boundary, the write must store up
 to the boundary but not beyond. The output parameter NumBytes
 must be set to correctly indicate the number of bytes actually
 written. The caller must be aware that a write may be
 partially completed. All writes, partial or otherwise, must be
 fully flushed to the hardware before the Write() service
 returns.

 @param This                 Indicates the EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Lba                  The starting logical block index to write to.

 @param Offset               Offset into the block at which to begin writing.

 @param NumBytes             The pointer to a UINTN.
                             At entry, *NumBytes contains the total size of the buffer.
                             At exit, *NumBytes contains the total number of bytes actually written.

 @param Buffer               The pointer to a caller-allocated buffer that contains the source for the write.

 @retval EFI_SUCCESS         The firmware volume was written successfully.

 @retval EFI_BAD_BUFFER_SIZE The write was attempted across an LBA boundary.
                             On output, NumBytes contains the total number of bytes
                             actually written.

 @retval EFI_ACCESS_DENIED   The firmware volume is in the WriteDisabled state.

 @retval EFI_DEVICE_ERROR    The block device is malfunctioning and could not be written.


 **/
EFI_STATUS
EFIAPI
FvbWrite (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL   *This,
  IN        EFI_LBA                               Lba,
  IN        UINTN                                 Offset,
  IN OUT    UINTN                                 *NumBytes,
  IN        UINT8                                 *Buffer
  )
{
  UINTN                       BlockSize;

  EFI_STATUS                  Status = EFI_SUCCESS;
  EFI_STATUS                  TmpStatus;
  MEM_INSTANCE                *Instance;
  VOID*                       Base;
  UINT64                      EepromAddr = 0;
  UINT32                      EepromSlaveAddr = 0;

  TmpStatus = EFI_SUCCESS;
  Instance = INSTANCE_FROM_FVB_THIS(This);
  if (Instance->Initialized == FALSE) {
    Instance->Initialize (Instance);
  }

  // Cache the block size to avoid de-referencing pointers all the time
  BlockSize = Instance->BlockSize;

  // The read must not span block boundaries.
  // We need to check each variable individually because adding two large values together overflows.
  if (Offset >= BlockSize) {
    *NumBytes = 0;
    return EFI_BAD_BUFFER_SIZE;
  }

  // We must have some bytes to write
  if (*NumBytes == 0) {
    return EFI_BAD_BUFFER_SIZE;
  }

  if ((Offset + *NumBytes) > BlockSize) {
    *NumBytes = BlockSize - Offset;
    TmpStatus = EFI_BAD_BUFFER_SIZE;
  }

  EepromAddr = ((Lba % BLOCKS_IN_ONE_PARTITION) * BlockSize) + Offset;
  EepromSlaveAddr = GetEepromSlaveAddress(Lba);
  if (EepromSlaveAddr == EEPROM_VARIABLE_STORE_MAX) {
    Status = EFI_DEVICE_ERROR;
    goto exit;
  }

  Status = EepromWrite(EepromSlaveAddr, EepromAddr,
                         EEPROM_ADDR_WIDTH_2BYTES, Buffer, *NumBytes);
  if (!EFI_ERROR(Status)) {
    Base = (VOID *)Instance->MemBaseAddress + (Lba * BlockSize) + Offset;
    // Update the memory copy
    CopyMem (Base, Buffer, *NumBytes);

    return TmpStatus;
  } else {
    DEBUG ((DEBUG_ERROR, "Eeprom_Write returned %r\n", Status));
    Status = EFI_DEVICE_ERROR;
  }
exit:
  return Status;
}

/**
 Erases and initialises a firmware volume block.

 The EraseBlocks() function erases one or more blocks as denoted
 by the variable argument list. The entire parameter list of
 blocks must be verified before erasing any blocks. If a block is
 requested that does not exist within the associated firmware
 volume (it has a larger index than the last block of the
 firmware volume), the EraseBlocks() function must return the
 status code EFI_INVALID_PARAMETER without modifying the contents
 of the firmware volume. Implementations should be mindful that
 the firmware volume might be in the WriteDisabled state. If it
 is in this state, the EraseBlocks() function must return the
 status code EFI_ACCESS_DENIED without modifying the contents of
 the firmware volume. All calls to EraseBlocks() must be fully
 flushed to the hardware before the EraseBlocks() service
 returns.

 @param This                     Indicates the EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL
 instance.

 @param ...                      The variable argument list is a list of tuples.
                                 Each tuple describes a range of LBAs to erase
                                 and consists of the following:
                                 - An EFI_LBA that indicates the starting LBA
                                 - A UINTN that indicates the number of blocks to erase.

                                 The list is terminated with an EFI_LBA_LIST_TERMINATOR.
                                 For example, the following indicates that two ranges of blocks
                                 (5-7 and 10-11) are to be erased:
                                 EraseBlocks (This, 5, 3, 10, 2, EFI_LBA_LIST_TERMINATOR);

 @retval EFI_SUCCESS             The erase request successfully completed.

 @retval EFI_ACCESS_DENIED       The firmware volume is in the WriteDisabled state.

 @retval EFI_DEVICE_ERROR        The block device is not functioning correctly and could not be written.
                                 The firmware device may have been partially erased.

 @retval EFI_INVALID_PARAMETER   One or more of the LBAs listed in the variable argument list do
                                 not exist in the firmware volume.

 **/
EFI_STATUS
EFIAPI
FvbEraseBlocks (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL *This,
  ...
  )
{
  VA_LIST       Args;
  EFI_LBA       Start, End;
  UINTN         Length;
  MEM_INSTANCE  *Instance;
  UINT32        EepromSlaveAddr = 0;
  UINT64        EepromAddr = 0;
  EFI_STATUS    Status = EFI_SUCCESS;
  UINT32        Index = 0;
  UINT8         *EraseBuff;

  Instance = INSTANCE_FROM_FVB_THIS(This);
  EraseBuff = mInstance.ShadowBuffer + EEPROM_ADDR_WIDTH_2BYTES;

  SetMem ((VOID *)EraseBuff, Instance->BlockSize, ~0);

  DEBUG ((DEBUG_INFO, "%a: Starting Erase\n", __FUNCTION__));
  VA_START (Args, This);
  for (Start = VA_ARG (Args, EFI_LBA);
    Start != EFI_LBA_LIST_TERMINATOR;
    Start = VA_ARG (Args, EFI_LBA)) {
    Length = VA_ARG (Args, UINTN);
    End = Start + Length;

    for (Index = Start; Index < End; Index++) {
      EepromSlaveAddr = GetEepromSlaveAddress(Index);
      EepromAddr = ((Index % BLOCKS_IN_ONE_PARTITION) * (Instance->BlockSize));
      Status = EepromWrite(EepromSlaveAddr, EepromAddr,
                 EEPROM_ADDR_WIDTH_2BYTES, (UINT8 *)EraseBuff,
                 Instance->BlockSize);
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "%a: EepromWrite failed\n", __FUNCTION__));
      }
    }
    SetMem ((VOID *)Instance->MemBaseAddress + Start * BLOCK_SIZE,
      Length * BLOCK_SIZE, ~0);
  }

  VA_END (Args);
  DEBUG ((DEBUG_INFO, "%a: Erase Done \n", __FUNCTION__, __LINE__));

  return EFI_SUCCESS;
}

/**
 The GetBlockSize() function retrieves the size of the requested
 block. It also returns the number of additional blocks with
 the identical size. The GetBlockSize() function is used to
 retrieve the block map (see EFI_FIRMWARE_VOLUME_HEADER).


 @param This                     Indicates the EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL instance.

 @param Lba                      Indicates the block for which to return the size.

 @param BlockSize                Pointer to a caller-allocated UINTN in which
                                 the size of the block is returned.

 @param NumberOfBlocks           Pointer to a caller-allocated UINTN in
                                 which the number of consecutive blocks,
                                 starting with Lba, is returned. All
                                 blocks in this range have a size of
                                 BlockSize.


 @retval EFI_SUCCESS             The firmware volume base address was returned.

 @retval EFI_INVALID_PARAMETER   The requested LBA is out of range.

 **/
STATIC
EFI_STATUS
FvbGetBlockSize (
  IN CONST  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *This,
  IN        EFI_LBA                            Lba,
  OUT       UINTN                              *BlockSize,
  OUT       UINTN                              *NumberOfBlocks
  )
{
  MEM_INSTANCE *Instance;

  Instance = INSTANCE_FROM_FVB_THIS(This);
  *NumberOfBlocks = Instance->NBlocks;
  *BlockSize = Instance->BlockSize;

  return EFI_SUCCESS;
}

/**
  Since we use a memory backed storage we need to restore the EEPROM contents
  into memory before we register the Fvb protocol.

  @param Instance

  @retval     0 on success, OP-TEE error on failure
**/
STATIC
EFI_STATUS
EFIAPI
ReadEntireFlash (
  MEM_INSTANCE *Instance
 )
{
  EFI_PHYSICAL_ADDRESS ReadAddr;
  EFI_STATUS Status;
  UINT64 StartOffset = 0;

  UINTN StorageVariableSize   = PcdGet32(PcdFlashNvStorageVariableSize);
  UINTN StorageFtwWorkingSize = PcdGet32(PcdFlashNvStorageFtwWorkingSize);
  UINTN StorageFtwSpareSize   = PcdGet32(PcdFlashNvStorageFtwSpareSize);

  ReadAddr = Instance->MemBaseAddress;

  Status = EepromRead(EEPROM_VARIABLE_STORE_ADDR, StartOffset,
                        EEPROM_ADDR_WIDTH_2BYTES, (UINT8 *)ReadAddr,
                        StorageVariableSize);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "EepromRead failed for Variables\n"));
    goto exit;
  }

  Status = EepromRead(EEPROM_FTW_WORKING_SPACE_ADDR, StartOffset,
                        EEPROM_ADDR_WIDTH_2BYTES, ((UINT8 *)(ReadAddr) +
                        StorageVariableSize), StorageFtwWorkingSize);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "EepromRead failed for FTW Working Space\n"));
    goto exit;
  }

  Status = EepromRead(EEPROM_FTW_SPARE_SPACE_ADDR, StartOffset,
                        EEPROM_ADDR_WIDTH_2BYTES,
                        ((UINT8 *)(ReadAddr) + StorageVariableSize +
                        StorageFtwWorkingSize), StorageFtwSpareSize);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "EepromRead failed for FTW Spare Space\n"));
    goto exit;
  }

exit:
  return Status;
}

STATIC
EFI_STATUS
EFIAPI
ValidateFvHeader (
  IN EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader
  )
{
  UINT16                      Checksum;
  VARIABLE_STORE_HEADER       *VariableStoreHeader;
  UINTN                       VariableStoreLength;
  UINTN                       FvLength;

  FvLength = PcdGet32(PcdFlashNvStorageVariableSize) +
             PcdGet32(PcdFlashNvStorageFtwWorkingSize) +
             PcdGet32(PcdFlashNvStorageFtwSpareSize);

  //
  // Verify the header revision, header signature, length
  //
  if (   (FwVolHeader->Revision  != EFI_FVH_REVISION)
      || (FwVolHeader->Signature != EFI_FVH_SIGNATURE)
      || (FwVolHeader->FvLength  != FvLength)
      )
  {
    DEBUG ((DEBUG_ERROR, "%a: No Firmware Volume header present\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  // Check the Firmware Volume Guid
  if (!CompareGuid (&FwVolHeader->FileSystemGuid, &gEfiSystemNvDataFvGuid)) {
    DEBUG ((DEBUG_INFO, "%a: Firmware Volume Guid non-compatible\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  // Verify the header checksum
  Checksum = CalculateSum16((UINT16*)FwVolHeader, FwVolHeader->HeaderLength);
  if (Checksum != 0) {
    DEBUG ((DEBUG_ERROR, "%a: FV checksum is invalid (Checksum:0x%X)\n",
      __FUNCTION__, Checksum));
    return EFI_NOT_FOUND;
  }

  VariableStoreHeader = (VARIABLE_STORE_HEADER*)((UINTN)FwVolHeader +
                                                 FwVolHeader->HeaderLength);
  // Check the Variable Store Guid
  if (!CompareGuid (&VariableStoreHeader->Signature, &gEfiVariableGuid) &&
      !CompareGuid (&VariableStoreHeader->Signature,
        &gEfiAuthenticatedVariableGuid)) {
    DEBUG ((DEBUG_ERROR, "%a: Variable Store Guid non-compatible\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  VariableStoreLength = PcdGet32 (PcdFlashNvStorageVariableSize) -
                        FwVolHeader->HeaderLength;
  if (VariableStoreHeader->Size != VariableStoreLength) {
    DEBUG ((DEBUG_ERROR, "%a: Variable Store Length does not match\n",
      __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;

}

STATIC
EFI_STATUS
InitializeFvAndVariableStoreHeaders (
  EFI_PHYSICAL_ADDRESS Addr
  )
{
  EFI_FIRMWARE_VOLUME_HEADER *FirmwareVolumeHeader;
  VARIABLE_STORE_HEADER      *VariableStoreHeader;
  EFI_STATUS                 Status = EFI_SUCCESS;
  UINTN                      HeadersLength;
  VOID*                      Headers;

  HeadersLength = sizeof(EFI_FIRMWARE_VOLUME_HEADER) +
                  sizeof(EFI_FV_BLOCK_MAP_ENTRY) +
                  sizeof(VARIABLE_STORE_HEADER);
  Headers = mInstance.ShadowBuffer + EEPROM_ADDR_WIDTH_2BYTES;

  //
  // EFI_FIRMWARE_VOLUME_HEADER
  //
  FirmwareVolumeHeader = (EFI_FIRMWARE_VOLUME_HEADER*)Headers;
  CopyGuid (&FirmwareVolumeHeader->FileSystemGuid, &gEfiSystemNvDataFvGuid);
  FirmwareVolumeHeader->FvLength =
      PcdGet32(PcdFlashNvStorageVariableSize) +
      PcdGet32(PcdFlashNvStorageFtwWorkingSize) +
      PcdGet32(PcdFlashNvStorageFtwSpareSize);
  FirmwareVolumeHeader->Signature = EFI_FVH_SIGNATURE;
  FirmwareVolumeHeader->Attributes = EFI_FVB2_READ_ENABLED_CAP |
                                     EFI_FVB2_READ_STATUS |
                                     EFI_FVB2_STICKY_WRITE |
                                     EFI_FVB2_MEMORY_MAPPED |
                                     EFI_FVB2_ERASE_POLARITY |
                                     EFI_FVB2_WRITE_STATUS |
                                     EFI_FVB2_WRITE_ENABLED_CAP;

  FirmwareVolumeHeader->HeaderLength = sizeof(EFI_FIRMWARE_VOLUME_HEADER) +
                                       sizeof(EFI_FV_BLOCK_MAP_ENTRY);
  FirmwareVolumeHeader->Revision = EFI_FVH_REVISION;
  FirmwareVolumeHeader->BlockMap[0].NumBlocks = NBLOCKS + 1;
  FirmwareVolumeHeader->BlockMap[0].Length      = BLOCK_SIZE;
  FirmwareVolumeHeader->BlockMap[1].NumBlocks = 0;
  FirmwareVolumeHeader->BlockMap[1].Length      = 0;
  FirmwareVolumeHeader->Checksum = CalculateCheckSum16 (
                                     (UINT16*)FirmwareVolumeHeader,
                                     FirmwareVolumeHeader->HeaderLength);

  //
  // VARIABLE_STORE_HEADER
  //
  VariableStoreHeader = (VOID *)((UINTN)Headers +
                                 FirmwareVolumeHeader->HeaderLength);
  CopyGuid (&VariableStoreHeader->Signature, &gEfiAuthenticatedVariableGuid);
  VariableStoreHeader->Size = PcdGet32(PcdFlashNvStorageVariableSize) -
                              FirmwareVolumeHeader->HeaderLength;
  VariableStoreHeader->Format = VARIABLE_STORE_FORMATTED;
  VariableStoreHeader->State = VARIABLE_STORE_HEALTHY;

  // Install the combined super-header in EEPROM Flash
  Status = EepromWrite(EEPROM_VARIABLE_STORE_ADDR, 0x0,
    EEPROM_ADDR_WIDTH_2BYTES, (UINT8 *)Headers, (UINT32)HeadersLength);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "EepromWrite failed for installing Super-Header\n"));
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  // Install the combined super-header in memory
  CopyMem ((VOID*) Addr, Headers, HeadersLength);

Exit:

  return Status;
}

STATIC
EFI_STATUS
EFIAPI
FvbInitialize (
  MEM_INSTANCE *Instance
  )
{
  UINTN                       mFlashNvStorageVariableBase;
  EFI_FIRMWARE_VOLUME_HEADER *FwVolHeader;
  EFI_STATUS                  Status;
  UINTN                       BusFrequency;

  if (Instance->Initialized == TRUE) {
    return EFI_SUCCESS;
  }

  // Switch I2c Bus to 1MHz to speed up the entire eeprom read
  BusFrequency = 1000000;
  Status = mI2cMaster->SetBusFrequency (mI2cMaster, &BusFrequency);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: I2CMaster->SetBusFrequency () failed - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  // FirmwareVolumeHeader->FvLength is declared to have the Variable area
  // AND the FTW working area AND the FTW Spare contiguous.
  ASSERT (PcdGet32 (PcdFlashNvStorageVariableBase) +
          PcdGet32 (PcdFlashNvStorageVariableSize) ==
          PcdGet32 (PcdFlashNvStorageFtwWorkingBase));
  ASSERT (PcdGet32 (PcdFlashNvStorageFtwWorkingBase) +
          PcdGet32 (PcdFlashNvStorageFtwWorkingSize) ==
          PcdGet32 (PcdFlashNvStorageFtwSpareBase));

  // Check if the size of the area is at least one block size
  ASSERT ((PcdGet32 (PcdFlashNvStorageVariableSize) > 0) &&
          (PcdGet32 (PcdFlashNvStorageVariableSize) / BLOCK_SIZE > 0));
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwWorkingSize) > 0) &&
          (PcdGet32 (PcdFlashNvStorageFtwWorkingSize) / BLOCK_SIZE > 0));
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwSpareSize) > 0) &&
          (PcdGet32 (PcdFlashNvStorageFtwSpareSize) / BLOCK_SIZE > 0));

  // Ensure the Variable areas are aligned on block size boundaries
  ASSERT ((PcdGet32 (PcdFlashNvStorageVariableBase) % BLOCK_SIZE) == 0);
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwWorkingBase) % BLOCK_SIZE) == 0);
  ASSERT ((PcdGet32 (PcdFlashNvStorageFtwSpareBase) % BLOCK_SIZE) == 0);

  mFlashNvStorageVariableBase = PcdGet32 (PcdFlashNvStorageVariableBase);
  // Read the file from disk and copy it to memory
  Status = ReadEntireFlash(Instance);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "ReadEntireFlash failed\n"));
    goto exit;
  }

  FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *) mFlashNvStorageVariableBase;
  Status = ValidateFvHeader(FwVolHeader);
  if (EFI_ERROR (Status)) {
    // There is no valid header, so time to install one.
    DEBUG ((DEBUG_INFO, "%a: The FVB Header is not valid.\n", __FUNCTION__));

    // Reset memory
    SetMem ((VOID *)Instance->MemBaseAddress, NBLOCKS * BLOCK_SIZE, ~0);

    DEBUG ((DEBUG_INFO, "%a: Installing a correct one for this volume.\n",
      __FUNCTION__));

    Status = InitializeFvAndVariableStoreHeaders (mFlashNvStorageVariableBase);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: InitializeFvAndVariableStoreHeaders failed \n", __FUNCTION__));
      goto exit;
    }
  } else {
    DEBUG ((DEBUG_INFO, "%a: Found valid FVB Header.\n", __FUNCTION__));
  }

  Instance->Initialized = TRUE;

exit:
  // Switch I2c Bus back to normal speeds
  BusFrequency = FixedPcdGet16 (PcdI2cSpeed);
  mI2cMaster->SetBusFrequency (mI2cMaster, &BusFrequency);

  return Status;
}

EFI_STATUS
EepromFvbInitialize ()
{
  EFI_STATUS           Status;
  VOID *Addr;

  Addr = AllocatePages(48);
  ASSERT (Addr != NULL);

  SetMem (&mInstance, sizeof (mInstance), 0);

  mInstance.FvbProtocol.GetPhysicalAddress = FvbGetPhysicalAddress;
  mInstance.FvbProtocol.GetAttributes = FvbGetAttributes;
  mInstance.FvbProtocol.SetAttributes = FvbSetAttributes;
  mInstance.FvbProtocol.GetBlockSize = FvbGetBlockSize;
  mInstance.FvbProtocol.EraseBlocks = FvbEraseBlocks;
  mInstance.FvbProtocol.Write = FvbWrite;
  mInstance.FvbProtocol.Read = FvbRead;

  mInstance.Signature =      FLASH_SIGNATURE;
  mInstance.Initialize     = FvbInitialize;
  mInstance.BlockSize =      BLOCK_SIZE;
  mInstance.NBlocks =        NBLOCKS;
  mInstance.MemBaseAddress = (EFI_PHYSICAL_ADDRESS) Addr;
  mInstance.ShadowBuffer = AllocatePool (BLOCK_SIZE + EEPROM_ADDR_WIDTH_2BYTES);

  // Update the defined PCDs related to Variable Storage
  PatchPcdSet32 (PcdFlashNvStorageVariableBase, mInstance.MemBaseAddress);
  PatchPcdSet32 (PcdFlashNvStorageFtwWorkingBase, mInstance.MemBaseAddress +
    PcdGet32 (PcdFlashNvStorageVariableSize));
  PatchPcdSet32 (PcdFlashNvStorageFtwSpareBase, mInstance.MemBaseAddress +
    PcdGet32 (PcdFlashNvStorageVariableSize) +
    PcdGet32 (PcdFlashNvStorageFtwWorkingSize));

  Status = gMmst->MmInstallProtocolInterface (
                    &mInstance.Handle,
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mInstance.FvbProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a: Register EEPROM Fvb\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "%a: Using NV store FV in-memory copy at 0x%lx\n",
    __FUNCTION__, PatchPcdGet32 (PcdFlashNvStorageVariableBase)));

  return Status;
}

EFI_STATUS
EFIAPI
EepromInitialize (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_MM_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;
  Status = EFI_SUCCESS;

  Status = gMmst->MmLocateProtocol (&gEfiI2cMasterProtocolGuid, NULL, (VOID **)&mI2cMaster);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = EepromFvbInitialize();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: EepromFVBInitialize() failed - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  return Status;
}

