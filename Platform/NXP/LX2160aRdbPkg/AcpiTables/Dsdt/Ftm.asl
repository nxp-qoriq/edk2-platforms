/** @file

  Differentiated System Description Table Fields (DSDT)

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

  Derived from:
   ArmPlatformPkg/ArmJunoPkg/AcpiTables/Dsdt.asl

**/

Scope(_SB)
{
  Device(FTM) {
    Name(_HID, "NXP0014")
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, FTM_BASE, FTM_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { FTM_IT }
    }) // end of _CRS for flex timer device

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
         Package () {"little-endian", 1},
      }
    })
  } // end of Flex Timer device
}
