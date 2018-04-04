#  LX2160A.dsc
#  LX2160A Soc package.
#
#  Copyright 2018 NXP
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
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x6200000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x0c0c0000

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdGenericWatchdogControlBase|0x23A0000
  gArmTokenSpaceGuid.PcdGenericWatchdogRefreshBase|0x2390000
  gArmTokenSpaceGuid.PcdGenericWatchdogEl2IntrNum|28

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
  gNxpQoriqLsTokenSpaceGuid.PcdSystemMemoryExBase|0x2080000000    # Extended System Memory Base
  gNxpQoriqLsTokenSpaceGuid.PcdSystemMemoryExSize|0x0380000000    # 14GB Extended System Memory Size
  gNxpQoriqLsTokenSpaceGuid.PcdUsbBaseAddr|0x3100000
  gNxpQoriqLsTokenSpaceGuid.PcdUsbSize|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdGutsBaseAddr|0x1E00000
  gNxpQoriqLsTokenSpaceGuid.PcdSdxcBaseAddr|0x02140000
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

##