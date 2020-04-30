/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

Scope(_SB)
{
  Device(GUT0) {
    Name(_HID, "NXP0030")
    Name(_CCA, 1)
    Name(_UID, 0)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"little-endian", 0},
          Package () {"model", "NXP Layerscape LX2160ARDB"},
      }
    })
    Name(_CRS, ResourceTemplate() {
     Memory32Fixed(ReadWrite, 0x01e00000, 0x10000)
    }) // end of _CRS for guts device
   } // end of guts device
} // end of guts hardware block

