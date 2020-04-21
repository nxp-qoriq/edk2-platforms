/** @file
*  DSDT: COM0 Device ACPI Table entry
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
    Device(COM0) {
      Name(_HID, "NXP0018") //_HID:HardwareID
      Name(_CRS, ResourceTemplate() { //_CRS:CurrentResourceSettings
        Memory32Fixed(ReadWrite, UART0_BASE, UART0_LENGTH)
        Interrupt(ResourceConsumer,Level,ActiveHigh,Exclusive,,,) { UART0_IT }
      })
      Name (CLCK, 0x11e1a300) //300000000
      Name (_DSD, Package () {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () { Package (2) { "clock-frequency", CLCK }, }
      })
     }
}
