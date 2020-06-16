/** @file
*
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Derived from:
*   LX2160aRdbPkg/AcpiTables/Dsdt/Rcpm.asl
*
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
         Package () {"#fsl,rcpm-wakeup-cells", 1},
      }
    })
  } // end of Rcpm device
}
