/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
  Copyright 2017-2018, 2020 NXP
  Copyright 2020 Puresoftware Ltd

  SPDX-License-Identifier: BSD-2-Clause-Patent

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/
Scope(_SB)
{
  Device(FPI0) {
    Name(_HID, "NXP0009")
    Name(_UID, 0)
    Method (_STA) {
      Return (0x01)
    }
    Name(_CRS, ResourceTemplate() {
      Memory32Fixed(ReadWrite, FSPI0_BASE, FSPI_LEN,)
      Memory32Fixed(ReadWrite, FSPIMM_BASE, FSPIMM_LEN,)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { FSPI_IT }
    }) // end of _CRS for flex spi device

    Name(RBF1, ResourceTemplate() {
      SpiSerialBus(0x0000, PolarityLow, FourWireMode, 0x08, ControllerInitiated, 0x2FAF080,
                   ClockPolarityLow, ClockPhaseFirst, "\\_SB.FPI0", 0x00, ResourceConsumer, ,)
    })

    Name(RBF2, ResourceTemplate() {
      SpiSerialBus(0x0001, PolarityLow, FourWireMode, 0x08, ControllerInitiated, 0x2FF080,
                   ClockPolarityLow, ClockPhaseFirst, "\\_SB.FPI0", 0x00, ResourceConsumer, ,)
    })

    Device(MTD0) {
      Name(_HID, "PRP0001")
      Name(_UID, 0)
      Method(_CRS, 0, Serialized) {
        Return (RBF1)
      }
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "micron,m25p80"},
        }
      })
    } // end of mtd 0 device

    Device(MTD1) {
      Name(_HID, "PRP0001")
      Name(_UID, 1)
      Method(_CRS, 0, Serialized) {
        Return (RBF2)
      }
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package() {
          Package() {"compatible", "micron,m25p80"},
        }
      })
    } // end of mtd 1 device
  } // end of Flex SPI device
}
