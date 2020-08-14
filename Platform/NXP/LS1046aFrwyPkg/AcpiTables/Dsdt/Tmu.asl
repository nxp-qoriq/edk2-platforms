/** @file
*  DSDT : Thermal Management Devices
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
    Switch (ToInteger(Arg0)) {
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
    Switch(ToInteger(Arg0)) {
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
    Switch(ToInteger(Arg0)) {
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
    Switch(ToInteger(Arg0)) {
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
    Switch(ToInteger(Arg0)) {
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

#include <AcpiTableInclude/Dsdt/Tmu.asl>

Scope(\_TZ)
{
  // Thermal constants
  Name(TRPP, TMU_PASSIVE_THRESHOLD)
  Name(TRP0, TMU_ACTIVE_HIGH_THRESHOLD)
  Name(TRP1, TMU_ACTIVE_LOW_THRESHOLD)

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

  Scope(\_TZ.THM3)
  {
    // Thermal Object: Passive
    Method(_PSV, 0) {
      Return (TRPP)
    }
  }

  Scope(\_TZ.THM5)
  {
    Method(_AC0, 0, Serialized) { Return(TRP0) }
    Method(_AC1, 0, Serialized) { Return(TRP1) }
    Name(_AL0, Package() { FAN1, FAN3, FAN5, FAN7, FAN9 })
    Name(_AL1, Package() { FAN0, FAN2, FAN4, FAN6, FAN8 })
  }
}
