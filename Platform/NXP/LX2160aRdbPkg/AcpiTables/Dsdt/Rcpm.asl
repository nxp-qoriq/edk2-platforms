/** @file

  Differentiated System Description Table Fields (DSDT)

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

  Derived from:
   ArmPlatformPkg/ArmJunoPkg/AcpiTables/Dsdt.asl

**/

Scope(_SB)
{
  Device(RCPM) {
    Name(_HID, "NXP0015")
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, RCPM_BASE, RCPM_LEN)
    }) // end of _CRS for Rcpm device

    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
         Package () {"little-endian", 1},
         Package () {"#fsl,rcpm-wakeup-cells", 7},
      }
    })
  } // end of Rcpm device
}
