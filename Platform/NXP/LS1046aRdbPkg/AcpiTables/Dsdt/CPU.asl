/** @file
*  DSDT: CPU Entries
*  Copyright 2019-2020 NXP
*
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
    //
    // A72x4 Processor core declaration
    //
    Device(CPU0) {
      Name(_HID, "ACPI0007")
      Name(_UID, 0)
    }
    Device(CPU1) {
      Name(_HID, "ACPI0007")
      Name(_UID, 1)
    }
    Device(CPU2) {
      Name(_HID, "ACPI0007")
      Name(_UID, 2)
    }
    Device(CPU3) {
      Name(_HID, "ACPI0007")
      Name(_UID, 3)
    }
}
