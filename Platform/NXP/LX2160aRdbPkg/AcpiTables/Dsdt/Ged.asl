/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*  Copyright 2019 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause
*
*  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/
*
**/

Scope(_SB)
{
    //@add methos for GED for Pci root port zero.
 Device (HED0)
 {
    Name (_HID, "PNP0C33")
    Name (_UID, 0)
 }
 Device (GED0)
  {
    Name (_HID, "ACPI0013")
    Name (_UID, 0)
    Name (_CRS, ResourceTemplate (){
         Interrupt(ResourceConsumer, Level, ActiveHigh, Shared){151}
       })

    Method (_EVT, 1) {
     if (Lequal(151, Arg0))
      {
	Notify (HED0, 0x80)
      }
     }
    }
}
