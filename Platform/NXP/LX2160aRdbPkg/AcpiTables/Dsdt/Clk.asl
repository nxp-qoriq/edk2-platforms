/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2020 NXP
  Copyright 2020 Puresoftware Ltd

  SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under Platform/ARM/JunoPkg/AcpiTables/

**/
Scope(_SB)
{
  Device(PCLK) {
    Name(_HID, "NXP0017")
    Name(OSCL, 166667)
    Name(SCLK, 0)
    Name(CLK, 0)
    Name(AVBL, 0)
    OperationRegion(RCWS, SystemMemory, DCFG_BASE, DCFG_LEN)
    Method(_REG,2) {
      if (Arg0 == "RCWS") {
        Store(Arg1, AVBL)
      }
    }
    Field (RCWS, ByteAcc, NoLock, Preserve) {
      offset(0x100),
      PCFG, 2,
      PRAT, 6,
      offset(0x124),
      RESV, 4,
      SFRQ, 10
    }

    Method(_INI, 0, NotSerialized) {
      SCLK = OSCL * SFRQ    // System clock = 166.667KHZ * SYSCLK_FREQ
      Local0 = SCLK * PRAT
      Local0 /= 2
      Store(Local0, CLK)
    }
  }
} // end of device PCLK
