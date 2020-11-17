#  LS1046aRdbPkg.dsc
#
#  LS1046ARDB Board package.
#
#  Copyright 2017, 2020 NXP
#  Copyright 2020 Puresoftware Ltd.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  #
  # Defines for default states.  These can be changed on the command line.
  # -D FLAG=VALUE
  #
  PLATFORM_NAME                  = LS1046aRdbPkg
  PLATFORM_GUID                  = 43920156-3f3b-4199-9b29-c6db1fb792b0
  OUTPUT_DIRECTORY               = Build/LS1046aRdbPkg
  FLASH_DEFINITION               = Platform/NXP/LS1046aRdbPkg/LS1046aRdbPkg.fdf

!include Silicon/NXP/NxpQoriqLs.dsc.inc
!include Silicon/NXP/LS1046A/LS1046A.dsc.inc

[LibraryClasses.common]
  ArmPlatformLib|Platform/NXP/LS1046aRdbPkg/Library/ArmPlatformLib/ArmPlatformLib.inf
  RealTimeClockLib|Silicon/NXP/Library/Pcf2129RealTimeClockLib/Pcf2129RealTimeClockLib.inf
  SocFixupLib|Silicon/NXP/LS1046A/Library/SocFixupLib/SocFixupLib.inf
  ItbParseLib|Silicon/NXP/Library/ItbParseLib/ItbParse.inf
  SocLib|Silicon/NXP/LS1046A/Library/SocLib/SocLib.inf
  FpgaLib|Platform/NXP/LS1046aRdbPkg/Library/FpgaLib/FpgaLib.inf

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
  #
  # Architectural Protocols
  #
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <PcdsFixedAtBuild>
    gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable|TRUE
  }

  Platform/NXP/LS1046aFrwyPkg/Drivers/PlatformDxe/PlatformDxe.inf

  #
  # DT support
  #

  Silicon/NXP/Drivers/DtInitDxe/DtInitDxe.inf {
    <LibraryClasses>
      FdtLib|EmbeddedPkg/Library/FdtLib/FdtLib.inf
      DtPlatformDtbLoaderLib|Silicon/NXP/Library/DtbLoaderLib/DtbLoaderLib.inf
  }

  EmbeddedPkg/Drivers/DtPlatformDxe/DtPlatformDxe.inf {
    <LibraryClasses>
      FdtLib|EmbeddedPkg/Library/FdtLib/FdtLib.inf
      DtPlatformDtbLoaderLib|Silicon/NXP/Library/DtbLoaderLib/DtbLoaderLib.inf
  }

  Platform/NXP/LS1046aRdbPkg/DeviceTree/DeviceTree.inf

  Silicon/NXP/Drivers/SataInitDxe/SataInitDxe.inf
  Silicon/NXP/Drivers/UsbHcdInitDxe/UsbHcd.inf

##
