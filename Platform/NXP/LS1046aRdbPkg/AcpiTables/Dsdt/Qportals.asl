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
  Device(QPR0) {
    Name(_HID, "NXP0022")
    Name(_UID, 0)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL1_MIN_ADD1,QMAN_PORTAL1_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL1_MIN_ADD2,QMAN_PORTAL1_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ1 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR1) {
    Name(_HID, "NXP0022")
    Name(_UID, 1)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL2_MIN_ADD1,QMAN_PORTAL2_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL2_MIN_ADD2,QMAN_PORTAL2_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ2 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 1},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR2) {
    Name(_HID, "NXP0022")
    Name(_UID, 2)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL3_MIN_ADD1,QMAN_PORTAL3_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL3_MIN_ADD2,QMAN_PORTAL3_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ3 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 2},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR3) {
    Name(_HID, "NXP0022")
    Name(_UID, 3)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL4_MIN_ADD1,QMAN_PORTAL4_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL4_MIN_ADD2,QMAN_PORTAL4_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ4 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 3},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR4) {
    Name(_HID, "NXP0022")
    Name(_UID, 4)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL5_MIN_ADD1,QMAN_PORTAL5_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL5_MIN_ADD2,QMAN_PORTAL5_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ5 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 4},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR5) {
    Name(_HID, "NXP0022")
    Name(_UID, 5)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL6_MIN_ADD1,QMAN_PORTAL6_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL6_MIN_ADD2,QMAN_PORTAL6_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ6 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 5},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR6) {
    Name(_HID, "NXP0022")
    Name(_UID, 6)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL7_MIN_ADD1,QMAN_PORTAL7_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL7_MIN_ADD2,QMAN_PORTAL7_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ7 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 6},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR7) {
    Name(_HID, "NXP0022")
    Name(_UID, 7)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL8_MIN_ADD1,QMAN_PORTAL8_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL8_MIN_ADD2,QMAN_PORTAL8_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ8 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 7},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR8) {
    Name(_HID, "NXP0022")
    Name(_UID, 8)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL9_MIN_ADD1,QMAN_PORTAL9_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL9_MIN_ADD2,QMAN_PORTAL9_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ9 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 8},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device

  Device(QPR9) {
    Name(_HID, "NXP0022")
    Name(_UID, 9)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(QMAN_PORTAL10_MIN_ADD1,QMAN_PORTAL10_MAX_ADD1)
      DPAA_PORTAL(QMAN_PORTAL10_MIN_ADD2,QMAN_PORTAL10_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QMAN_PORTAL_IRQ10 }
    }) // end of _CRS for qportal device
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 9},
      }
    }) // end of DSD QMan Portal device
  } // end of QMan Portal device
}
