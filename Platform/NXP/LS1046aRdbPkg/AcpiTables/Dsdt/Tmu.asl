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
    offset(0x01),                      // ADT7461 remote sensor temperature register offset
    AccessAs (BufferAcc, AttribByte),
    FLD0, 8                            // Virtual register at command value 0x01
  }

  Name(BUFF, Buffer(34){})
  CreateByteField(BUFF, 0x00, STAT)
  CreateByteField(BUFF, 0x01, LEN)
  CreateByteField(BUFF, 0x02, DATA)

  // Method to read temerature from remote sensor
  Method (STMP, 0, Serialized) {
    Store(Zero, Local0)
    If (LEqual (\_SB.I2C0.AVBL, One)) {
      Store(One, LEN)
      Store(FLD0, BUFF)
      Local0 = DATA
    }
    Return (Local0)
  }
}

#include <AcpiTableInclude/Dsdt/Tmu.asl>

Scope(\_TZ)
{
  // Thermal constants
  Name(TRP1, TMU_PASSIVE_THRESHOLD)

  // ThermalZone for sensor near Arm A72 core
  Scope(\_TZ.THM3) {
    Method(_PSV, 0) {
      Return (TRP1)
    }
  }

} //end of Scope _TZ
