/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2019-2020 NXP
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <AcpiTablesInclude/Dsdt/Mdio.asl>

Scope(\_SB.MDI0)
{
  Device(PHY1) {
    Name (_ADR, 0x1)
  } // end of PHY1

  Device(PHY2) {
    Name (_ADR, 0x2)
  } // end of PHY2

  Device(PHY4) {
    Name (_ADR, 0x4)
    Name(_CRS, ResourceTemplate() {
    Interrupt(ResourceConsumer, Level, ActiveHigh, Shared)
    {
      AQR_PHY4_IT
    }
    }) // end of _CRS for PHY4
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"compatible", "ethernet-phy-ieee802.3-c45"}
       }
    })
  } // end of PHY4

  Device(PHY5) {
    Name (_ADR, 0x5)
    Name(_CRS, ResourceTemplate() {
    Interrupt(ResourceConsumer, Level, ActiveHigh, Shared)
    {
      AQR_PHY5_IT
    }
    }) // end of _CRS for PHY5
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"compatible", "ethernet-phy-ieee802.3-c45"}
       }
    })
  } // end of PHY5
} // end of MDI0

Scope(\_SB.MDI1)
{
  Device(PHY0) {
    Name (_ADR, 0x0)
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"compatible", "ethernet-phy-id0210.7440"}
       }
    })
  } // end of PHY0 Inphi

} // end of MDI1
