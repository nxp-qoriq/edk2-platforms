/** @file
*  Differentiated System Description Table Fields (DSDT)
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
    Memory32Fixed(ReadWrite, MDIO_0_REG_BASE, MDIO_REG_BASE_SZ)
  })

  Device(PHY1) {
    Name (_ADR, 0x1)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 1},
      }
    })
  } // end of PHY1

  Device(PHY2) {
    Name (_ADR, 0x2)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 2},
      }
    })
  } // end of PHY2

  Device(PHY3) {
    Name (_ADR, 0x3)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 3},
      }
    })
  } // end of PHY3

  Device(PHY4) {
    Name (_ADR, 0x4)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 4},
      }
    })
  } // end of PHY4
} // end of MDI0

Scope(\_SB.FMN0.MDI1) {
  Name(_CRS, ResourceTemplate() {
    Memory32Fixed(ReadWrite, MDIO_1_REG_BASE, MDIO_REG_BASE_SZ)
  })

  Device(PHY0) {
    Name (_ADR, 0x0)
    Name(_CRS, ResourceTemplate() {
      Interrupt(ResourceConsumer, Level, ActiveHigh, Shared) {PHY_C45_10G_IRQ}
    }) // end of _CRS for PHY0
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"reg", 0},
        Package () {"compatible", "ethernet-phy-ieee802.3-c45"}
      }
    })
  } // end of PHY0
} //end of MDI1
