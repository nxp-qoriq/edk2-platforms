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
  Device(MAC1) {
    Name(_HID, "NXP0025")
    Name(_UID, 1)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1ae8000, 0x1000)
    }) // end of _CRS for MAC1
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"mac-address", Package (6) {11, 22, 33, 44, 55, 66}},
        Package () {"cell-index", 0x4},
        Package () {"phy-handle", \_SB.FMN0.MDI0.PHY2},
        Package () {"phy-connection-type", "qsgmii"},
        Package () {"compatible", "fman-memac"},
        Package () {"fsl,fman-ports", Package () {\_SB.FMN0.PRX3, \_SB.FMN0.PTX3}},
        Package () {"pcsphy-handle", \_SB.FMN0.MDI3.PCS3}
      }
    })
  } // end of MAC1

  Device(MDI3) {
    Name(_HID, "NXP0006")
    Name(_UID, 3)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1ae9000, 0x1000)
    }) // end of _CRS for MDI3

    Device(PCS3) {
      Name (_ADR, 0x0)
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"reg", 0x0}
        }
      })
    } // end of PCS3
  } // end of MDI3

  Device(PRX3) {
    Name(_HID, "NXP0026")
    Name(_UID, 2)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1a8c000, 0x1000)
    }) // end of _CRS for PRX3
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0xc},
        Package () {"compatible", "fman-v3-port-rx"}
      }
    })
  } // PRX3 end

  Device(PTX3) {
    Name(_HID, "NXP0026")
    Name(_UID, 3)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1aac000, 0x1000)
    }) // end of _CRS for PTX3
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0x2c},
        Package () {"compatible", "fman-v3-port-tx"},
        Package () {"fsl,qman-channel-id", 0x806}
      }
    })
  } // PTX3 end
}
