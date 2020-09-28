/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2019-2020 NXP
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <AcpiTablesInclude/Dsdt/Mc.asl>

Scope(\_SB.MCE0.PR03) // 10G
{
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) {"phy-mode", "usxgmii"},
            Package (2) {"phy-handle",\_SB.MDI0.PHY4}
    }
  })
}

Scope(\_SB.MCE0.PR04) // 10G
{
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) {"phy-mode", "usxgmii"},
            Package (2) {"phy-handle",\_SB.MDI0.PHY5}
   }
  })
}

Scope(\_SB.MCE0.PR17) // 1G
{
  Name (_DSD, Package () {
     ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
         Package () {
             Package (2) {"phy-mode", "rgmii-id"},
             Package (2) {"phy-handle",\_SB.MDI0.PHY1}
      }
   })
}

Scope(\_SB.MCE0.PR18) // 1G
{
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) {"phy-mode", "rgmii-id"},
             Package (2) {"phy-handle",\_SB.MDI0.PHY2}
    }
  })
}
