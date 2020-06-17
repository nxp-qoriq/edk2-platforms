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

#define NXP_USB0_STREAM_ID    1
#define NXP_USB1_STREAM_ID    2
#define NXP_USB2_STREAM_ID    3
#define NXP_SATA0_STREAM_ID   5

#include <AcpiTableInclude/Icid.c>
