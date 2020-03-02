/** I2cDxe.h
  Header defining the constant, base address amd function for I2C controller

  Copyright 2017, 2020 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __I2C_DXE_H___
#define __I2C_DXE_H__

#include <Uefi.h>

typedef struct {
  VENDOR_DEVICE_PATH        Guid;
  EFI_DEVICE_PATH_PROTOCOL  End;
} I2C_DEVICE_PATH;

#endif
