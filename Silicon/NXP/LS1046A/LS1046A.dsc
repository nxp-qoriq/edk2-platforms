#  LS1046A.dsc
#  LS1046A Soc package.
#
#  Copyright 2017-2020 NXP
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
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x01410000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x01420000
  gNxpQoriqLsTokenSpaceGuid.PcdPciFdtCompatible|"fsl,ls1046a-pcie"

[LibraryClasses.common]
  #AcpiPlatformLib|Silicon/NXP/LS1046A/Library/AcpiPlatformLib/AcpiPlatformLib.inf
  SocPlatformLib|Silicon/NXP/LS1046A/Library/SocPlatformLib/SocPlatformLib.inf

[PcdsFixedAtBuild.common]

  #
  # CCSR Address Space and other attached Memories
  #
  gNxpQoriqLsTokenSpaceGuid.PcdCcsrBaseAddr|0x01000000
  gNxpQoriqLsTokenSpaceGuid.PcdCcsrSize|0x0F000000
  gNxpQoriqLsTokenSpaceGuid.PcdClkBaseAddr|0x01EE1000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcRegion1BaseAddr|0x60000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcRegion1Size|0x20000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcRegion2BaseAddr|0x0620000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcRegion2Size|0x00E0000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcNandReservedSize|0x2EA
  gNxpQoriqLsTokenSpaceGuid.PcdQmanSwpBaseAddr|0x0500000000
  gNxpQoriqLsTokenSpaceGuid.PcdQmanSwpSize|0x0080000000
  gNxpQoriqLsTokenSpaceGuid.PcdBmanSwpBaseAddr|0x0508000000
  gNxpQoriqLsTokenSpaceGuid.PcdBmanSwpSize|0x0080000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseAddr|0x4000000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseSize|0x800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp2BaseAddr|0x4800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp2BaseSize|0x800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp3BaseAddr|0x5000000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp3BaseSize|0x800000000
  gNxpQoriqLsTokenSpaceGuid.PcdScfgBaseAddr|0x1570000
  gNxpQoriqLsTokenSpaceGuid.PcdScfgIntPol|0x80000000
  gNxpQoriqLsTokenSpaceGuid.PcdGutsBaseAddr|0x01EE0000
  gNxpQoriqLsTokenSpaceGuid.PcdWdog1BaseAddr|0x02AD0000
  gNxpQoriqLsTokenSpaceGuid.PcdSdxcBaseAddr|0x01560000
  gNxpQoriqLsTokenSpaceGuid.PcdI2c0BaseAddr|0x02180000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbBaseAddr|0x2F00000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbSize|0x100000
  gNxpQoriqLsTokenSpaceGuid.PcdNumUsbController|3
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdNumI2cController|4
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegionBaseAddr|0x40000000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegionSize|0x20000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcBaseAddr|0x1530000
  gNxpQoriqLsTokenSpaceGuid.PcdNumPciController|3
  gNxpQoriqLsTokenSpaceGuid.PcdRomBaseAddr|0x00000000
  gNxpQoriqLsTokenSpaceGuid.PcdRomSize|0x00100000
  gNxpQoriqLsTokenSpaceGuid.PcdDcsrBaseAddr|0x20000000
  gNxpQoriqLsTokenSpaceGuid.PcdDcsrSize|0x04000000
  gNxpQoriqLsTokenSpaceGuid.PcdSataBaseAddr|0x3200000
  gNxpQoriqLsTokenSpaceGuid.PcdSataSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdNumSataController|0x1

  gNxpQoriqLsTokenSpaceGuid.PcdQspiFdtCompatible|"fsl,ls1021a-qspi"
  gNxpQoriqLsTokenSpaceGuid.PcdDspiFdtCompatible|"fsl,ls1021a-v1.0-dspi"
  gNxpQoriqLsTokenSpaceGuid.PcdQspiErratumA008886|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdSpiBusCount|2

  gNxpQoriqLsTokenSpaceGuid.PcdUsbPhy1BaseAddress|0x084F0000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbPhy2BaseAddress|0x08500000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbPhy3BaseAddress|0x08510000

  gNxpQoriqLsTokenSpaceGuid.PcdNumGpioController|0x04
  gNxpQoriqLsTokenSpaceGuid.PcdGpioModuleBaseAddress|0x02300000
  gNxpQoriqLsTokenSpaceGuid.PcdGpioControllerOffset|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdGpioControllerBigEndian|TRUE

##
