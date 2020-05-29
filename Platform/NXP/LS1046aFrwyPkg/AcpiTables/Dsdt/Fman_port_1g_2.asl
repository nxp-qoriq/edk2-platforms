/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(\_SB.FMN0)
{
  Device(MAC2) {
    Name(_HID, "NXP0025")
    Name(_UID, 2)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1aea000, 0x1000)
    }) // end of _CRS for MAC2
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"mac-address", Package (6) {11, 22, 33, 44, 55, 66}},
        Package () {"cell-index", 0x5},
        Package () {"phy-handle", \_SB.FMN0.MDI0.PHY1},
        Package () {"phy-connection-type", "qsgmii"},
        Package () {"compatible", "fman-memac"},
        Package () {"fsl,fman-ports", Package () {\_SB.FMN0.PRX4, \_SB.FMN0.PTX4}},
        Package () {"pcsphy-handle", \_SB.FMN0.MDI4.PCS4}
      }
    })
  }

  Device(MDI4) {
    Name(_HID, "NXP0006")
    Name(_UID, 4)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1aeb000, 0x1000)
    }) // end of _CRS for MDI4

    Device(PCS4) {
      Name (_ADR, 0x0)
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"reg", 0x0}
        }
      })
    } // end of PCS4
  } // end of MDI4

  Device(PRX4) {
    Name(_HID, "NXP0026")
    Name(_UID, 4)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1a8d000, 0x1000)
    }) // end of _CRS for PRX4
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0xd},
        Package () {"compatible", "fman-v3-port-rx"}
      }
    })
  } // PRX4 end

  Device(PTX4) {
    Name(_HID, "NXP0026")
    Name(_UID, 5)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1aad000, 0x1000)
    }) // end of _CRS for PTX4
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0x2d},
        Package () {"compatible", "fman-v3-port-tx"},
        Package () {"fsl,qman-channel-id", 0x807}
      }
    })
  } // PTX4 end
}
