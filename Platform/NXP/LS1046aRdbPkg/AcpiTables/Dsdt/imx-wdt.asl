/** @file
*  DSDT: Watchdog Table (non-sbsa compliant) NXP specific IP
*
*  Copyright 2019-2020 NXP
*
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
Scope(_SB)
{
    Device(WDT0) {
      Name(_HID, "NXP0019") //_HID:HardwareID
      Name(_CRS, ResourceTemplate() { //_CRS:CurrentResourceSettings
        Memory32Fixed(ReadWrite, WDT0_BASE, WDT0_LENGTH,)
        Interrupt(ResourceConsumer,Level,ActiveHigh,Exclusive,,,) { WDT0_IT }
      })
     }
}
