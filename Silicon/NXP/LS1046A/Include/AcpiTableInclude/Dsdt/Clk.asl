/** @file
*  DSDT : Dynamic Clock ACPI Information
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Device(PCLK) {
    Name(_HID, "NXP0017")
    Name(CLK, 0)
    Name(AVBL, 0)
    OperationRegion(RCWS, SystemMemory, DCFG_BASE, DCFG_LEN)
    Method(_REG,2) {
      if (Arg0 == "RCWS") {
        Store(Arg1, AVBL)
      }
    }
    Field (RCWS, ByteAcc, NoLock, Preserve) {
      /* The below table provides the func of diff bits in 512 bits RCW data:
         SYS_PLL_CFG : 0-1 bits
         SYS_PLL_RAT : 2-6 bits
         SYSCLK_FREQ : 472-481 bits etc.
         Refer LS1046ARM for more info.
         For LS1046 RCWSRs are read as RCW[0:31] .
      */
      offset(0x100),
      RESV, 1,
      PRAT, 5,
      PCFG, 2,
      offset(0x13B),
      HFRQ, 8,  // Higher 8 bits of SYSCLK_FREQ
      RESX, 6,
      LFRQ, 2   // Lower bits of SYSCLK_FREQ
    }

    Method(_INI, 0, NotSerialized) {
      Local0 = (HFRQ<<2 | LFRQ) // Concatinating LFRQ at end of HFRQ
      Multiply(Local0, 500000, Local0)
      Multiply(Local0, PRAT, Local0)
      Divide(Local0, 3, Local1, Local0)
      Store(Local0, CLK)
    }
  } // end of device PCLK
}
