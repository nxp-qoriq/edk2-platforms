/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2019-2020 NXP
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(_SB)
{
  Device(MDI0) {
    Name(_HID, "NXP0006")
    Name(_CCA, 1)
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, MDI0_BASE, MDI_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared)
       {
         MDI0_IT
       }
    }) // end of _CRS for MDI0
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
         Package () {"little-endian", 1},
         Package () {"fsl,erratum-a011043", 1},
      }
    })
  } // end of MDI0
  Device(MDI1) {
    Name(_HID, "NXP0006")
    Name(_CCA, 1)
    Name(_UID, 1)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, MDI1_BASE, MDI_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared)
       {
         MDI1_IT
       }
    }) // end of _CRS for MDI1
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
         Package () {"little-endian", 1},
         Package () {"fsl,erratum-a011043", 1},
      }
    })
  }
}
