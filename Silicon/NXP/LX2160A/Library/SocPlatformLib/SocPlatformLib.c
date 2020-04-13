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

/**
  Get PCIe controller type based on SoC Version

**/
VOID
SocGetPcieCtrl (
  )
{
  CCSR_GUR     *GurBase;
  UINT32        Svr;

  GurBase = (VOID *)PcdGet64 (PcdGutsBaseAddr);
  Svr = GurRead ((UINTN)&GurBase->Svr);

  if ((Svr & SVR_LX2160A_REV_MASK) == SVR_LX2160A_REV1) {
    PcdSetBool(PcdPciLsGen4Ctrl, TRUE);
  }
}

/**
  Get StreamId Allocation Scheme

**/
VOID
SocGetStreamIdAllocationScheme (
  )
{
  CCSR_GUR     *GurBase;
  UINT32        Svr;

  GurBase = (VOID *)PcdGet64 (PcdGutsBaseAddr);
  Svr = GurRead ((UINTN)&GurBase->Svr);

  if (((Svr & SVR_LX2160A_REV_MASK) >> 8) == SVR_LX2160A) {
    PcdSetBool(PcdPciStreamIdPerCtrl, TRUE);
  }
}

/**
  Set RootPort Hide Option

**/
VOID
SocSetPciRootPort (
  )
{
  CCSR_GUR     *GurBase;
  UINT32        Svr;

  GurBase = (VOID *)PcdGet64 (PcdGutsBaseAddr);
  Svr = GurRead ((UINTN)&GurBase->Svr);

  if ((Svr & SVR_LX2160A_REV_MASK) == SVR_LX2160A_REV2) {
    PcdSetBool(PcdPciHideRootPort, TRUE);
  }
}
