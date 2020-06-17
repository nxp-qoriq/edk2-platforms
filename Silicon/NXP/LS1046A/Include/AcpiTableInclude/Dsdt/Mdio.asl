/** @file
*  DSDT : MDIO ACPI Information
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(\_SB.FMN0)
{
  Device(MDI0) {
    Name(_HID, "NXP0006")
    Name(_UID, 0)

    Device(PHY1) {
    }

    Device(PHY2) {
    }

    Device(PHY3) {
    }

    Device(PHY4) {
    }

  } // end of MDI0

  Device(MDI1) {
    Name(_HID, "NXP0006")
    Name(_UID, 1)

    Device(PHY0) {
    }

  } //end of MDI1
}
