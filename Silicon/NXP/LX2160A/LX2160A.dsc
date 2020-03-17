#  LX2160A.dsc
#  LX2160A Soc package.
#
#  Copyright 2018-2020 NXP
#
#  SPDX-License-Identifier: BSD-2-Clause
#

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsDynamicDefault.common]

  #
  # ARM General Interrupt Controller
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x6000000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x6200000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x0c0c0000
  gNxpQoriqLsTokenSpaceGuid.PcdPciFdtCompatible|"fsl,lx2160a-pcie"

[LibraryClasses.common]
  SocClockLib|Silicon/NXP/LX2160A/Library/SocClockLib/SocClockLib.inf
  SocFixupLib|Silicon/NXP/LX2160A/Library/SocFixupLib/SocFixupLib.inf
  AcpiPlatformLib|Silicon/NXP/LX2160A/Library/AcpiPlatformLib/AcpiPlatformLib.inf

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdGenericWatchdogControlBase|0x23A0000
  gArmTokenSpaceGuid.PcdGenericWatchdogRefreshBase|0x2390000
  gArmTokenSpaceGuid.PcdGenericWatchdogEl2IntrNum|28

  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultReceiveFifoDepth|0

  #
  # ARM L2x0 PCDs
  gArmTokenSpaceGuid.PcdL2x0ControllerBase|0x10900000

  #
  # CCSR Address Space and other attached Memories
  #
  gNxpQoriqLsTokenSpaceGuid.PcdCcsrBaseAddr|0x01000000
  gNxpQoriqLsTokenSpaceGuid.PcdCcsrSize|0x0F000000
  gNxpQoriqLsTokenSpaceGuid.PcdClkBaseAddr|0x1370000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegionBaseAddr|0x20000000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegionSize|0x10000000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegion2BaseAddr|0x400000000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegion2Size|0x10000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseAddr|0x8000000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseSize|0x800000000        # 32 GB
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp2BaseAddr|0x8800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp2BaseSize|0x800000000        # 32 GB
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp3BaseAddr|0x9000000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp3BaseSize|0x800000000        # 32 GB
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp4BaseAddr|0x9800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp4BaseSize|0x800000000        # 32 GB
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp5BaseAddr|0xA000000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp6BaseAddr|0xA800000000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbBaseAddr|0x3100000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdGutsBaseAddr|0x1E00000
  gNxpQoriqLsTokenSpaceGuid.PcdSdxcBaseAddr|0x02140000
  gNxpQoriqLsTokenSpaceGuid.PcdEMmcBaseAddr|0x02150000
  gNxpQoriqLsTokenSpaceGuid.PcdI2c0BaseAddr|0x02000000
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdNumI2cController|8
  gNxpQoriqLsTokenSpaceGuid.PcdNumUsbController|2
  gNxpQoriqLsTokenSpaceGuid.PcdNumPciController|6
  gNxpQoriqLsTokenSpaceGuid.PcdRomBaseAddr|0x00000000
  gNxpQoriqLsTokenSpaceGuid.PcdRomSize|0x00100000
  gNxpQoriqLsTokenSpaceGuid.PcdSataBaseAddr|0x3200000
  gNxpQoriqLsTokenSpaceGuid.PcdSataSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdNumSataController|0x4
  gNxpQoriqLsTokenSpaceGuid.PcdPciCfgShiftEnable|TRUE

  gNxpQoriqLsTokenSpaceGuid.PcdFlexSpiFdtCompatible|"nxp,lx2160a-fspi"
  gNxpQoriqLsTokenSpaceGuid.PcdSpiBusCount|4
  #
  # DPAA2 specific Pcd
  #
  gNxpQoriqLsTokenSpaceGuid.PcdMdioBustCount|0x2
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2Wriop1Mdio1Addr|0x8B96000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2Wriop1Mdio2Addr|0x8B97000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McPortalBaseAddr|0x00080C000000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McPortalSize|0x4000000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2NiPortalsBaseAddr|0x000810000000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2NiPortalsSize|0x8000000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2QBmanPortalsBaseAddr|0x000818000000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2QBmanPortalSize|0x8000000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2QBmanPortalsCacheSize|0x4000000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McFwNorAddr|0x20A00000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McDpcNorAddr|0x20E00000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McDplNorAddr|0x20D00000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McDpcMaxLen|0x20000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McDpcMcDramOffset|0x00F00000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McDplMaxLen|0x20000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McDplMcDramOffset|0x00F20000
# to check for Cortina phy f/w
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2CortinaFwNorAddr|0x20980000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2CortinaFwMaxLen|0x40000
  gNxpQoriqLsTokenSpaceGuid.PcdBypassAmqMask|0x60000
  gNxpQoriqLsTokenSpaceGuid.PcdMacDeviceDisableRegAddr|0x1e00074

[PcdsFeatureFlag]
  gNxpQoriqLsTokenSpaceGuid.PcdI2cErratumA009203|TRUE
##
