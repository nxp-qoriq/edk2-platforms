#  LX2160aRdbPkg.dsc
#
#  LX2160ARDB Board package.
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
  PLATFORM_NAME                  = LX2160aRdbPkg
  PLATFORM_GUID                  = be06d8bc-05eb-44d6-b39f-191e93617ebd
  OUTPUT_DIRECTORY               = Build/LX2160aRdbPkg
  FLASH_DEFINITION               = Platform/NXP/LX2160aRdbPkg/LX2160aRdbPkg.fdf
  DEFINE MC_HIGH_MEM             = TRUE

!include ../NxpQoriqLs.dsc
!include ../../../Silicon/NXP/LX2160A/LX2160A.dsc

[LibraryClasses.common]
  ArmPlatformLib|Platform/NXP/LX2160aRdbPkg/Library/PlatformLib/ArmPlatformLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  PL011UartLib|ArmPlatformPkg/Library/PL011UartLib/PL011UartLib.inf
  SerialPortLib|ArmPlatformPkg/Library/PL011SerialPortLib/PL011SerialPortLib.inf
  BeIoLib|Silicon/NXP/Library/BeIoLib/BeIoLib.inf
  SocLib|Silicon/NXP/Chassis/LX2160aSocLib.inf
  RealTimeClockLib|Silicon/NXP/Library/Pcf2129RtcLib/Pcf2129RtcLib.inf
  I2cLib|Silicon/NXP/Library/I2cLib/I2cLib.inf
  FpgaLib|Platform/NXP/LX2160aRdbPkg/Library/FpgaLib/FpgaLib.inf
  PciSegmentLib|Silicon/NXP/Library/PciSegmentLib/PciSegmentLib.inf
  PciHostBridgeLib|Silicon/NXP/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  MmcLib|Silicon/NXP/Library/MmcLib/MmcLib.inf
  ItbParseLib|Silicon/NXP/Library/ItbParseLib/ItbParse.inf  

[PcdsFixedAtBuild.common]

!if $(MC_HIGH_MEM) == TRUE                                        # Management Complex loaded at the end of DDR2
  gNxpQoriqLsTokenSpaceGuid.PcdDram1BaseAddr|0x0080000000         # Actual base address (0x0080000000)
  gNxpQoriqLsTokenSpaceGuid.PcdDram1Size|0x0080000000             # 2 GB
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McRamSize|0x80000000          # 2GB (PcdDpaa2McRamSize must be 512MB aligned)
  gNxpQoriqLsTokenSpaceGuid.PcdMcHighMemSupport|1
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x0080000000             # Actual base
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x0080000000             # 2G
!else
  gNxpQoriqLsTokenSpaceGuid.PcdDram1BaseAddr|0x00A0000000         # Actual base address (0x0080000000) + 512MB
  gNxpQoriqLsTokenSpaceGuid.PcdDram1Size|0x0060000000             # 2GB - 512MB
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McRamSize|0x20000000          # 512MB (Fixed)
  gNxpQoriqLsTokenSpaceGuid.PcdMcHighMemSupport|0
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x00A0000000             # Actual base + 512MB
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x0060000000             # 2G - 512MB
!endif
  gNxpQoriqLsTokenSpaceGuid.PcdDramMemSize|0x380000000            # 14 GB
  gNxpQoriqLsTokenSpaceGuid.PcdDram2BaseAddr|0x2080000000
  gNxpQoriqLsTokenSpaceGuid.PcdDram2Size|0x8800000000             # 512 GB
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x02000000

  #
  # Board Specific Pcds
  #
  # ARM SBSA WDT
  gArmTokenSpaceGuid.PcdGenericWatchdogControlBase|0x23A0000
  gArmTokenSpaceGuid.PcdGenericWatchdogRefreshBase|0x2390000
  gArmTokenSpaceGuid.PcdGenericWatchdogEl2IntrNum|28

  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x21C0000
  gArmPlatformTokenSpaceGuid.PL011UartClkInHz|150000000
  gNxpQoriqLsTokenSpaceGuid.PcdSerdes2Enabled|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdPlatformFreqDiv|0x4
  gNxpQoriqLsTokenSpaceGuid.PcdDdrClk|133333333

  gNxpQoriqLsTokenSpaceGuid.PcdOcramStackBase|0x18010000
  gNxpQoriqLsTokenSpaceGuid.PcdFdNorBaseAddress|0x20100000

  gNxpQoriqLsTokenSpaceGuid.PcdI2cErratumA009203|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdUsbErratumA009007|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdErratumA008751|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdErratumA009008|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdErratumA009798|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdErratumA008514|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdErratumA008336|TRUE



  #
  # I2C controller Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdI2cBus|0

  #
  # RTC Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdI2cBus|0
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSpeed|100000
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSlaveAddress|0x51

  #
  # NV Storage PCDs.
  #
  gArmTokenSpaceGuid.PcdVFPEnabled|1

  #
  # PCI PCDs.
  #
  gNxpQoriqLsTokenSpaceGuid.PcdPciDebug|FALSE
  gNxpQoriqLsTokenSpaceGuid.PcdPcieLutBase|0x80000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieLutDbg|0x407FC

  #
  # SATA Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdSataErratumA009185|TRUE
################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
  #
  # Architectural Protocols
  #
  MdeModulePkg/Universal/Variable/EmuRuntimeDxe/EmuVariableRuntimeDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf{
     <LibraryClasses>
     NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
  }
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf
  ArmPkg/Drivers/GenericWatchdogDxe/GenericWatchdogDxe.inf
  Silicon/NXP/Drivers/I2cDxe/I2cDxe.inf
  Silicon/NXP/Drivers/UsbHcdInitDxe/UsbHcd.inf
  Silicon/NXP/Drivers/PciCpuIo2Dxe/PciCpuIo2Dxe.inf
  Silicon/NXP/Library/Pcf2129RtcLib/Pcf2129RtcLib.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <PcdsFixedAtBuild>
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x8010004F
  }
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  Silicon/NXP/Drivers/SataInitDxe/SataInitDxe.inf
  Silicon/NXP/Drivers/MmcHostDxe/MmcHostDxe.inf

  #
  # PrePiNor
  #
  Platform/NXP/LX2160aRdbPkg/Library/PrePiNor/PrePiNor.inf
  Silicon/NXP/Drivers/PpaInitDxe/PpaInitDxe.inf


 ##
