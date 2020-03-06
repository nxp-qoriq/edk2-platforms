/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2019-2020 NXP
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Scope(_SB)
{
  Device(MCE0) {
    Name(_HID, "NXP0008")
    Name(_CCA, 1)
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      QwordMemory (
        ResourceConsumer,
        PosDecode,      // _DEC
        MinFixed,       // _MIF
        MaxFixed,       // _MAF
        NonCacheable,   // _MEM
        ReadWrite,      // _RW
        0,              // _GRA
        0x80c000000,    // _MIN MinAddress
        0x80c00003f,    // _MAX MaxAddress
        0,              // _TRA
        0x40,           // _LEN
          ,)
      Memory32Fixed(ReadWrite, 0x08340000, 0x40000)
    }) // end of _CRS for fsl-mc device

    //DPMACs
    Device(PR03) { // 10G
      Name (_ADR, 0x3)
    }
     Device(PR04) { // 10G
      Name (_ADR, 0x4)
    }
    Device(PR05) { // 25G
      Name (_ADR, 0x5)
    }
    Device(PR06) { // 25G
      Name (_ADR, 0x6)
    }
    Device(PR17) { // 1G
      Name (_ADR, 0x11)
    }
    Device(PR18) { // 1G
      Name (_ADR, 0x12)
    } // end of DPMAC
  } // end of fsl-mc device
} // end of fsl-mc controller
