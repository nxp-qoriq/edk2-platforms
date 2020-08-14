/** @file

  Fixup the ICIDs of legacy devices in DCFG space and create
  iommus property in corresponding device's Device tree node.

  Copyright 2020 PureSoftware

  SPDX-License-Identifier: BSD-2-Clause

**/
#include <Library/DebugLib.h>
#include <Library/ItbParse.h>
#include <Library/UefiLib.h>
#include <Library/IoAccessLib.h>
#include <Library/IoLib.h>
#include <Soc.h>
#include <Uefi.h>
#include <Chassis.h>

/**
  Entry point of the Icid Application.
  Fixup the ICIDs of legacy devices in DCFG space

  @param  ImageHandle  A handle for the image that is initializing this driver.
  @param  SystemTable  A pointer to the EFI system table.

  @return EFI_SUCCESS  Driver initialized successfully.

**/
EFI_STATUS
EFIAPI
FixupIcid (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  CCSR_SCFG *Scfg;
  UINT32 Icid;
  Scfg = (VOID *)PcdGet64 (PcdScfgBaseAddr);

  Icid = NXP_USB0_STREAM_ID;
  MmioWrite32 ((UINTN)&Scfg->Usb1Icid, Icid);

  Icid = NXP_USB1_STREAM_ID;
  MmioWrite32 ((UINTN)&Scfg->Usb2Icid, Icid);

  Icid = NXP_USB2_STREAM_ID;
  MmioWrite32 ((UINTN)&Scfg->Usb3Icid, Icid);

  Icid = NXP_SATA0_STREAM_ID;
  MmioWrite32 ((UINTN)&Scfg->SataIcid, Icid);

  return EFI_SUCCESS;
}
