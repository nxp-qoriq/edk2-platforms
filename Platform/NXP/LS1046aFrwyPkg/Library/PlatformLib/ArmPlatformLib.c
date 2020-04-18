/** @file

 Copyright 2019-2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/ArmPlatformLib.h>
#include <Ppi/ArmMpCoreInfo.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>
#include "ArmPlatformInternalLib.h"

extern VOID SocInit (VOID);

/**
  FRWY-LS1046A GPIO 23 use for USB2
  mux seclection
**/
STATIC VOID  MuxSelectUsb2(VOID)
{

  SetDir (GPIO3,USB2_MUX_SEL_GPIO,OUTPUT);
  SetData (GPIO3,USB2_MUX_SEL_GPIO,HIGH);

  return;
}

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
 Placeholder for Platform Initialization
**/
EFI_STATUS
ArmPlatformInitialize (
  IN  UINTN   MpId
  )
{
 SocInit ();
 MuxSelectUsb2();

 return EFI_SUCCESS;
}

ARM_CORE_INFO LS1046aMpCoreInfoCTA72x4[] = {
  {
    // Cluster 0, Core 0
    0x0, 0x0,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
};

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  *CoreCount    = sizeof (LS1046aMpCoreInfoCTA72x4) / sizeof (ARM_CORE_INFO);
  *ArmCoreTable = LS1046aMpCoreInfoCTA72x4;

  return EFI_SUCCESS;
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };

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
  *PpiListSize = sizeof (gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}


UINTN
ArmPlatformGetCorePosition (
  IN UINTN MpId
  )
{
  return 1;
}
