#  LS2088A.dsc
#  LS2088A Soc package.
#
#  Copyright 2017 NXP
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution. The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
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
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x6100000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x00

[PcdsFixedAtBuild.common]

  gArmPlatformTokenSpaceGuid.PcdSP805WatchdogBase|0x0C000000
  gArmPlatformTokenSpaceGuid.PcdSP805WatchdogClockFrequencyInHz|266666666 #266MHz

  #
  # ARM L2x0 PCDs
  gArmTokenSpaceGuid.PcdL2x0ControllerBase|0x10900000

  #
  # CCSR Address Space and other attached Memories
  #
  gNxpQoriqLsTokenSpaceGuid.PcdCcsrBaseAddr|0x01000000
  gNxpQoriqLsTokenSpaceGuid.PcdCcsrSize|0x0F000000
  gNxpQoriqLsTokenSpaceGuid.PcdClkBaseAddr|0x1370000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcRegion1BaseAddr|0x30000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcRegion1Size|0x10000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcRegion2BaseAddr|0x510000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcRegion2Size|0xF0000000
  gNxpQoriqLsTokenSpaceGuid.PcdIfcNandReservedSize|0x3EEA
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegionBaseAddr|0x20000000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegionSize|0x10000000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegion2BaseAddr|0x400000000
  gNxpQoriqLsTokenSpaceGuid.PcdQspiRegion2Size|0x10000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseAddr|0x2000000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp1BaseSize|0x800000000        # 32 GB
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp2BaseAddr|0x2800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp2BaseSize|0x800000000        # 32 GB
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp3BaseAddr|0x3000000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp3BaseSize|0x800000000        # 32 GB
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp4BaseAddr|0x3800000000
  gNxpQoriqLsTokenSpaceGuid.PcdPciExp4BaseSize|0x800000000        # 32 GB
  gNxpQoriqLsTokenSpaceGuid.PcdSystemMemoryExBase|0x8080000000    # Extended System Memory Base
  gNxpQoriqLsTokenSpaceGuid.PcdSystemMemoryExSize|0x0380000000    # 14GB Extended System Memory Size
  gNxpQoriqLsTokenSpaceGuid.PcdUsbBaseAddr|0x3100000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdGutsBaseAddr|0x1E00000
  gNxpQoriqLsTokenSpaceGuid.PcdSdxcBaseAddr|0x02140000
  gNxpQoriqLsTokenSpaceGuid.PcdI2c0BaseAddr|0x02000000
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdNumI2cController|4

##
