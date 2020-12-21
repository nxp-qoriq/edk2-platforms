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
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE

!include Silicon/NXP/NxpQoriqLs.dsc.inc
!include Silicon/NXP/LS1046A/LS1046A.dsc.inc

[LibraryClasses.common]
  ArmPlatformLib|Platform/NXP/LS1046aRdbPkg/Library/ArmPlatformLib/ArmPlatformLib.inf
  RealTimeClockLib|Silicon/NXP/Library/Pcf2129RealTimeClockLib/Pcf2129RealTimeClockLib.inf
  SocFixupLib|Silicon/NXP/LS1046A/Library/SocFixupLib/SocFixupLib.inf
  ItbParseLib|Silicon/NXP/Library/ItbParseLib/ItbParse.inf
  SocLib|Silicon/NXP/LS1046A/Library/SocLib/SocLib.inf
  PciSegmentLib|Silicon/NXP/Library/PciSegmentLib/PciSegmentLib.inf
  PciHostBridgeLib|Silicon/NXP/Library/PciHostBridgeLib/PciHostBridgeLib.inf

  #
  # DPAA1
  #
  Dpaa1Lib|Silicon/NXP/Library/Dpaa1Lib/Dpaa1Lib.inf
  Dpaa1EthernetMacLib|Silicon/NXP/Library/Dpaa1EthernetMacLib/Dpaa1EthernetMacLib.inf
  Dpaa1EthernetPhyLib|Silicon/NXP/Library/Dpaa1EthernetPhyLib/Dpaa1EthernetPhyLib.inf
  Dpaa1BoardLib|Platform/NXP/LS1046aRdbPkg/Library/Dpaa1BoardLib/Dpaa1BoardLib.inf

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

  #
  # PCI
  #
  Silicon/NXP/Drivers/PciCpuIo2Dxe/PciCpuIo2Dxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  Silicon/NXP/Drivers/LanIntelE1000Dxe/LanIntelE1000Dxe.inf

  #
  # Networking stack
  #
  !include NetworkPkg/Network.dsc.inc

  #
  # DPAA1 Ethernet driver
  #
  Silicon/NXP/Drivers/Dpaa1Ethernet/Dpaa1EthernetDxe.inf

  # Platform DXE Driver
  Platform/NXP/LS1046aRdbPkg/Drivers/PlatformDxe/PlatformDxe.inf
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
  Silicon/NXP/Drivers/Dpaa1EthernetDxe/Dpaa1EthernetDxe.inf

  Silicon/NXP/Drivers/SataInitDxe/SataInitDxe.inf
  Silicon/NXP/Drivers/UsbHcdInitDxe/UsbHcd.inf

[PcdsFixedAtBuild.common]

  #
  # Big Endian IPs
  #
  gNxpQoriqLsTokenSpaceGuid.PcdGurBigEndian|TRUE


 #
 # DPAA1 Pcds
 #
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1Initialize|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1DebugFlags|0x0
  gNxpQoriqLsTokenSpaceGuid.PcdFManFwFlashAddr|0x40900000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1UsedMemacsMask|0x33C
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanMdio1Addr|0x01AFC000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanMdio2Addr|0x01AFD000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanAddr|0x01a00000
  gNxpQoriqLsTokenSpaceGuid.PcdFManFwFlashAddr|0x40900000
  gNxpQoriqLsTokenSpaceGuid.PcdSgmiiPrtclInit|TRUE

  gNxpQoriqLsTokenSpaceGuid.PcdFdtAddress|0x40F00000
##
