/** @file

  Copyright (c) 2020, Linaro Ltd. All rights reserved.<BR>
  Copyright 2020 NXP.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent.<BR>

**/

#ifndef __EEPROM_FVB_
#define __EEPROM_FVB_

#include <Protocol/I2cMaster.h>

#define NBLOCKS                    (3 * 256) // EFI Vars, FTW working, FTW spare
#define BLOCK_SIZE                 0x100
#define BLOCKS_IN_ONE_PARTITION    256

#define FLASH_SIGNATURE            SIGNATURE_32('e', 'p', 'r', 'm')
#define INSTANCE_FROM_FVB_THIS(a)  CR(a, MEM_INSTANCE, FvbProtocol, \
                                      FLASH_SIGNATURE)

/* Since there are 4 logical blocks of size 64KB each in EEPROM which can
 * be accessed with specific slave addres 0x54-0x57, we will use single
 * block for Variable, FTW Working, FTW Spare Space */
#define EEPROM_VARIABLE_STORE_ADDR      0x54
#define EEPROM_FTW_WORKING_SPACE_ADDR   0x55
#define EEPROM_FTW_SPARE_SPACE_ADDR     0x56
#define EEPROM_VARIABLE_STORE_MAX       0xff

#define EEPROM_ADDR_WIDTH_1BYTE         0x1
#define EEPROM_ADDR_WIDTH_2BYTES        0x2
#define EEPROM_ADDR_WIDTH_3BYTES        0x3

typedef struct _MEM_INSTANCE         MEM_INSTANCE;
typedef EFI_STATUS (*MEM_INITIALIZE) (MEM_INSTANCE* Instance);
struct _MEM_INSTANCE {
  UINT32                              Signature;
  MEM_INITIALIZE                      Initialize;
  BOOLEAN                             Initialized;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  FvbProtocol;
  EFI_HANDLE                          Handle;
  EFI_PHYSICAL_ADDRESS                MemBaseAddress;
  UINT16                              BlockSize;
  UINT16                              NBlocks;
  VOID*                               ShadowBuffer;
};

typedef struct {
  UINTN                           OperationCount;
  EFI_I2C_OPERATION               SetAddressOp;
  EFI_I2C_OPERATION               GetDataOp;
} EEPROM_GET_I2C_REQUEST;

typedef EFI_I2C_REQUEST_PACKET    EEPROM_SET_I2C_REQUEST;

EFI_STATUS
EepromWrite (IN  UINT32  SlaveAddress, IN  UINT64  RegAddress,
  IN  UINT8   RegAddressWidthInBytes, IN  UINT8   *RegValue,
  IN  UINT32  RegValueNumBytes);

EFI_STATUS
EepromRead (IN  UINT32  SlaveAddress, IN  UINT64  RegAddress,
  IN  UINT8   RegAddressWidthInBytes, IN OUT  UINT8   *RegValue,
  IN  UINT32  RegValueNumBytes);

#endif
