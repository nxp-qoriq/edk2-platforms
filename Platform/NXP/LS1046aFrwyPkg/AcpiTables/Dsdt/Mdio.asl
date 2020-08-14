/** @file
*  DSDT : MDIO Devices
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <AcpiTableInclude/Dsdt/Mdio.asl>

Scope(\_SB.FMN0.MDI0)
{
  Name(_CRS, ResourceTemplate() {
    Memory32Fixed(ReadWrite, MDIO_1_REG_BASE, MDIO_REG_BASE_SZ)
  })

  Device(PHY1) {
    Name (_ADR, 0x1c)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 0x1c},
      }
    })
  } // end of PHY1

  Device(PHY2) {
    Name (_ADR, 0x1d)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 0x1d},
      }
    })
  } // end of PHY2

  Device(PHY3) {
    Name (_ADR, 0x1e)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 0x1e},
      }
    })
  } // end of PHY3

  Device(PHY4) {
    Name (_ADR, 0x1f)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 0x1f},
      }
    })
  } // end of PHY4
} // end of MDI0

Scope(\_SB.FMN0.MDI1)
{
  Method (_STA, 0, Serialized)  // _STA: Status
  {
    Return (Zero)
  }
}
