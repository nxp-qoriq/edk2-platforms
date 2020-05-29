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
  Device(MAC0) {
    Name(_HID, "NXP0025")
      Name(_UID, 0)
      Name(_CCA, 1) // Cache Coherency Attribute
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x1ae0000, 0x1000)
      }) // end of _CRS for MAC0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"mac-address", Package (6) {11, 22, 33, 44, 55, 66}},
          Package () {"cell-index", 0x0},
          Package () {"phy-handle", \_SB.FMN0.MDI0.PHY4},
          Package () {"phy-connection-type", "qsgmii"},
          Package () {"compatible", "fman-memac"},
          Package () {"fsl,fman-ports", Package () {\_SB.FMN0.PRX2, \_SB.FMN0.PTX2}},
          Package () {"pcsphy-handle", \_SB.FMN0.MDI2.PCS2}
        }
      }) // end of _DSD for MAC0
  } // end of MAC0

  Device(MDI2) {
    Name(_HID, "NXP0006")
    Name(_UID, 2)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1ae1000, 0x1000)
    }) // end of _CRS for MDI2

    Device(PCS2) {
      Name (_ADR, 0x0)
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"reg", 0x0}
        }
      })
    } // end of PCS2
  } // end of MDI2

  Device(PRX2) {
    Name(_HID, "NXP0026")
    Name(_UID, 0)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1a88000, 0x1000)
    }) // end of _CRS for PRX2
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0x8},
        Package () {"compatible", "fman-v3-port-rx"}
      }
    })
  } // PRX2 end

  Device(PTX2) {
    Name(_HID, "NXP0026")
    Name(_UID, 1)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1aa8000, 0x1000)
    }) // end of _CRS for PTX2
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0x28},
        Package () {"compatible", "fman-v3-port-tx"},
        Package () {"fsl,qman-channel-id", 0x802}
      }
    })
  } // PTX2 end
}
