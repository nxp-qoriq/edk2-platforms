/** @file
*
*  Copyright 2019-2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/IoAccessLib.h>
#include <Library/GpioLib.h>
#include <Library/SocLib.h>
#include <Include/Soc.h>
#include <Include/SocClockInternalLib.h>

#include <Ppi/ArmMpCoreInfo.h>
#include <Ppi/NxpPlatformGetClock.h>

#define USB2_MUX_SEL_GPIO    23

ARM_CORE_INFO mLS1046aMpCoreInfoTable[] = {
  {
    // Cluster 0, Core 0
    0x0, 0x0,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  }
};

/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

/**
  Get the clocks supplied by Platform(Board) to NXP Layerscape SOC IPs

  @param[in]  ClockType  Variable of Type NXP_IP_CLOCK. Indicates which IP clock
                         is to be retrieved.
  @param[in]  ...        Variable argument list which is parsed based on
                         ClockType. e.g. if the ClockType is NXP_I2C_CLOCK, then
                         the second argument will be interpreted as controller
                         number.
                         if ClockType is NXP_CORE_CLOCK, then second argument
                         is interpreted as cluster number and third argument is
                         interpreted as core number (within the cluster)

  @return                Actual Clock Frequency. Return value 0 should be
                         interpreted as clock not being provided to IP.
**/
UINT64
EFIAPI
NxpPlatformGetClock(
  IN  UINT32  ClockType,
  ...
  )
{
  UINT64      Clock;
  VA_LIST     Args;
  MMIO_OPERATIONS  *mScfgOps;
  CCSR_SCFG    *Scfg;
  UINT32       ConfigRegister; // device configuration register. can be used for any device
  RCW_FIELDS   *Rcw;
  UINT64       ClusterGroupA;
  CCSR_GUR     *GurBase;

  Clock = 0;

  GurBase = (VOID *)PcdGet64 (PcdGutsBaseAddr);
  Scfg    = (VOID *)PcdGet64 (PcdScfgBaseAddr);
  ASSERT ((GurBase != NULL) && (Scfg != NULL));

  mScfgOps = GetMmioOperations (FeaturePcdGet (PcdScfgBigEndian));

  VA_START (Args, ClockType);
  Rcw = (RCW_FIELDS *)GurBase->RcwSr;

  switch (ClockType) {
  case NXP_SYSTEM_CLOCK:
  case NXP_USB_PHY_CLOCK:
    Clock = 100 * 1000 * 1000; // 100 MHz
    break;
  case NXP_I2C_CLOCK:
  case NXP_UART_CLOCK:
    Clock = NxpPlatformGetClock (NXP_SYSTEM_CLOCK);
    Clock = SocGetClock (Clock, ClockType, Args);
    break;
   case NXP_QSPI_CLOCK:
    Clock = NxpPlatformGetClock (NXP_SYSTEM_CLOCK);
    ConfigRegister = mScfgOps->Read32 ( (UINTN)&Scfg->QspiCfg);
    if (ConfigRegister & QSPI_CLOCK_DISABLE) {
      break;
    }

    switch (Rcw->HwaCgaM2ClkSel) {
      case 1:
      case 2:
      case 3:
        ClusterGroupA = ((UINT64)Rcw->CgaPll2Rat * Clock) / Rcw->HwaCgaM2ClkSel;
        break;
      case 6:
        ClusterGroupA = ((UINT64)Rcw->CgaPll1Rat * Clock) >> 1;
        break;
      default:
        ClusterGroupA = 0;
        break;
    }

    if (ClusterGroupA) {
      switch ((ConfigRegister & 0xF0000000) >> 28) { // CLK_SEL bits in SCFG_QSPI_CFG
        case 0:
          Clock = ClusterGroupA >> 8; // Divide by 256
          break;
        case 1:
          Clock = ClusterGroupA >> 6; // Divide by 64
          break;
        case 2:
          Clock = ClusterGroupA >> 5; // Divide by 32
          break;
        case 3:
          Clock = (UINT64)ClusterGroupA / 24; // Divide by 24
          break;
        case 4:
          Clock = (UINT64)ClusterGroupA / 20; // Divide by 20
          break;
        case 5:
          Clock = ClusterGroupA >> 4; // Divide by 16
          break;
        case 6:
          Clock = (UINT64)ClusterGroupA / 12; // Divide by 12
          break;
        case 7:
          Clock = ClusterGroupA >> 3; // Divide by 8
          break;
        default:
          break;
      }
    }
    break;

  default:
    break;
  }

  VA_END (Args);

  return Clock;
}

/**
  FRWY-LS1046A GPIO 23 use for USB2
  mux seclection
**/
STATIC VOID  MuxSelectUsb2 (VOID)
{

  SetDir (GPIO3, USB2_MUX_SEL_GPIO, OUTPUT);
  SetData (GPIO3, USB2_MUX_SEL_GPIO, HIGH);

  return;
}

/**
  Initialize controllers that must setup in the normal world

  This function is called by the ArmPlatformPkg/PrePi or ArmPlatformPkg/PlatformPei
  in the PEI phase.

**/
EFI_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
  SocInit ();
  MuxSelectUsb2 ();

  return EFI_SUCCESS;
}

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  if (ArmIsMpCore()) {
    *CoreCount    = sizeof(mLS1046aMpCoreInfoTable) / sizeof(ARM_CORE_INFO);
    *ArmCoreTable = mLS1046aMpCoreInfoTable;
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };
NXP_PLATFORM_GET_CLOCK_PPI gPlatformGetClockPpi = { NxpPlatformGetClock };

EFI_PEI_PPI_DESCRIPTOR      gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  if (ArmIsMpCore()) {
    *PpiListSize = sizeof(gPlatformPpiTable);
    *PpiList = gPlatformPpiTable;
  } else {
    *PpiListSize = 0;
    *PpiList = NULL;
  }
}
