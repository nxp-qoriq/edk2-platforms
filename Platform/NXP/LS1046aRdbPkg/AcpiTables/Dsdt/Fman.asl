/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Device(FMN0) {
    Name(_HID, "NXP0024")
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, FMAN_REG_BASE, FMAN_REG_BASE_SZ)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {FMAN_IRQ1, FMAN_IRQ2}
    }) // end of _CRS for FMAN0
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"little-endian", 1},
        Package () {"cell-index", 0},
        Package () {"fsl,qman-channel-range", Package () {QMAN_CHANNEL_BASE, QMAN_CHANNEL_BASE_SZ}},
        Package () {"clock-frequency", ^PCLK.CLK},
        Package () {"ptimer-handle", \_SB.PTP0},
      }
    })

    Device(CC0) {
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-cc"},
        }
      })
    } // end of CC0 device

    Device(MRM0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_MURAM_BASE, FMAN_MURAM_BASE_SZ)
      }) // end of _CRS for MURAM0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-muram"},
          Package() {"reg", 0x60000},
        }
      })
    } // end of MURAM0 device

    Device(BMI0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_BMI_BASE, FMAN_BMI_BASE_SZ)
      }) // end of _CRS for BMI0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-bmi"},
        }
      })
    } // end of BMI0 device

    Device(QMI0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_QMI_BASE, FMAN_QMI_BASE_SZ)
      }) // end of _CRS for QMI0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-qmi"},
        }
      })
    } // end of QMI0 device

    Device(PLY0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_PLICER_BASE, FMAN_PLICER_BASE_SZ)
      }) // end of _CRS for PLY0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-policer"},
        }
      })
    } // end of PLY0 device

    Device(KYG0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_KEYGEN_BASE, FMAN_KEYGEN_BASE_SZ)
      }) // end of _CRS for KYG0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-keygen"},
        }
      })
    } // end of KYG0 device

    Device(DMA0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_DMA_BASE, FMAN_DMA_BASE_SZ)
      }) // end of _CRS for DMA0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-dma"},
        }
      })
    } // end of DMA0 device

    Device(FPM0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_FPM_BASE, FMAN_FPM_BASE_SZ)
      }) // end of _CRS for FPM0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-fpm"},
        }
      })
    } // end of FPM0 device

    Device(PRS0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_PARSER_BASE, FMAN_PARSER_BASE_SZ)
      }) // end of _CRS for PRS0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-parser"},
        }
      })
    } // end of PRS0 device

    Device(VSP0) {
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, FMAN_VSP_BASE, FMAN_VSP_BASE_SZ)
      }) // end of _CRS for VSP0
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "fsl,fman-vsps"},
        }
      })
    } // end of VSP0 device
  } // end of FMN0 device

  Device(PTP0) {
    Name(_HID, "NXP0027")
    Name(_UID, 0)
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, PTP_CLK_BASE, PTP_CLK_BASE_SZ)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {PTP_CLK_IRQ}
    }) // end of _CRS for PTP0
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package () {"clock-frequency", ^PCLK.CLK},
      }
    }) // end of _DSD for PTP0
  } // end of PTP0
} // end of _SB
