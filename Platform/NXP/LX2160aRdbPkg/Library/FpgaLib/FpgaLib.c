/** @FpgaLib.c
  Fpga Library for LX2160A-RDB board, containing functions to
  program and read the Fpga registers.

  FPGA is connected to IFC Controller and so MMIO APIs are used
  to read/write FPGA registers

  Copyright 2018, 2020 NXP

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
#include <Library/FpgaLib.h>
#include <Library/I2cLib.h>

/**
   Function to read FPGA register.

   @param  Reg  Register offset of FPGA to read.

**/
UINT8
FpgaRead (
  IN  UINTN  Reg
  )
{
  UINT8  Val;
  UINTN  I2cBase;

  I2cBase = ( EFI_PHYSICAL_ADDRESS)(FixedPcdGet64 (PcdI2c0BaseAddr) +
                        (QIXIS_BASE_I2C_BUS * FixedPcdGet32 (PcdI2cSize)));

  I2cBusReadReg (I2cBase, QIXIS_BASE_I2C_ADR, Reg, 1, &Val, 1);
  return Val;
}

/**
   Function to Write FPGA register.

   @param  Reg  Register offset of FPGA to write.
   @param  Value Value to be written.

**/
VOID
FpgaWrite (
  IN  UINTN  Reg,
  IN  UINT8  Value
  )
{
  UINTN                   I2cBase;
  UINT8                   Buffer[2];
  EFI_I2C_REQUEST_PACKET  Req;

  I2cBase = ( EFI_PHYSICAL_ADDRESS)(FixedPcdGet64 (PcdI2c0BaseAddr) +
                        (QIXIS_BASE_I2C_BUS * FixedPcdGet32 (PcdI2cSize)));

  Buffer[0] = Reg;
  Buffer[1] = Value;

  Req.OperationCount                   = 1;
  Req.Operation[0].Flags               = 0;
  Req.Operation[0].LengthInBytes       = sizeof (Buffer);
  Req.Operation[0].Buffer              = (VOID *)Buffer;

  I2cBusXfer (I2cBase, QIXIS_BASE_I2C_ADR,  &Req);
}

/**
   Function to get board system clock frequency.

**/
UINTN
GetBoardSysClk (
  VOID
  )
{
  UINT8 SysclkConf;

  SysclkConf = FPGA_READ (BrdCfg[1]);
  switch (SysclkConf & FPGA_CLK_MASK) {
    case CLK_100:
      return SYSCLK_100_MHZ;
  }
  return SYSCLK_100_MHZ;
}

/**
   Function to print board personality.

**/
VOID
PrintBoardPersonality (
  VOID
  )
{
  UINT8 SwitchConf;
  SwitchConf = FPGA_READ (Arch);

  DEBUG ((DEBUG_INFO, "Board Arch: V%d, ", SwitchConf >> 4));
  DEBUG ((DEBUG_INFO, "Board version: %c, boot from ",
        (SwitchConf & 0xf) + 'A'));

  SwitchConf = FPGA_READ (BrdCfg[0]);

  if (SwitchConf & FPGA_CS_MASK)
    DEBUG ((DEBUG_INFO, "NAND\n"));
  else
    DEBUG ((DEBUG_INFO,  "vBank: %d\n", (SwitchConf & FPGA_VBANK_MASK)));

  DEBUG ((DEBUG_INFO, "FPGA: v%d.%d\n", FPGA_READ (Ver),
        FPGA_READ (Minor)));
}
