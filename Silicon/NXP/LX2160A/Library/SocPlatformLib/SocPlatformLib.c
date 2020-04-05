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
  CCSR_GUR     *GurBase;
  UINT32        Svr;

  GurBase = (VOID *)PcdGet64 (PcdGutsBaseAddr);
  Svr = GurRead ((UINTN)&GurBase->Svr);

  if ((Svr & SVR_LX2160A_REV_MASK) == SVR_LX2160A_REV2) {
    PcdSetBool(PcdPciCfgShiftEnable, TRUE);
  }
}
