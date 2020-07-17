/** @file
*  DSDT : QSPI (Quad SPI) Devices
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <AcpiTableInclude/Dsdt/Qspi.asl>

Scope(_SB.QPI0)
{
  Method (_STA, 0, Serialized)  // _STA: Status
  {
    // Indicate a valid device for which no device driver should be loaded.
    Return (QSPI_STATUS)
  }
}

Scope(_SB.QPI0.MTD1)
{
  Method (_STA, 0, Serialized)  // _STA: Status
  {
    // On frwy platform only one MTD device is present
    Return (Zero)
  }
}
