/** @file
*  Differentiated System Description Table Fields (DSDT)
*  Add I2C Support
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

/*
  i2c connections on this board and users
  I2C1
  Slaves - EEPROM, Thermal Monitor,DDR SPD,
           Clock Generator, Current Monitor,
           retimer

  I2C2
  Slaves - RTC

Most of devices are used for boot, except few to be
exposed to OS
*/

Scope(_SB)
{
  Device(I2C0) {
    Name(_HID, "NXP0001")
    Name(_UID, 0)
    Name(CLK, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, I2C0_BASE, I2C_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { I2C0_IT }
    }) // end of _CRS for i2c0 device
    Method(_INI, 0, NotSerialized) {
      Store(\_SB.PCLK.CLK, CLK)
      Divide(CLK, 2, Local0, CLK)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"clock-frequency", CLK},
      }
    })

    Device(POW1) {
      Name (_HID, "PRP0001")
      Name(_CRS, ResourceTemplate() {
        I2CSerialBus(0x40, ControllerInitiated, 100000, AddressingMode7Bit, "\\_SB.I2C0", 0, ResourceConsumer, ,)
      })
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "ti,ina220"},
          Package() {"shunt-resister", 0x3e8}, //shunt resistor value is 1000mohm
        }
      })
    } //end of power monitor device

    Device(THE1) {
      NAME(_HID, "PRP0001")
      Name(_CRS, ResourceTemplate() {
        I2CSerialBus(0x4C, ControllerInitiated, 100000, AddressingMode7Bit, "\\_SB.I2C0", 0, ResourceConsumer, ,)
      }) // end of CRS for thermal monitor device
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "adi,adt7461"},
        }
      })
    } //end of thermal monitor device
  } // end of i2c device

  Device(I2C3) {
    Name(_HID, "NXP0001")
    Name(_UID, 3)
    Name(CLK, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, I2C3_BASE, I2C_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { I2C3_IT }
    }) // end of _CRS for i2c3 device
    Method(_INI, 0, NotSerialized) {
      Store(\_SB.PCLK.CLK, CLK)
      Divide(CLK, 2, Local0, CLK)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"clock-frequency", CLK},
      }
    })
  } // end of i2c device
} // end of i2c controllers
