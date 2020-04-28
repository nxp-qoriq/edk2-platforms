/** @file
*  (DSDT) : QSPI (Quad SPI)
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under Arm/JunoPkg/AcpiTables/
*
**/
Scope(_SB)
{
  Device(QPI0) {
    Name(_HID, "NXP0020")
    Name(_UID, 0)
    Method (_STA) {
      Return (0x01)
    }
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, QSPI0_BASE, QSPI_LEN,)
      Memory32Fixed(ReadWrite, QSPIMM_BASE, QSPIMM_LEN,)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { QSPI_IT }
    }) // end of _CRS for quad spi device

    Device(MTD0) {
      Name(_HID, "PRP0001")
      Name(_UID, 0)
      Method(_CRS, 0, Serialized) {
        Name(RBUF, ResourceTemplate() {
          SpiSerialBus(0x0000, PolarityLow, FourWireMode, 0x04, ControllerInitiated, 0x2FAF080,
                         ClockPolarityLow, ClockPhaseFirst, "\\.SB.QPI0.MTD0", 0x00, ResourceConsumer, ,)
        })
        Return (RBUF)
      }
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "spansion,m25p80"},
        }
      })
    } // end of mtd 0 device

    Device(MTD1) {
      Name(_HID, "PRP0001")
      Name(_UID, 1)
      Method(_CRS, 0, Serialized) {
        Name(RBUF, ResourceTemplate() {
          SpiSerialBus(0x0001, PolarityLow, FourWireMode, 0x04, ControllerInitiated, 0x2FAF080,
                         ClockPolarityLow, ClockPhaseFirst, "\\.SB.QPI0.MTD1", 0x00, ResourceConsumer, ,)
        })
        Return (RBUF)
      }
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "spansion,m25p80"},
        }
      })
    } // end of mtd 1 device
  } // end of QSPI device
}
