#  LS1046aFrwyPkg.dsc
#
#  LS1046AFRWY Board package.
#
#  Copyright 2019-2020 NXP
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
  PLATFORM_NAME                  = LS1046aFrwyPkg
  PLATFORM_GUID                  = 79adaa48-5f50-49f0-aa9a-544ac9260ef8
  OUTPUT_DIRECTORY               = Build/LS1046aFrwyPkg
  FLASH_DEFINITION               = Platform/NXP/LS1046aFrwyPkg/LS1046aFrwyPkg.fdf
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = TRUE
  DEFINE NETWORK_ALLOW_HTTP_CONNECTIONS = TRUE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE
  DEFINE CAPSULE_ENABLE                 = TRUE
  DEFINE X64EMU_ENABLE                  = FALSE
  DEFINE AARCH64_GOP_ENABLE             = FALSE
  DEFINE DYNAMIC_ACPI_ENABLE            = TRUE

!include Platform/NXP/NxpQoriqLs.dsc
!include Silicon/NXP/Chassis/Chassis2/Chassis2.dsc
!include Silicon/NXP/LS1046A/LS1046A.dsc

!if $(DYNAMIC_ACPI_ENABLE) == TRUE
  !include DynamicTablesPkg/DynamicTables.dsc.inc
  !include Platform/NXP/ConfigurationManager/ConfigurationManager.dsc.inc
!endif

[LibraryClasses.common]
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  ArmPlatformLib|Platform/NXP/LS1046aFrwyPkg/Library/PlatformLib/ArmPlatformLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  PL011UartLib|ArmPlatformPkg/Library/PL011UartLib/PL011UartLib.inf
  SerialPortLib|Silicon/NXP/Library/DUartPortLib/DUartPortLib.inf
  SocLib|Silicon/NXP/Chassis/LS1046aSocLib.inf
  RealTimeClockLib|Silicon/NXP/Library/Pcf2129RtcLib/Pcf2129RtcLib.inf
  IfcLib|Silicon/NXP/Library/IfcLib/IfcLib.inf
  BoardLib|Platform/NXP/LS1046aFrwyPkg/Library/BoardLib/BoardLib.inf
  FpgaLib|Platform/NXP/LS1046aFrwyPkg/Library/FpgaLib/FpgaLib.inf
  IortLib|Silicon/NXP/LS1046A/Library/IortLib/IortLib.inf
  PciSegmentLib|Silicon/NXP/Library/PciSegmentLib/PciSegmentLib.inf
  PciHostBridgeLib|Silicon/NXP/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  MmcLib|Silicon/NXP/Library/MmcLib/MmcLib.inf
  SecureMonRngLib|Silicon/NXP/Library/SecureMonRngLib/SecureMonRngLib.inf
  MemoryInitPeiLib|Silicon/NXP/Library/MemoryInitPei/MemoryInitPeiLib.inf
  PlatformASLTablesLib|Platform/NXP/LS1046aFrwyPkg/PlatformASLTablesLib/PlatformASLTablesLib.inf

  #
  # DPAA1
  #
  Dpaa1Lib|Silicon/NXP/Library/Dpaa1Lib/Dpaa1Lib.inf
  Dpaa1EthernetMacLib|Silicon/NXP/Library/Dpaa1EthernetMacLib/Dpaa1EthernetMacLib.inf
  Dpaa1EthernetPhyLib|Silicon/NXP/Library/Dpaa1EthernetPhyLib/Dpaa1EthernetPhyLib.inf
  Dpaa1BoardLib|Platform/NXP/LS1046aFrwyPkg/Library/Dpaa1BoardLib/Dpaa1BoardLib.inf

  #
  # USB Requirements
  #
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf


[PcdsFeatureFlag.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

[PcdsFixedAtBuild.common]

  #
  # FRWY-LS1046A board Specific PCDs
  # XX (DRAM - Region 1 2GB - 66 MB)
  # (NOR - IFC Region 1 512MB)
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x007BE00000
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x02000000
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiExposedTableVersions|0x20
  gEfiSecurityPkgTokenSpaceGuid.PcdOptionRomImageVerificationPolicy|0x00000001

  #
  # Board Specific Pcds
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x021c0500
  gNxpQoriqLsTokenSpaceGuid.PcdSerdes2Enabled|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdPlatformFreqDiv|0x2

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

  gNxpQoriqLsTokenSpaceGuid.PcdSysEepromI2cBus|0
  gNxpQoriqLsTokenSpaceGuid.PcdSysEepromI2cAddress|0x52

  #
  # RTC Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSlaveAddress|0x51
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSpeed|100000
  gNxpQoriqLsTokenSpaceGuid.PcdIsRtcDeviceMuxed|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdMuxDeviceAddress|0x77
  gNxpQoriqLsTokenSpaceGuid.PcdMuxControlRegOffset|0x00
  gNxpQoriqLsTokenSpaceGuid.PcdMuxRtcChannelValue|0x01
  gNxpQoriqLsTokenSpaceGuid.PcdMuxDefaultChannelValue|0x01

  #
  # PCI PCDs.
  #
  gNxpQoriqLsTokenSpaceGuid.PcdPciDebug|FALSE
  gNxpQoriqLsTokenSpaceGuid.PcdPcieLutBase|0x80000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieLutDbg|0x407FC

  gNxpQoriqLsTokenSpaceGuid.PcdFmanFwFlashAddr|0x40900000

 #
 # DPAA1 Pcds
 #
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1Initialize|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1DebugFlags|0x00
  gNxpQoriqLsTokenSpaceGuid.PcdFManFwFlashAddr|0x40900000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1UsedMemacsMask|0x231
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanMdio1Addr|0x01AFC000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanMdio2Addr|0x01AFD000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa1FmanAddr|0x01a00000
  gNxpQoriqLsTokenSpaceGuid.PcdFManFwFlashAddr|0x40900000
  gNxpQoriqLsTokenSpaceGuid.PcdSgmiiPrtclInit|TRUE

  gNxpQoriqLsTokenSpaceGuid.PcdFdtAddress|0x40F00000

  # DEFAULT_READY_WAIT_JIFFIES   40UL * HZ
  gNxpQoriqLsTokenSpaceGuid.PcdSpiNorPageProgramToutUs|40000

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
     NULL|EmbeddedPkg/Library/NvVarStoreFormattedLib/NvVarStoreFormattedLib.inf
  }
  #MdeModulePkg/Universal/Variable/EmuRuntimeDxe/EmuVariableRuntimeDxe.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf

  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  Silicon/NXP/Drivers/I2cDxe/I2cDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf
  Silicon/NXP/Drivers/UsbHcdInitDxe/UsbHcd.inf
  Silicon/NXP/Drivers/PciCpuIo2Dxe/PciCpuIo2Dxe.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <PcdsFixedAtBuild>
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x8010004F
  }
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  Silicon/NXP/Drivers/LanIntelE1000Dxe/LanIntelE1000Dxe.inf

  Silicon/NXP/Drivers/QspiDxe/QspiDxe.inf

  #
  # Networking stack
  #
!include NetworkPkg/Network.dsc.inc

  Silicon/NXP/Drivers/SpiBusDxe/SpiBusDxe.inf
  Silicon/NXP/Drivers/SpiNorFlashDxe/SpiNorFlashDxe.inf
  Silicon/NXP/Drivers/SpiConfigurationDxe/SpiConfigurationDxe.inf

  # DPAA1 Ethernet driver
  #
  Silicon/NXP/Drivers/Dpaa1Ethernet/Dpaa1EthernetDxe.inf

  # Platform DXE Driver
  Silicon/NXP/Drivers/PlatformDxe/PlatformDxe.inf

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

  Platform/NXP/LS1046aFrwyPkg/DeviceTree/DeviceTree.inf
  Silicon/NXP/Drivers/MmcHostDxe/MmcHostDxe.inf
  Silicon/NXP/Drivers/RngDxe/RngDxe.inf

!if $(DYNAMIC_ACPI_ENABLE) == FALSE
  Silicon/NXP/Drivers/Dpaa1EthernetDxe/Dpaa1EthernetDxe.inf
!endif

 ##
  #
  # Acpi Support
  #
  Silicon/NXP/Drivers/NxpAcpiPlatformDxe/AcpiPlatformDxe.inf

  #
  # Platform
  #
!if $(DYNAMIC_ACPI_ENABLE) == FALSE
  Platform/NXP/LS1046aFrwyPkg/AcpiTables/AcpiTables.inf
!endif
  Platform/NXP/LS1046aFrwyPkg/AcpiTables/Icid.inf

  #
  #SMBIOS
  #
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
!if $(CAPSULE_ENABLE)
  Platform/NXP/LS1046aFrwyPkg/SystemFirmwareDescriptor/SystemFirmwareDescriptor.inf
!endif

  #
  # GOP Support
  #
  edk2-non-osi/Drivers/OptionRomPkg/AMDGop/AMDGop.inf

  #
  # X86 Emulation Support
  #
  edk2-non-osi/Emulator/X86EmulatorDxe/X86EmulatorDxe.inf
 ##
