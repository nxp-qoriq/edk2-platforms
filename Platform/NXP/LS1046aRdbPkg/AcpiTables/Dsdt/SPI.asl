/** @file
  (DSDT) : SPI ACPI information

  Copyright 2020 NXP
  Copyright 2020 Puresoftware Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under Arm/JunoPkg/AcpiTables/

**/
Scope(_SB)
{
  Device(SPI0) {
    Name(_HID, "NXP0005")
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, SPI0_BASE, SPI_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) { SPI_IT }
    }) // end of _CRS for spi device
    Name (_DSD, Package () {
      ToUUID(SPI_UUID),
      Package () {
        Package () {"clock-frequency", ^PCLK.CLK},
        Package () {"spi-num-chipselects", 4},
        Package () {"bus-num", 0},
      }
    }) // end of DSD SPI device
  } // end of SPI device
}
