/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Device(BMN0) {
    Name(_HID, "NXP0021")
    Name(_UID, 0)
    Name(_CCA, 1)
    //TODO: Reserved Memory Region Handling
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, BMAN_REG_BASE, BMAN_REG_BASE_SZ)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QBMAN_IRQ }
    }) // end of _CRS for bman device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"mem-base", Package (1) {BMAN_PRIVATE_MEM_AL}}, // mem-base will be fixed by firmware
        Package () {"mem-align", Package (1) {BMAN_PRIVATE_MEM_AL}},
        Package () {"size", Package (1) {BMAN_PRIVATE_MEM_SZ}}
      }
    }) // end of DSD QMan Portal device
  } // end of BMAN device
}
