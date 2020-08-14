/** @file
*  DSDT : I2C Devices
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <AcpiTableInclude/Dsdt/I2c.asl>

Scope(\_SB.I2C0)
{
  Device(MUX0) {
    NAME(_HID, "NXP0002")
    Name(_CRS, ResourceTemplate() {
      I2CSerialBus(0x77, ControllerInitiated, 100000, AddressingMode7Bit, "\\_SB.I2C0", 0, ResourceConsumer, ,)
    }) // end of CRS for mux device
    Device (CH00) {
      Name(_ADR, 0)
      Device(POW1) {
        Name (_HID, "PRP0001")
        Name(_CRS, ResourceTemplate() {
          I2CSerialBus(0x40, ControllerInitiated, 100000, AddressingMode7Bit, "\\_SB.I2C0.MUX0.CH00", 0, ResourceConsumer, ,)
        })
        Name (_DSD, Package () {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package() {
            Package() {"compatible", "ti,ina220"},
            Package() {"shunt-resister", 0x3e8}, //shunt resistor value is 1000mohm
          }
        })
      } //end of power monitor device
      Device(THE1) {
        Name (_HID, "PRP0001")
        Name(_CRS, ResourceTemplate() {
          I2CSerialBus(0x4C, ControllerInitiated, 100000, AddressingMode7Bit, "\\_SB.I2C0.MUX0.CH00", 0, ResourceConsumer, ,)
        })
        Name (_DSD, Package () {
          ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
          Package() {
            Package() {"compatible", "nxp,sa56004"},
            Package() {"vcc-supply", 0xe},
          }
        })
      } //end of temperature sensor device
    } // end of channel 1 of mux
  } //end of mux-0 device
} // end of i2c controllers
