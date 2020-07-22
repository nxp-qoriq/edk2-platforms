/** @file
*  Differentiated System Description Table Fields (DSDT)
*  Implement ACPI Thermal Management
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB.I2C0)
{
  Name (SDB0, ResourceTemplate() {
    I2CSerialBus(0x77, ControllerInitiated, 100000, AddressingMode7Bit,
                 "\\_SB.I2C0", 0, ResourceConsumer, ,)
  })

  Name (SDB1, ResourceTemplate() {
    I2CSerialBus(0x4C, ControllerInitiated, 100000, AddressingMode7Bit,
                 "\\_SB.I2C0", 0, ResourceConsumer, ,)
  })

  Name (AVBL, Zero)
  // _REG: Region Availability
  Method (_REG, 2, NotSerialized) {
    If (LEqual (Arg0, 0x09)) {
       Store (Arg1, AVBL)
    }
  }

  OperationRegion(TOP1, GenericSerialBus, 0x00, 0x100)
  Field(TOP1, BufferAcc, NoLock, Preserve) {
    Connection(SDB0),
    AccessAs (BufferAcc, AttribByte),
    FLD0, 8,                           // Virtual register at command value 0x00
    Connection(SDB1),
    offset(0x01),                      // sa56004fd remote sensor temperature register offset
    AccessAs (BufferAcc, AttribByte),
    FLD1, 8                            // Virtual register at command value 0x01
  }

  Name(BUFF, Buffer(34){})
  CreateByteField(BUFF, 0x00, STAT)
  CreateByteField(BUFF, 0x01, LEN)
  CreateByteField(BUFF, 0x02, DATA)

  // Method to set pca9546 channel id
  Method (SCHN, 1, Serialized) {
    Switch (Arg0) {
      case (0) {
        Store(0x01, DATA)
      }
      case (1) {
        Store(0x02, DATA)
      }
      case (2) {
        Store(0x04, DATA)
      }
      case (3) {
        Store(0x08, DATA)
      }
      Default {
        Store(0x01, DATA)
      }
    }
    Store(One, LEN)
    Store(BUFF, FLD0) //write
    Return (STAT)
  }

  // Method to read temperature from remote sensor
  Method (STMP, 0, Serialized) {
    Store(Zero, Local0)
    If (LEqual (\_SB.I2C0.AVBL, One)) {
      SCHN(I2C0_MUX_CHANNEL_0)
      Store(One, LEN)
      Store(FLD1, BUFF) //read
      If (LEqual(STAT, 0x00)) {
        Local0 = DATA
      }
    }
    Return (Local0)
  }
}

Scope(_SB.I2C3)
{
  Name (SDB0, ResourceTemplate() {
    I2CSerialBus(0x4D, ControllerInitiated, 100000, AddressingMode7Bit,
                 "\\_SB.I2C3", 0, ResourceConsumer, ,)
  })

  Name(BUFF, Buffer(34){})
  CreateByteField(BUFF, 0x00, STAT)
  CreateByteField(BUFF, 0x01, LEN)
  CreateByteField(BUFF, 0x02, DATA)

  Name (AVBL, Zero)
  // _REG: Region Availability
  Method (_REG, 2, NotSerialized) {
    If (LEqual (Arg0, 0x09)) {
       Store (Arg1, AVBL)
    }
  }

  OperationRegion(TOP1, GenericSerialBus, 0x00, 0x100)
  Field(TOP1, BufferAcc, NoLock, Preserve) {
    Connection(SDB0),
    offset(0x30),                      // emc2305 FAN 1 setting register offset
    AccessAs (BufferAcc, AttribByte),
    FLD0, 8,                           // Virtual register at command value 0x30
    Connection(SDB0),
    offset(0x40),                      // emc2305 FAN 2 setting register offset
    AccessAs (BufferAcc, AttribByte),
    FLD1, 8,                           // Virtual register at command value 0x40
    Connection(SDB0),
    offset(0x50),                      // emc2305 FAN 3 setting register offset
    AccessAs (BufferAcc, AttribByte),
    FLD2, 8,                           // Virtual register at command value 0x50
    Connection(SDB0),
    offset(0x60),                      // emc2305 FAN 4 setting register offset
    AccessAs (BufferAcc, AttribByte),
    FLD3, 8,                           // Virtual register at command value 0x60
    Connection(SDB0),
    offset(0x70),                      // EMC2305 FAN 5 setting register offset
    AccessAs (BufferAcc, AttribByte),
    FLD4, 8
  }

  Method (FSTA, 1, Serialized) {
    Store(Zero, Local0)
    Store(One, LEN)
    Switch(Arg0) {
      Case (TMU_FAN_1) {
        Store(FLD0, BUFF)
      }
      Case (TMU_FAN_2) {
        Store(FLD1, BUFF)
      }
      Case (TMU_FAN_3) {
        Store(FLD2, BUFF)
      }
      Case (TMU_FAN_4) {
        Store(FLD3, BUFF)
      }
      Case (TMU_FAN_5) {
        Store(FLD4, BUFF)
      }
      Default {
      }
    }
    If (LEqual(STAT, 0x00)) {
      Local0 = DATA
    }
    Return (Local0)
  }

  // Method to turn fan OFF
  Method(FOFF, 1, Serialized) {
    Store(One, LEN)
    Store(TMU_FAN_OFF_SPEED, DATA)
    Switch(Arg0) {
      Case (TMU_FAN_1) {
        Store(BUFF, FLD0)
      }
      Case (TMU_FAN_2) {
        Store(BUFF, FLD1)
      }
      Case (TMU_FAN_3) {
        Store(BUFF, FLD2)
      }
      Case (TMU_FAN_4) {
        Store(BUFF, FLD3)
      }
      Case (TMU_FAN_5) {
        Store(BUFF, FLD4)
      }
      Default {
      }
    }
    Return (STAT)
  }

  // Method to turn fan ON at Low speed
  Method(FONL, 1, Serialized) {
    Store(One, LEN)
    Store(TMU_FAN_LOW_SPEED, DATA)
    Switch(Arg0) {
      Case (TMU_FAN_1) {
        Store(BUFF, FLD0)
      }
      Case (TMU_FAN_2) {
        Store(BUFF, FLD1)
      }
      Case (TMU_FAN_3) {
        Store(BUFF, FLD2)
      }
      Case (TMU_FAN_4) {
        Store(BUFF, FLD3)
      }
      Case (TMU_FAN_5) {
        Store(BUFF, FLD4)
      }
      Default {
      }
    }
    Return (STAT)
  }

  // Method to turn fan ON at high speed
  Method(FONH, 1, Serialized) {
    Store(One, LEN)
    Store(TMU_FAN_HIGH_SPEED, DATA)
    Switch(Arg0) {
      Case (TMU_FAN_1) {
        Store(BUFF, FLD0)
      }
      Case (TMU_FAN_2) {
        Store(BUFF, FLD1)
      }
      Case (TMU_FAN_3) {
        Store(BUFF, FLD2)
      }
      Case (TMU_FAN_4) {
        Store(BUFF, FLD3)
      }
      Case (TMU_FAN_5) {
        Store(BUFF, FLD4)
      }
      Default {
      }
    }
    Return (STAT)
  }
}

Scope(_TZ)
{
  // Thermal constants
  Name(TRPP, TMU_PASSIVE_THRESHOLD)
  Name(TRPC, TMU_CRITICAL_THRESHOLD)
  Name(TRP0, TMU_ACTIVE_HIGH_THRESHOLD)
  Name(TRP1, TMU_ACTIVE_LOW_THRESHOLD)
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

  // FAN 1 power Resource at low speed
  PowerResource(FN1L, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(1), Local0)
        If (LGreater(Local0, TMU_FAN_OFF_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONL(TMU_FAN_1)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_1)
      }
    }
  }

  // FAN 1 power resources at high speed
  PowerResource(FN1H, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(1), Local0)
        If (LGreater(Local0, TMU_FAN_LOW_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONH(TMU_FAN_1)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_1)
      }
    }
  }

  // FAN 2 power resource at low speed
  PowerResource(FN2L, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(2), Local0)
        If (LGreater(Local0, TMU_FAN_OFF_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONL(TMU_FAN_2)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_2)
      }
    }
  }

  // FAN 2 power resources at high speed
  PowerResource(FN2H, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(2), Local0)
        If (LGreater(Local0, TMU_FAN_LOW_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONH(TMU_FAN_2)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_2)
      }
    }
  }

  // FAN 3 power resource at low speed
  PowerResource(FN3L, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(3), Local0)
        If (LGreater(Local0, TMU_FAN_OFF_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONL(TMU_FAN_3)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_3)
      }
    }
  }

  // FAN 3 power resource at high speed
  PowerResource(FN3H, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(3), Local0)
        If (LGreater(Local0, TMU_FAN_LOW_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONH(TMU_FAN_3)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_3)
      }
    }
  }

  // FAN 4 power resource at low speed
  PowerResource(FN4L, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(4), Local0)
        If (LGreater(Local0, TMU_FAN_OFF_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONL(TMU_FAN_4)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_4)
      }
    }
  }

  // FAN 4 power resource at high speed
  PowerResource(FN4H, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(4), Local0)
        If (LGreater(Local0, TMU_FAN_LOW_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONH(TMU_FAN_4)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_4)
      }
    }
  }

  // FAN 5 power resource at low speed
  PowerResource(FN5L, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(5), Local0)
        If (LGreater(Local0, TMU_FAN_OFF_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONL(TMU_FAN_5)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_5)
      }
    }
  }

  // FAN 5 power resource at high speed
  PowerResource(FN5H, 0, 0) {
    Method (_STA) {
      Store(Zero, Local1)
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        Store(\_SB.I2C3.FSTA(5), Local0)
        If (LGreater(Local0, TMU_FAN_LOW_SPEED)) {
          Store(One, Local1)
        } Else {
          Store(Zero, Local1)
        }
      }
      Return(Local1)
    }
    Method (_ON) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FONH(TMU_FAN_5)
      }
    }
    Method (_OFF) {
      If (LEqual (\_SB.I2C3.AVBL, One)) {
        \_SB.I2C3.FOFF(TMU_FAN_5)
      }
    }
  }

  // FAN 0 device object
  Device (FAN0) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 0)
   Name(_PR0, Package() { FN1L })
  }

  // FAN 1 device object
  Device (FAN1) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 1)
   Name(_PR0, Package() { FN1L, FN1H })
  }

  // FAN 2 device object
  Device (FAN2) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 2)
   Name(_PR0, Package() { FN2L })
  }

  // FAN 3 device object
  Device (FAN3) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 3)
   Name(_PR0, Package() { FN2L, FN2H })
  }

  // FAN 4 device object
  Device (FAN4) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 4)
   Name(_PR0, Package() { FN3L })
  }

  // FAN 5 device object
  Device (FAN5) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 5)
   Name(_PR0, Package() { FN3L, FN3H })
  }

  // FAN 6 device object
  Device (FAN6) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 6)
   Name(_PR0, Package() { FN4L })
  }

  // FAN 7 device object
  Device (FAN7) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 7)
   Name(_PR0, Package() { FN4L, FN4H })
  }

  // FAN 8 device object
  Device (FAN8) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 8)
   Name(_PR0, Package() { FN5L })
  }

  // FAN 9 device object
  Device (FAN9) {
   // Device ID for the FAN
   Name(_HID, EISAID("PNP0C0B"))
   Name(_UID, 9)
   Name(_PR0, Package() { FN5L, FN5H })
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

    // Thermal Object: Passive
    Method(_PSV, 0) {
      Return (TRPP)
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

    Method(_AC0, 0, Serialized) { Return(TRP0) }
    Method(_AC1, 0, Serialized) { Return(TRP1) }
    Name(_AL0, Package() { FAN1, FAN3, FAN5, FAN7, FAN9 })
    Name(_AL1, Package() { FAN0, FAN2, FAN4, FAN6, FAN8 })

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
