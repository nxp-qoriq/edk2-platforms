/** @file
*  DSDT : TMU ACPI Information
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_TZ)
{
  // Thermal constants
  Name(TRPC, TMU_CRITICAL_THRESHOLD)
  Name(PLC0, TMU_PASSIVE)
  Name(PLC1, TMU_PASSIVE)
  Name(PLC2, TMU_PASSIVE)
  Name(PLC3, TMU_PASSIVE)
  Name(PLC4, TMU_PASSIVE)
  Name(PLC5, TMU_PASSIVE)

  // TMU hardware registers
  OperationRegion(TMUR, SystemMemory, TMU_BASE, TMU_LEN)
  Field (TMUR, DWordAcc, NoLock, Preserve) {
    TMR,  32,           // TMU mode register
    TSR,  32,           // TMU status register
    TMIR, 32,           // TMU monitor temperature interval register
    offset(0x20),
    TIER, 32,           // TMU interrupt enable register
    TIDR, 32,           // TMU interrupt detect register
    TISC, 32,           // TMU interrupt site capture register
    CSCR, 32,           // TMU interrupt critical site capture register
    offset(0x40),
    HTCR, 32,           // TMU monitor high temp capture register
    LTCR, 32,           // TMU monitor low temp capture register
    offset(0x50),
    TITR, 32,           // TMU monitor high temp immediate threshold register
    TATR, 32,           // TMU monitor high temp avg. threshold register
    ACTR, 32,           // TMU monitor high temp avg. critical threshold register
    offset(0x80),
    TCFG, 32,           // TMU temperature configuration register
    SCFG, 32,           // TMU sensor configuration register
    offset(0x100),
    ISR0, 32,           // TMU report immediate temp site register 0
    ASR0, 32,           // TMU report average temp site register 0
    offset(0x110),
    ISR1, 32,           // TMU report immediate temp site register 1
    ASR1, 32,           // TMU report average temp site register 1
    offset(0x120),
    ISR2, 32,           // TMU report immediate temp site register 2
    ASR2, 32,           // TMU report average temp site register 2
    offset(0x130),
    ISR3, 32,           // TMU report immediate temp site register 3
    ASR3, 32,           // TMU report average temp site register 3
    offset(0x140),
    ISR4, 32,           // TMU report immediate temp site register 4
    ASR4, 32,           // TMU report average temp site register 4
    offset(0xBF8),
    IBR1, 32,          // IP block revision register 0
    IBR2, 32,          // IP block revision register 2
    offset(0xF00),
    EMR0, 32,
    offset(0xF10),
    TCR0, 32,         // TMU temp range control register 0
    TCR1, 32,         // TMU temp range control register 1
    TCR2, 32,         // TMU temp range control register 2
    TCR3, 32          // TMU temp range control register 3
  }

  // Method to read the sensors current temperature
  Method (GTMP, 1, Serialized) {
    Switch (Arg0) {
      Case (0) {
                 And (ISR0, 0xFF000000, Local0)
               }
      Case (1) {
                 And (ISR1, 0xFF000000, Local0)
               }
      Case (2) {
                 And (ISR2, 0xFF000000, Local0)
               }
      Case (3) {
                 And (ISR3, 0xFF000000, Local0)
               }
      Case (4) {
                 And (ISR4, 0xFF000000, Local0)
               }
      Default  {
                 And (ISR0, 0xFF000000, Local0)
               }
    }
    // Temperature is stored in Degree Celcius.
    // Adjustment according to the linux kelvin_offset(2732)
    ShiftRight (Local0, 24, Local0)
    Add (Local0, 273, Local0)
    Local0 = Local0 * 10 + 2
    Return (Local0)
  }

  // Method to swap the 4 bytes of the provided data
  Method (SWAP, 1 , Serialized)
  {
    And (Arg0, 0xFF000000, Local0)
    ShiftRight (Local0, 24, Local0)
    And (Arg0, 0xFF0000, Local1)
    And (Arg0, 0xFF0000, Local1)
    ShiftRight (Local1, 8, Local1)
    And (Arg0, 0xFF00, Local2)
    ShiftLeft (Local2, 8, Local2)
    And (Arg0, 0xFF, Local3)
    ShiftLeft (Local3, 24, Local3)

    Or (Local0, Local1, Local0)
    Or (Local0, Local2, Local0)
    Or (Local0, Local3, Local0)

    Return (Local0)
  }

  // Method to write the calibration data to temp. and sensor config registers
  Method (CTCF, 0)
  {
    Store (SWAP(TMU_POINT_1_0_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_0_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_0_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_0_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_1_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_1_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_2_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_2_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_3_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_3_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_4_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_4_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_5_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_5_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_6_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_6_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_7_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_7_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_8_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_8_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_9_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_9_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_10_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_10_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_1_11_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_1_11_SENSOR_CFG), SCFG)

    Store (SWAP(TMU_POINT_2_0_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_0_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_1_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_1_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_2_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_2_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_3_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_3_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_4_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_4_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_5_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_5_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_6_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_6_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_7_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_7_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_8_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_8_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_2_9_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_2_9_SENSOR_CFG), SCFG)

    Store (SWAP(TMU_POINT_3_0_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_3_0_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_3_1_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_3_1_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_3_2_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_3_2_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_3_3_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_3_3_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_3_4_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_3_4_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_3_5_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_3_5_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_3_6_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_3_6_SENSOR_CFG), SCFG)

    Store (SWAP(TMU_POINT_4_0_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_4_0_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_4_1_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_4_1_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_4_2_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_4_2_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_4_3_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_4_3_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_4_4_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_4_4_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_4_5_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_4_6_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_4_6_SENSOR_CFG), SCFG)
    Store (SWAP(TMU_POINT_4_7_TEMP_CFG), TCFG)
    Store (SWAP(TMU_POINT_4_7_SENSOR_CFG), SCFG)
  }

  Device(TMU) {
    Name(_HID, "NXP0012")
    Name(_UID, 0)

    //TMU initialization Method.
    Method (_INI, 0, Serialized) {
      // Disable interrupt, using polling instead
      Store (SWAP(TMU_TIDR_DISABLE_ALL), TIDR)
      Store (SWAP(TMU_TIER_DISABLE_ALL), TIER)
      Store (SWAP(TMU_ENGINEERING_MODE), EMR0)
      // Disable Monitoring
      Store (SWAP(TMU_TMR_DISABLE), TMR)
      Store (SWAP(TMU_ENGINEERING_MODE), EMR0)
      // Init temperature range registers
      Store (SWAP(TMU_TEMP_RANGE_0), TCR0)
      Store (SWAP(TMU_TEMP_RANGE_1), TCR1)
      Store (SWAP(TMU_TEMP_RANGE_2), TCR2)
      Store (SWAP(TMU_TEMP_RANGE_3), TCR3)
      // Init TMU configuration Table
      CTCF()
      // Set sites but disable Monitoring mode in TMR
      Store (SWAP(TMU_TMR_SITES_ENABLE), TMR)
      // Set update_interval
      Store (SWAP(TMU_TMTMIR_DEFAULT), TMIR)
      Store (SWAP(TMU_TMR_ENABLE), TMR)
    }
  }

  // Thermal zone for Sensor near DDR Controller
  ThermalZone(THM0) {
    Name(_STR, Unicode("system-thermal-zone-0"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    // Thermal Object: Set Cooling Policy
    Method(_SCP, 1) {
      If (Arg0) {
        Store (1, PLC0)
      } Else {
        Store (0, PLC0)
      }
      Notify(\_TZ.THM0, 0x81)
    }

    // Thermal Object: Temperature
    Method(_TMP, 0) {
      Return (GTMP(0))
    }

    // Thermal Object: Critical Temperature
    Method(_CRT, 0) {
      Return (TRPC)
    }
  }

  // ThermalZone for sensor near SerDes
  ThermalZone(THM1) {
    Name(_STR, Unicode("system-thermal-zone-1"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    // Thermal Object: Set Cooling Policy
    Method(_SCP, 1) {
      If (Arg0) {
        Store (1, PLC1)
      } Else {
        Store (0, PLC1)
      }
      Notify(\_TZ.THM1, 0x81)
    }

    // Thermal Object: Temperature
    Method(_TMP, 0) {
      Return (GTMP(1))
    }

    // Thermal Object: Critical Temperature
    Method(_CRT, 0) {
      Return (TRPC)
    }
  }

  // ThermalZone for sensor near Frame manager
  ThermalZone(THM2) {
    Name(_STR, Unicode("system-thermal-zone-2"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    // Thermal Object: Set Cooling Policy
    Method(_SCP, 1) {
      If (Arg0) {
        Store (1, PLC2)
      } Else {
        Store (0, PLC2)
      }
      Notify(\_TZ.THM2, 0x81)
    }

    // Thermal Object: Temperature
    Method(_TMP, 0) {
      Return (GTMP(2))
    }

    // Thermal Object: Critical Temperature
    Method(_CRT, 0) {
      Return (TRPC)
    }
  }

  // ThermalZone for sensor near Arm A72 core
  ThermalZone(THM3) {
    Name(_STR, Unicode("system-thermal-zone-3"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    // Thermal Object: Passive cooling device list
    Name(_PSL, Package() {
      \_SB.CPU0,
      \_SB.CPU1,
      \_SB.CPU2,
      \_SB.CPU3
    })

    // Thermal Object: Set Cooling Policy
    Method(_SCP, 1) {
      If (Arg0) {
        Store (1, PLC3)
      } Else {
        Store (0, PLC3)
      }
      Notify(\_TZ.THM3, 0x81)
    }

    // Thermal Object: Temperature
    Method(_TMP, 0) {
      Return (GTMP(3))
    }

    // Thermal Object: Critical Temperature
    Method(_CRT, 0) {
      Return (TRPC)
    }
  }

  // ThermalZone for sensor near SEC
  ThermalZone(THM4) {
    Name(_STR, Unicode("system-thermal-zone-4"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    // Thermal Object: Set Cooling Policy
    Method(_SCP, 1) {
      If (Arg0) {
        Store (1, PLC4)
      } Else {
        Store (0, PLC4)
      }
      Notify(\_TZ.THM4, 0x81)
    }

    // Thermal Object: Temperature
    Method(_TMP, 0) {
      Return (GTMP(4))
    }

    // Thermal Object: Critical Temperature
    Method(_CRT, 0) {
      Return (TRPC)
    }
  }

  ThermalZone(THM5) {
    Name(_STR, Unicode("system-thermal-zone-5"))
    Name(_TZP, TMU_TZ_POLLING_PERIOD)
    Name(_TSP, TMU_TZ_SAMPLING_PERIOD)
    Name(_TC1, TMU_THERMAL_COFFICIENT_1)
    Name(_TC2, TMU_THERMAL_COFFICIENT_2)

    Method(_SCP, 1, Serialized) {
      If (Arg0) {
        Store(1, PLC4)
      } Else {
        Store(0, PLC4)
      }
    }

    Method(_TMP, 0, Serialized) {
      Store(\_SB.I2C0.STMP(), Local0)
        // Temperature is stored in Degree Celcius.
        // Adjustment according to the linux kelvin_offset(2732)
        Local0 += 273
        Local0 = Local0 * 10 + 2
        Return (Local0)
    }

    Method(_CRT, 0, Serialized) {
      Return(TRPC)
    }
  }
} //end of Scope _TZ
