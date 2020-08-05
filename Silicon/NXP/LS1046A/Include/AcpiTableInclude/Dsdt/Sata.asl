/** @file
*  DSDT : SATA ACPI Information
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Device(SAT0) {
    Name(_HID, "NXP0004")
    Name(_CCA, 0)
    Name(_UID, 0)
    Name (_CLS, Package (0x03)  // _CLS: Class Code
    {
      0x01,
      0x06,
      0x01
    })
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, SATA0_BASE, SATA_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {SATA0_IT_1}
      QwordMemory (
        ResourceConsumer,
        PosDecode,          // _DEC
        MinFixed,           // _MIF
        MaxFixed,           // _MAF
        NonCacheable,       // _MEM
        ReadWrite,          // _RW
        0,                  // _GRA
        0x20140520,         // _MIN MinAddress
        0x20140523,         // _MAX MaxAddress
        0,                  // _TRA
        0x4,                // _LEN
      ,)
    })
  }
}
