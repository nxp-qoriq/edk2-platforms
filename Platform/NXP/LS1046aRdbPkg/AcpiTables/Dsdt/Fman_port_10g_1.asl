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
  Device(MAC5) {
    Name(_HID, "NXP0025")
    Name(_UID, 9)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1af2000, 0x1000)
    }) // end of _CRS for MAC9
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"mac-address", Package (6) {11, 22, 33, 44, 55, 66}},
        Package () {"cell-index", 0x9},
        Package () {"fixed-link", Package () {0, 1, 1000, 0, 0}},
        Package () {"phy-connection-type", "xgmii"},
        Package () {"compatible", "fman-memac"},
        Package () {"fsl,fman-ports", Package () {\_SB.FMN0.PRX9, \_SB.FMN0.PTX9}},
        Package () {"pcsphy-handle", \_SB.FMN0.MDI9.PCS9}
      }
    })
  }

  Device(MDI9) {
    Name(_HID, "NXP0006")
    Name(_UID, 9)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1af3000, 0x1000)
    }) // end of _CRS for MDI9

    Device(PCS9) {
      Name (_ADR, 0x0)
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"reg", 0x0}
        }
      })
    } // end of PCS9
  } // end of MDI9

  Device(PRX9) {
    Name(_HID, "NXP0026")
    Name(_UID, 10)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1a91000, 0x1000)
    }) // end of _CRS for PRX9
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0x11},
        Package () {"compatible", "fman-v3-port-rx"},
        Package () {"fsl,fman-10g-port", 1}
      }
    })
  } // PRX9 end

  Device(PTX9) {
    Name(_HID, "NXP0026")
    Name(_UID, 11)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1ab1000, 0x1000)
    }) // end of _CRS for PTX9
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0x31},
        Package () {"fsl,fman-10g-port", 1},
        Package () {"compatible", "fman-v3-port-tx"},
        Package () {"fsl,qman-channel-id", 0x801}
      }
    })
  } // PTX9 end
}
