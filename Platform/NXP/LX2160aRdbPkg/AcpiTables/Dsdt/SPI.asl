/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2017-2018, 2020 NXP
  Copyright 2020 Puresoftware Ltd

  SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/
Scope(_SB)
{
  Device(SPI0) {
    Name(_HID, "NXP0005")
    Name(_UID, 0)
    Name(CLK, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, SPI0_BASE, SPI_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { SPI_IT }
    }) // end of _CRS for spi device
    Method(_INI, 0, NotSerialized) {
      Store(\_SB.PCLK.CLK, CLK)
      Divide(CLK, 4, , CLK)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"clock-frequency", CLK},
        Package () {"spi-num-chipselects", 4},
        Package () {"bus-num", 0},
      }
    }) // end of DSD SPI device
  } // end of SPI device

  Device(SPI1) {
    Name(_HID, "NXP0005")
    Name(_UID, 1)
    Name(CLK, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, SPI1_BASE, SPI_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { SPI_IT }
    }) // end of _CRS for spi device
    Method(_INI, 0, NotSerialized) {
      Store(\_SB.PCLK.CLK, CLK)
      Divide(CLK, 4, , CLK)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"clock-frequency", CLK},
        Package () {"spi-num-chipselects", 4},
        Package () {"bus-num", 1},
      }
    }) // end of DSD SPI device
  } // end of SPI device

  Device(SPI2) {
    Name(_HID, "NXP0005")
    Name(_UID, 2)
    Name(CLK, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, SPI2_BASE, SPI_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { SPI_IT }
    }) // end of _CRS for spi device
    Method(_INI, 0, NotSerialized) {
      Store(\_SB.PCLK.CLK, CLK)
      Divide(CLK, 4, , CLK)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"clock-frequency", CLK},
        Package () {"spi-num-chipselects", 4},
        Package () {"bus-num", 2},
      }
    }) // end of DSD SPI device
  } // end of SPI device
}
