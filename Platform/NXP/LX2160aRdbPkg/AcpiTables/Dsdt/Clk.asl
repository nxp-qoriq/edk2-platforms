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
    Name(CLK, 0) // Maximum Platform Clock (Hz)
    Name(CLKA, 0) // Maximum CPU Core Clock CGA(MHz)
    Name(CLKB, 0) // Maximum CPU Core Clock CGB(MHz)
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
      offset(0x103),
      RES1, 2,
      CAPR, 6, // Cluster Group A PLL Ratio
      offset(0x106),
      RES2, 2,
      CBPR, 6, // Cluster Group B PLL Ratio
      offset(0x124),
      RESV, 4,
      SFRQ, 10
    }

    Method(_INI, 0, NotSerialized) {
      /*Calculating Platform Clock*/
      Store(SFRQ, Local0)
      Multiply(Local0, 500000, Local0)
      Multiply(Local0, PRAT, Local0)
      Divide(Local0, 6, , Local0)
      Store(Local0, CLK)

      /*Calculating maximum Core Clock*/
      Store(SFRQ, Local0)
      Multiply(Local0, 500000, Local0)
      Divide(Local0, 3, , Local0)
      Divide(Local0, 1000000, , Local0) //Just the MHz part of SYSCLK.
      Multiply(Local0, CAPR, CLKA) // PLL_Ratio * SYSCLK, Max Freq of Cluster Group A
      Multiply(Local0, CBPR, CLKB) // PLL_Ratio * SYSCLK, Max Freq of Cluster Group B
    }
  }
} // end of device PCLK
