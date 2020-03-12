/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright 2019-2020 NXP
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <AcpiTablesInclude/Dsdt/Mdio.asl>

Scope(\_SB.MDI0)
{
  Device(PHY1) {
    Name (_ADR, 0x1)
  } // end of PHY1

  Device(PHY2) {
    Name (_ADR, 0x2)
  } // end of PHY2

  Device(PHY4) {
    Name (_ADR, 0x4)
    Name(_CRS, ResourceTemplate() {
    Interrupt(ResourceConsumer, Level, ActiveHigh, Shared)
    {
      AQR_PHY4_IT
    }
    }) // end of _CRS for PHY4
  } // end of PHY4

  Device(PHY5) {
    Name (_ADR, 0x5)
    Name(_CRS, ResourceTemplate() {
    Interrupt(ResourceConsumer, Level, ActiveHigh, Shared)
    {
      AQR_PHY5_IT
    }
    }) // end of _CRS for PHY5
  } // end of PHY5
} // end of MDI0
