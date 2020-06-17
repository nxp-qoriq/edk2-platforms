/** @file
*  DSDT : I2C ACPI Information
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

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
  } // end of i2c device

  Device(I2C1) {
    Name(_HID, "NXP0001")
    Name(_UID, 1)
    Name(CLK, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, I2C1_BASE, I2C_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { I2C1_IT }
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
  } // end of i2c device

  Device(I2C2) {
    Name(_HID, "NXP0001")
    Name(_UID, 2)
    Name(CLK, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, I2C2_BASE, I2C_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { I2C2_IT }
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
