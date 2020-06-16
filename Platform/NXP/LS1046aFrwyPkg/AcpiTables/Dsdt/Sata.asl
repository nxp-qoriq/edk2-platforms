/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
  Copyright 2017-2018 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

Scope(_SB)
{
  Device(SAT0) {
    Name(_HID, "NXP0004")
    Name(_CCA, 1)
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, SATA0_BASE, SATA_LEN)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive)
       {
         SATA0_IT_1
       }
    QwordMemory (
      ResourceConsumer,
      PosDecode,         // _DEC
      MinFixed,          // _MIF
      MaxFixed,          // _MAF
      NonCacheable,      // _MEM
      ReadWrite,         // _RW
      0,                 // _GRA
      0x20140520,       // _MIN MinAddress
      0x20140523,       // _MAX MaxAddress
      0,                 // _TRA
      0x4,               // _LEN
      ,)
    })
  }
}
