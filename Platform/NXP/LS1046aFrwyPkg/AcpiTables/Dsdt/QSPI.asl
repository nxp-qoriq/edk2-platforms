/** @file
*  (DSDT) : QSPI (Quad SPI)
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Device(QPI0) {
    Name(_HID, "NXP0020")
    Name(_UID, 0)
    Method (_STA) {
      Return (QSPI_STATUS) //QSPI hardware resource disabled due to requirement in runtime services, to enable change QSPI_STATUS to 0x03
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
                         ClockPolarityLow, ClockPhaseFirst, "\\_SB.QPI0", 0x00, ResourceConsumer, ,)
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
  } // end of QSPI device
}
