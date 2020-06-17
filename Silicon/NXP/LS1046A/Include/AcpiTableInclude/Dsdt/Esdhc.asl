/** @file
*  DSDT : ESDHC ACPI Information
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Device(SDC0) {
    Name(_HID, "NXP0003")
    Name(_CID, "PNP0D40")
    Name(_CCA, 1)
    Name(_UID, 0)
    Name(CLK, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, SDC0_BASE, SDC_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {SDC0_IT}
    })
    Method(_INI, 0, NotSerialized) {
      Store(\_SB.PCLK.CLK, CLK)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
         Package () {"clock-frequency", CLK},
         Package () {"little-endian", 0},
         Package () {"voltage-ranges", Package() {1800, 1800, 3300, 3300}},
         Package () {"sdhci,auto-cmd12", 1},
         Package () {"bus-width", 4},
         Package () {"sd-uhs-sdr104", 1},
         Package () {"sd-uhs-sdr50", 1},
         Package () {"sd-uhs-sdr25", 1},
         Package () {"sd-uhs-sdr12", 1},
      }
    })
  }
}
