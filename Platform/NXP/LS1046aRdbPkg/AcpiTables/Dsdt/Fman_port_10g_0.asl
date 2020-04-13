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
  Device(MAC4) {
    Name(_HID, "NXP0025")
    Name(_UID, 8)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1af0000, 0x1000)
    }) // end of _CRS for MAC8
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"mac-address", Package (6) {11, 22, 33, 44, 55, 66}},
        Package () {"cell-index", 0x8},
        Package () {"phy-handle", \_SB.FMN0.MDI1.PHY0},
        Package () {"phy-connection-type", "xgmii"},
        Package () {"compatible", "fman-memac"},
        Package () {"fsl,fman-ports", Package () {\_SB.FMN0.PRX8, \_SB.FMN0.PTX8}},
        Package () {"pcsphy-handle", \_SB.FMN0.MDI8.PCS8}
      }
    })
  }

  Device(MDI8) {
    Name(_HID, "NXP0006")
    Name(_UID, 8)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1af1000, 0x1000)
    }) // end of _CRS for MDI8

    Device(PCS8) {
      Name (_ADR, 0x0)
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
          Package () {"reg", 0x0}
        }
      })
    } // end of PCS8
  } // end of MDI8

  Device(PRX8) {
    Name(_HID, "NXP0026")
    Name(_UID, 8)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1a90000, 0x1000)
    }) // end of _CRS for PRX8
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0x10},
        Package () {"compatible", "fman-v3-port-rx"},
        Package () {"fsl,fman-10g-port", 1}
      }
    })
  } // PRX8 end

  Device(PTX8) {
    Name(_HID, "NXP0026")
    Name(_UID, 9)
    Name(_CCA, 1) // Cache Coherency Attribute
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x1ab0000, 0x1000)
    }) // end of _CRS for PTX8
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"cell-index", 0x30},
        Package () {"compatible", "fman-v3-port-tx"},
        Package () {"fsl,fman-10g-port", 1},
        Package () {"fsl,qman-channel-id", 0x800}
      }
    })
  } // PTX8 end
}
