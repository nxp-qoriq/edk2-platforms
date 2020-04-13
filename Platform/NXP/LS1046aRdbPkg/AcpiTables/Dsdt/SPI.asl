/** @file
  (DSDT) : SPI ACPI information

  Copyright 2020 NXP
  Copyright 2020 Puresoftware Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

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
      Divide(CLK, 2, Local0, CLK)
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
}
