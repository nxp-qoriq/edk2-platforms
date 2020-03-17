#  LS1043aRdbPkg.dsc
#
#  LS1043ARDB Board package.
#
#  Copyright 2017, 2020 NXP
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
  PLATFORM_NAME                  = LS1043aRdbPkg
  PLATFORM_GUID                  = 60169ec4-d2b4-44f8-825e-f8684fd42e4f
  OUTPUT_DIRECTORY               = Build/LS1043aRdbPkg
  FLASH_DEFINITION               = Platform/NXP/LS1043aRdbPkg/LS1043aRdbPkg.fdf
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = FALSE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE

!include Platform/NXP/NxpQoriqLs.dsc
!include Silicon/NXP/Chassis/Chassis2/Chassis2.dsc
!include Silicon/NXP/LS1043A/LS1043A.dsc

[LibraryClasses.common]
  ArmPlatformLib|Platform/NXP/LS1043aRdbPkg/Library/PlatformLib/ArmPlatformLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SerialPortLib|Silicon/NXP/Library/DUartPortLib/DUartPortLib.inf
  BeIoLib|Silicon/NXP/Library/BeIoLib/BeIoLib.inf
  SocLib|Silicon/NXP/Chassis/LS1043aSocLib.inf
  Ds1307RealTimeClockLib|Silicon/Maxim/Library/Ds1307RtcLib/Ds1307RtcLib.inf
  Pcf85263RealTimeClockLib|Silicon/NXP/Library/Pcf85263RtcLib/Pcf85263RtcLib.inf
  IfcLib|Silicon/NXP/Library/IfcLib/IfcLib.inf
  BoardLib|Platform/NXP/LS1043aRdbPkg/Library/BoardLib/BoardLib.inf
  FpgaLib|Platform/NXP/LS1043aRdbPkg/Library/FpgaLib/FpgaLib.inf
  NorFlashLib|Silicon/NXP/Library/NorFlashLib/NorFlashLib.inf
  PciSegmentLib|Silicon/NXP/Library/PciSegmentLib/PciSegmentLib.inf
  PciHostBridgeLib|Silicon/NXP/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  MmcLib|Silicon/NXP/Library/MmcLib/MmcLib.inf
  SecureMonRngLib|Silicon/NXP/Library/SecureMonRngLib/SecureMonRngLib.inf
  MemoryInitPeiLib|Silicon/NXP/Library/MemoryInitPei/MemoryInitPeiLib.inf

  #
  # DPAA1
  #
  Dpaa1Lib|Silicon/NXP/Library/Dpaa1Lib/Dpaa1Lib.inf
  Dpaa1EthernetMacLib|Silicon/NXP/Library/Dpaa1EthernetMacLib/Dpaa1EthernetMacLib.inf
  Dpaa1EthernetPhyLib|Silicon/NXP/Library/Dpaa1EthernetPhyLib/Dpaa1EthernetPhyLib.inf
  Dpaa1BoardLib|Platform/NXP/LS1043aRdbPkg/Library/Dpaa1BoardLib/Dpaa1BoardLib.inf


[PcdsFixedAtBuild.common]

  #
  # LS1043a board Specific PCDs
  # XX (DRAM - Region 1 2GB - 66MB)
  # (NOR - IFC Region 1 512MB)
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x7BE00000
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x02000000

  #
  # Board Specific Pcds
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x021c0500
  gNxpQoriqLsTokenSpaceGuid.PcdSerdes2Enabled|FALSE
  gNxpQoriqLsTokenSpaceGuid.PcdPlatformFreqDiv|0x1
  gNxpQoriqLsTokenSpaceGuid.PcdMuxToUsb3|TRUE

  #
  # Big Endian IPs
  #
  gNxpQoriqLsTokenSpaceGuid.PcdGurBigEndian|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdWdogBigEndian|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdIfcBigEndian|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdPciLutBigEndian|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdMmcBigEndian|TRUE

  #
  # I2C controller Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdI2cBus|0
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSpeed|100000
  gNxpQoriqLsTokenSpaceGuid.PcdSysEepromI2cBus|0
  gNxpQoriqLsTokenSpaceGuid.PcdSysEepromI2cAddress|0x53

  #
  # NV Storage PCDs.
  #
  gArmTokenSpaceGuid.PcdVFPEnabled|1
  gNxpQoriqLsTokenSpaceGuid.PcdFlashDeviceBase64|0x060000000
  gNxpQoriqLsTokenSpaceGuid.PcdFlashReservedRegionBase64|0x60500000

  #
  # PCI PCDs.
  #
  gNxpQoriqLsTokenSpaceGuid.PcdPciDebug|FALSE
  gNxpQoriqLsTokenSpaceGuid.PcdPcieLutBase|0x10000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieLutDbg|0x7FC

  gNxpQoriqLsTokenSpaceGuid.PcdFmanFwFlashAddr|0x60900000

  #
  # DPAA1 Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1Initialize|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1DebugFlags|0x0
  gNxpQoriqLsTokenSpaceGuid.PcdFManFwFlashAddr|0x60900000

  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1UsedMemacsMask|0x13F
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanMdio1Addr|0x01AFC000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanMdio2Addr|0x01AFD000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanAddr|0x01a00000

  gNxpQoriqLsTokenSpaceGuid.PcdFdtAddress|0x60F00000

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform
#
################################################################################
[Components.common]
  #
  # Architectural Protocols
  #

  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf{
     <LibraryClasses>
     NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
  }
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf

  Silicon/NXP/Drivers/WatchDog/WatchDogDxe.inf
  Silicon/NXP/Drivers/I2cDxe/I2cDxe.inf
  Silicon/NXP/Drivers/NorFlashDxe/NorFlashDxe.inf
  Silicon/NXP/Drivers/PciCpuIo2Dxe/PciCpuIo2Dxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <PcdsFixedAtBuild>
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x8010004F
  }
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  Silicon/NXP/Drivers/LanIntelE1000Dxe/LanIntelE1000Dxe.inf

  Silicon/NXP/Drivers/MmcHostDxe/MmcHostDxe.inf
  Silicon/NXP/Drivers/NandFlashDxe/NandFlashDxe.inf
  Silicon/NXP/Drivers/RngDxe/RngDxe.inf
  Silicon/NXP/Drivers/Dpaa1EthernetDxe/Dpaa1EthernetDxe.inf

  Platform/NXP/LS1043aRdbPkg/DeviceTree/DeviceTree.inf
  Silicon/NXP/Drivers/DtPlatformDxe/DtPlatformDxe.inf

  #
  # Networking stack
  #
!include NetworkPkg/Network.dsc.inc


  Silicon/NXP/Drivers/Dpaa1Ethernet/Dpaa1EthernetDxe.inf

  Platform/NXP/LS1043aRdbPkg/Drivers/Ds1307RealTimeClockRuntimeDxe/Ds1307RealTimeClockRuntimeDxe.inf
  Platform/NXP/LS1043aRdbPkg/Drivers/Pcf85263RealTimeClockRuntimeDxe/Pcf85263RealTimeClockRuntimeDxe.inf
