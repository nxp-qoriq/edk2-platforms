#  LS1028A.dsc
#  LS1028A Soc package.
#
#  Copyright 2019-2020 NXP
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsDynamicDefault.common]

  #
  # ARM General Interrupt Controller
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x06000000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x06040000
  gNxpQoriqLsTokenSpaceGuid.PcdPciFdtCompatible|"fsl,ls1028a-pcie"

[LibraryClasses.common]
  SocClockLib|Silicon/NXP/LS1028A/Library/SocClockLib/SocClockLib.inf
  SocFixupLib|Silicon/NXP/LS1028A/Library/SocFixupLib/SocFixupLib.inf
  SocPlatformLib|Silicon/NXP/LS1028A/Library/SocPlatformLib/SocPlatformLib.inf

[PcdsFixedAtBuild.common]

  #
  # CCSR Address Space and other attached Memories
  #
  gNxpQoriqLsTokenSpaceGuid.PcdCcsrBaseAddr|0x01000000
  gNxpQoriqLsTokenSpaceGuid.PcdCcsrSize|0x0F000000
  gNxpQoriqLsTokenSpaceGuid.PcdClkBaseAddr|0x1370000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegionBaseAddr|0x20000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseAddr|0x8000000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseSize|0x800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp2BaseAddr|0x8800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp2BaseSize|0x800000000
  gNxpQoriqLsTokenSpaceGuid.PcdScfgBaseAddr|0x1FC0000
  gNxpQoriqLsTokenSpaceGuid.PcdGutsBaseAddr|0x01E00000
  gNxpQoriqLsTokenSpaceGuid.PcdWdog1BaseAddr|0x02AD0000
  gNxpQoriqLsTokenSpaceGuid.PcdSdxcBaseAddr|0x02140000
  gNxpQoriqLsTokenSpaceGuid.PcdI2c0BaseAddr|0x02000000
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdNumI2cController|8
  gNxpQoriqLsTokenSpaceGuid.PcdNumPciController|2
  gNxpQoriqLsTokenSpaceGuid.PcdRomBaseAddr|0x00000000
  gNxpQoriqLsTokenSpaceGuid.PcdRomSize|0x00100000
  gNxpQoriqLsTokenSpaceGuid.PcdSataBaseAddr|0x3200000
  gNxpQoriqLsTokenSpaceGuid.PcdSataSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdNumSataController|0x1

  gNxpQoriqLsTokenSpaceGuid.PcdFlexSpiFdtCompatible|"nxp,lx2160a-fspi"
  gNxpQoriqLsTokenSpaceGuid.PcdDspiFdtCompatible|"fsl,ls1021a-v1.0-dspi"
  gNxpQoriqLsTokenSpaceGuid.PcdSpiBusCount|4
  gNxpQoriqLsTokenSpaceGuid.PcdCh3Srds1PrtclMask|0xffff0000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcEnabled|FALSE
  gNxpQoriqLsTokenSpaceGuid.PcdNumCcPlls|3
##
