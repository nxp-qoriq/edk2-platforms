/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Chassis.h>
#include <Soc.h>
#include <Library/DebugLib.h>

/**
  Enable PCie feature based on SoC revision

**/
VOID
SocPcieCfgShift (
  )
{
  PcdSetBool(PcdPciCfgShiftEnable, TRUE);
}

/**
  Get PCIe controller type based on SoC Version

**/
VOID
SocGetPcieCtrl (
  )
{
}

/**
  Get StreamId Allocation Scheme

**/
VOID
SocGetStreamIdAllocationScheme (
  )
{
}

/**
  Set RootPort Hide Option

**/
VOID
SocSetPciRootPort (
  )
{
  PcdSetBool(PcdPciHideRootPort, TRUE);
}
