/** @file
*
*  Copyright 2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Pcie.h>

/**
  The entry point for PlatformDxe driver. This driver
  intends to perform platform specific initialization.

  @param[in] ImageHandle     The image handle of the driver.
  @param[in] SystemTable     The system table.

  @retval EFI_SUCCESS         Driver initialization success.

**/
EFI_STATUS
EFIAPI
PlatformDxeEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  // Enable/disable PCIe CFG Shift Feature
  SocPcieCfgShift ();

  // Get PCIe controller type
  SocGetPcieCtrl ();

  // Get StreamId Allocation Scheme
  SocGetStreamIdAllocationScheme ();

  // Set Pcie Root Port hide option
  SocSetPciRootPort ();

  return EFI_SUCCESS;
}

