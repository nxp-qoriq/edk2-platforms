/** @file
*  DSDT : QMan ACPI Information
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Device(QMN0) {
    Name(_HID, "NXP0028")
    Name(_UID, 0)
    Name(_CCA, 1)
    //TODO: Reserved Memory Region Handling
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, QMAN_REG_BASE, QMAN_REG_BASE_SZ)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {QBMAN_IRQ}
    }) // end of _CRS for qman device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"mem-base", Package (2) {QMAN_PRIVATE_MEM1_AL, QMAN_PRIVATE_MEM2_AL}}, // mem-base will be fixed by firmware
        Package () {"mem-align", Package (2) {QMAN_PRIVATE_MEM1_AL, QMAN_PRIVATE_MEM2_AL}},
        Package () {"size", Package (2) {QMAN_PRIVATE_MEM1_SZ, QMAN_PRIVATE_MEM2_SZ}}
      }
    }) // end of DSD QMan Portal device
  } // end of QMAN device
}
