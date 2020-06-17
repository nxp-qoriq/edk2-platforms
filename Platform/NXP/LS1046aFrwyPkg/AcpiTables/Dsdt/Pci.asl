/** @file
*  DSDT : PCI Express Controllers Devices
*
*  Copyright 2019-2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*/

#include <AcpiTableInclude/Dsdt/Pci.asl>

Scope(\_SB.PCI0)
{
  Method (_STA, 0, Notserialized)
  {
    Return (Zero)
  }
}
