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
            Package () {"reg", 3},
            Package () {"phy-connection-type", "xgmii"},
            Package () {"phy-handle", Package (){\_SB.MDI0.PHY4}}
    }
  })
}

Scope(\_SB.MCE0.PR04) // 10G
{
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () {"reg", 4},
            Package () {"phy-connection-type", "xgmii"},
            Package () {"phy-handle", Package (){\_SB.MDI0.PHY5}}
    }
  })
}

Scope(\_SB.MCE0.PR05) // 25G
{
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () {"reg", 5},
            Package () {"phy-handle", Package (){\_SB.MDI1.PHY0}}
    }
  })
}

Scope(\_SB.MCE0.PR06) // 25G
{
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () {"reg", 6},
            Package () {"phy-handle", Package (){\_SB.MDI1.PHY0}}
     }
  })
}

Scope(\_SB.MCE0.PR17) // 1G
{
  Name (_DSD, Package () {
     ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
         Package () {
             Package () {"reg", 17},
             Package () {"phy-connection-type", "rgmii-id"},
             Package () {"phy-handle", Package (){\_SB.MDI0.PHY1}}
      }
   })
}

Scope(\_SB.MCE0.PR18) // 1G
{
  Name (_DSD, Package () {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () {"reg", 18},
            Package () {"phy-connection-type", "rgmii-id"},
            Package () {"phy-handle", Package (){\_SB.MDI0.PHY2}}
    }
  })
}
