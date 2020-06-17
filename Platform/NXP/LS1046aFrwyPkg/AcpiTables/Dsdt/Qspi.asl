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

Scope(_SB.QPI0.MTD1)
{
  Method (_STA, 0, Serialized)  // _STA: Status
  {
    Return (Zero)
  }
}
