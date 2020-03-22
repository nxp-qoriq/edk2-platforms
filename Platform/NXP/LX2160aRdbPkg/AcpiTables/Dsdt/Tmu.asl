/** @file
*  Differentiated System Description Table Fields (DSDT)
*  Implement Acpi Thermal Management
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under Platform/ARM/JunoPkg/AcpiTables/
*
**/
Scope(_TZ)
{
  // Thermal constant
  Name(TRP1, TMU_PASSIVE_THERSHOLD)
  Name(TRPC, TMU_CRITICAL_THERSHOLD)
  Name(PLC0, TMU_PASSIVE)
  Name(PLC1, TMU_PASSIVE)
  Name(PLC2, TMU_PASSIVE)
  Name(PLC3, TMU_PASSIVE)
  Name(PLC4, TMU_PASSIVE)
  Name(PLC5, TMU_PASSIVE)
  Name(PLC6, TMU_PASSIVE)

  // TMU hardware registers
  OperationRegion(TMUR, SystemMemory, TMU_BASE, TMU_LEN)
  Field (TMUR, DWordAcc, NoLock, Preserve) {
    TMR,  32,
    TSR,  32,
    TMSR, 32,
    TMIR, 32,
    offset(0x20),
    TIER, 32,
    TIDR, 32,
    offset(0x30),
    TISC, 32,
    ASCR, 32,
    CSCR, 32,
    offset(0x40),
    HTCR, 32,
    LTCR, 32,
    TRCR, 32,
    TFCR, 32,
    TITR, 32,
    TATR, 32,
    ACTR, 32,
    offset(0x60),
    LITR, 32,
    LATR, 32,
    MCTR, 32,
    offset(0x70),
    RCTR, 32,
    FCTR, 32,
    offset(0x80),
    TCFG, 32,
    SCFG, 32,
    offset(0x100),
    ISR0, 32,
    ASR0, 32,
    offset(0x110),
    ISR1, 32,
    ASR1, 32,
    offset(0x120),
    ISR2, 32,
    ASR2, 32,
    offset(0x130),
    ISR3, 32,
    ASR3, 32,
    offset(0x140),
    ISR4, 32,
    ASR4, 32,
    offset(0x150),
    ISR5, 32,
    ASR5, 32,
    offset(0x160),
    ISR6, 32,
    ASR6, 32,
    offset(0xF08),
    TEMR, 32,
    offset(0xF10),
    TCR0, 32,
    TCR1, 32,
    TCR2, 32,
    TCR3, 32
  }

  //Method to read the sensors current temperature
  Method(GTMP, 1, Serialized) {
    Switch (Arg0) {
      Case (0) { Local0 = ISR0 }
      Case (1) { Local0 = ISR1 }
      Case (2) { Local0 = ISR2 }
      Case (3) { Local0 = ISR3 }
      Case (4) { Local0 = ISR4 }
      Case (5) { Local0 = ISR5 }
      Case (6) { Local0 = ISR6 }
      Default  { Local0 = ISR0 }
    }
    // Adjustment according to the linux kelvin_offset
    Local0 = Local0 * 10 + 2
    Return (Local0)
  }

  Device(TMU) {
    Name(_HID, "NXP0012")
    Name(_UID, 0)

    Method(_INI, 0, NotSerialized) {
      // Disable interrupt, using polling instead
      Store(TMU_TIDR_DISABLE_ALL, TIDR)
      Store(TMU_TIER_DISABLE_ALL, TIER)
      // Set update_interval
      Store(TMU_TMTMIR_DEFAULT, TMIR)
      // Disable monitoring
      Store(TMU_TMR_DISABLE, TMR)
      // Init temperature range registers
      Store(TMU_TEMP_RANGE_0, TCR0)
      Store(TMU_TEMP_RANGE_1, TCR1)
      // Init TMU configuration Table
      Store(TMU_POINT_0_TEMP_CFG, TCFG)
      Store(TMU_POINT_0_SENSOR_CFG, SCFG)
      Store(TMU_POINT_1_TEMP_CFG, TCFG)
      Store(TMU_POINT_1_SENSOR_CFG, SCFG)
      Store(TMU_SENSOR_ENABLE_ALL, TMSR)
      // Enable Monitoring
      Store(TMU_TMR_ENABLE, TMR)
    }
  }

  // ThermalZone for core cluster 6,7
  ThermalZone(THM0) {
    Name(_STR, Unicode("system-thermal-zone-0"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)
    // Passive cooling device list
    Name(_PSL, Package() {
      \_SB.CP12,
      \_SB.CP13,
      \_SB.CP14,
      \_SB.CP15
    })

    Method(_SCP, 1) {
      If (Arg0) {
        Store(1, PLC0)
      } Else {
        Store(0, PLC0)
      }
      Notify(\_TZ.THM0, 0x81)
    }

    Method(_TMP, 0) {
      Return(GTMP(0))
    }

    Method(_CRT, 0) {
      Return(TRPC)
    }

    Method(_PSV, 0) {
      Return(TRP1)
    }
  }

  // ThermalZone for core cluster 5
  ThermalZone(THM1) {
    Name(_STR, Unicode("system-thermal-zone-1"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)
    // Passive cooling device list
    Name(_PSL, Package() {
      \_SB.CP10,
      \_SB.CP11
    })

    Method(_SCP, 1) {
      If (Arg0) {
        Store(1, PLC1)
      } Else {
        Store(0, PLC1)
      }
      Notify(\_TZ.THM1, 0x81)
    }

    Method(_TMP, 0) {
      Return(GTMP(1))
    }

    Method(_CRT, 0) {
      Return(TRPC)
    }

    Method(_PSV, 0) {
      Return(TRP1)
    }
  }

  // ThermalZone for WRIOP
  ThermalZone(THM2) {
    Name(_STR, Unicode("system-thermal-zone-2"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    Method(_SCP, 1) {
      If (Arg0) {
        Store(1, PLC2)
      } Else {
        Store(0, PLC2)
      }
      Notify(\_TZ.THM1, 0x81)
    }

    Method(_TMP, 0) {
      Return(GTMP(2))
    }

    Method(_CRT, 0) {
      Return(TRPC)
    }

    Method(_PSV, 0) {
      Return(TRP1)
    }
  }

  // ThermalZone for DCE, QBMAN, HSIO3
  ThermalZone(THM3) {
    Name(_STR, Unicode("system-thermal-zone-3"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    Method(_SCP, 1) {
      If (Arg0) {
        Store(1, PLC3)
      } Else {
        Store(0, PLC3)
      }
      Notify(\_TZ.THM1, 0x81)
    }

    Method(_TMP, 0) {
      Return(GTMP(3))
    }

    Method(_CRT, 0) {
      Return(TRPC)
    }

    Method(_PSV, 0) {
      Return(TRP1)
    }
  }

  // ThermalZone for CCN508, DPAA, TBU
  ThermalZone(THM4) {
    Name(_STR, Unicode("system-thermal-zone-4"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    Method(_SCP, 1) {
      If (Arg0) {
        Store(1, PLC4)
      } Else {
        Store(0, PLC4)
      }
      Notify(\_TZ.THM1, 0x81)
    }

    Method(_TMP, 0) {
      Return(GTMP(4))
    }

    Method(_CRT, 0) {
      Return(TRPC)
    }

    Method(_PSV, 0) {
      Return(TRP1)
    }
  }

  // ThermalZone for core cluster 4
  ThermalZone(THM5) {
    Name(_STR, Unicode("system-thermal-zone-5"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)
    // Passive cooling device list
    Name(_PSL, Package() {
      \_SB.CPU8,
      \_SB.CPU9,
    })

    Method(_SCP, 1) {
      If (Arg0) {
        Store(1, PLC5)
      } Else {
        Store(0, PLC5)
      }
      Notify(\_TZ.THM2, 0x81)
    }

    Method(_TMP, 0) {
      Return(GTMP(5))
    }

    Method(_CRT, 0) {
      Return(TRPC)
    }

    Method(_PSV, 0) {
      Return(TRP1)
    }
  }

  // ThermalZone for core cluster 2,3
  ThermalZone(THM6) {
    Name(_STR, Unicode("system-thermal-zone-6"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)
    // Passive cooling device list
    Name(_PSL, Package() {
      \_SB.CPU4,
      \_SB.CPU5,
      \_SB.CPU6,
      \_SB.CPU7
    })

    Method(_SCP, 1) {
      If (Arg0) {
        Store(1, PLC6)
      } Else {
        Store(0, PLC6)
      }
      Notify(\_TZ.THM3, 0x81)
    }

    Method(_TMP, 0) {
      Return(GTMP(6))
    }

    Method(_CRT, 0) {
      Return(TRPC)
    }

    Method(_PSV, 0) {
      Return(TRP1)
    }
  }
} //end of Scope _TZ
