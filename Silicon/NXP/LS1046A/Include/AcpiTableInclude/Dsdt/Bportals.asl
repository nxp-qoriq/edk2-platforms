/** @file
*  DSDT : BMan Portals ACPI Information
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Device(BPR0) {
    Name(_HID, "NXP0023")
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL1_MIN_ADD1, BMAN_PORTAL1_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL1_MIN_ADD2, BMAN_PORTAL1_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ1}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR1) {
    Name(_HID, "NXP0023")
    Name(_UID, 1)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL2_MIN_ADD1, BMAN_PORTAL2_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL2_MIN_ADD2, BMAN_PORTAL2_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ2}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR2) {
    Name(_HID, "NXP0023")
    Name(_UID, 2)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL3_MIN_ADD1, BMAN_PORTAL3_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL3_MIN_ADD2, BMAN_PORTAL3_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ3}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR3) {
    Name(_HID, "NXP0023")
    Name(_UID, 3)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL4_MIN_ADD1, BMAN_PORTAL4_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL4_MIN_ADD2, BMAN_PORTAL4_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ4}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR4) {
    Name(_HID, "NXP0023")
    Name(_UID, 4)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL5_MIN_ADD1, BMAN_PORTAL5_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL5_MIN_ADD2, BMAN_PORTAL5_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ5}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR5) {
    Name(_HID, "NXP0023")
    Name(_UID, 5)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL6_MIN_ADD1, BMAN_PORTAL6_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL6_MIN_ADD2, BMAN_PORTAL6_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ6}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR6) {
    Name(_HID, "NXP0023")
    Name(_UID, 6)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL7_MIN_ADD1, BMAN_PORTAL7_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL7_MIN_ADD2, BMAN_PORTAL7_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ7}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR7) {
    Name(_HID, "NXP0023")
    Name(_UID, 7)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL8_MIN_ADD1, BMAN_PORTAL8_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL8_MIN_ADD2, BMAN_PORTAL8_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ8}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR8) {
    Name(_HID, "NXP0023")
    Name(_UID, 8)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL9_MIN_ADD1, BMAN_PORTAL9_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL9_MIN_ADD2, BMAN_PORTAL9_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ9}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device

  Device(BPR9) {
    Name(_HID, "NXP0023")
    Name(_UID, 9)
    Name(_CRS, ResourceTemplate() {
      DPAA_PORTAL(BMAN_PORTAL10_MIN_ADD1, BMAN_PORTAL10_MAX_ADD1)
      DPAA_PORTAL(BMAN_PORTAL10_MIN_ADD2, BMAN_PORTAL10_MAX_ADD2)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {BMAN_PORTAL_IRQ10}
    }) // end of _CRS for bportal device
  } // end of BMan Portal device
}
