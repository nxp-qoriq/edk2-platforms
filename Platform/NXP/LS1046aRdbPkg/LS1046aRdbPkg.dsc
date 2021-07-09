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
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = TRUE
  DEFINE NETWORK_ALLOW_HTTP_CONNECTIONS = TRUE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE
  DEFINE CAPSULE_ENABLE                 = TRUE
  DEFINE X64EMU_ENABLE                  = FALSE
  DEFINE AARCH64_GOP_ENABLE             = FALSE

!include Platform/NXP/NxpQoriqLs.dsc
!include Silicon/NXP/Chassis/Chassis2/Chassis2.dsc
!include Silicon/NXP/LS1046A/LS1046A.dsc

!if $(DYNAMIC_ACPI_ENABLE) == TRUE
  !include DynamicTablesPkg/DynamicTables.dsc.inc
!endif

[LibraryClasses.common]
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  ArmPlatformLib|Platform/NXP/LS1046aRdbPkg/Library/PlatformLib/ArmPlatformLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  PL011UartLib|ArmPlatformPkg/Library/PL011UartLib/PL011UartLib.inf
  SerialPortLib|Silicon/NXP/Library/DUartPortLib/DUartPortLib.inf
  SocLib|Silicon/NXP/Chassis/LS1046aSocLib.inf
  RealTimeClockLib|Silicon/NXP/Library/Pcf2129RtcLib/Pcf2129RtcLib.inf
  IfcLib|Silicon/NXP/Library/IfcLib/IfcLib.inf
  BoardLib|Platform/NXP/LS1046aRdbPkg/Library/BoardLib/BoardLib.inf
  FpgaLib|Platform/NXP/LS1046aRdbPkg/Library/FpgaLib/FpgaLib.inf
  IortLib|Silicon/NXP/LS1046A/Library/IortLib/IortLib.inf
  PciSegmentLib|Silicon/NXP/Library/PciSegmentLib/PciSegmentLib.inf
  PciHostBridgeLib|Silicon/NXP/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  MmcLib|Silicon/NXP/Library/MmcLib/MmcLib.inf
  SecureMonRngLib|Silicon/NXP/Library/SecureMonRngLib/SecureMonRngLib.inf
  MemoryInitPeiLib|Silicon/NXP/Library/MemoryInitPei/MemoryInitPeiLib.inf
  PlatformASLTablesLib|Platform/NXP/LS1046aRdbPkg/PlatformASLTablesLib/PlatformASLTablesLib.inf

  #
  # DPAA1
  #
  Dpaa1Lib|Silicon/NXP/Library/Dpaa1Lib/Dpaa1Lib.inf
  Dpaa1EthernetMacLib|Silicon/NXP/Library/Dpaa1EthernetMacLib/Dpaa1EthernetMacLib.inf
  Dpaa1EthernetPhyLib|Silicon/NXP/Library/Dpaa1EthernetPhyLib/Dpaa1EthernetPhyLib.inf
  Dpaa1BoardLib|Platform/NXP/LS1046aRdbPkg/Library/Dpaa1BoardLib/Dpaa1BoardLib.inf

  #
  # USB Requirements
  #
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf

[PcdsFeatureFlag.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

[PcdsFixedAtBuild.common]

  #
  # LS1046a board Specific PCDs
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
  gNxpQoriqLsTokenSpaceGuid.PcdI2cBus|3

  gNxpQoriqLsTokenSpaceGuid.PcdSysEepromI2cBus|0
  gNxpQoriqLsTokenSpaceGuid.PcdSysEepromI2cAddress|0x53

  #
  # RTC Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSlaveAddress|0x51
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSpeed|100000

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
  gNxpQoriqLsTokenSpaceGuid.PcdSataErratumA010554|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdSataErratumA010635|TRUE

  gNxpQoriqLsTokenSpaceGuid.PcdFmanFwFlashAddr|0x40900000

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

  Silicon/NXP/Drivers/SataInitDxe/SataInitDxe.inf

  #
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

  Platform/NXP/LS1046aRdbPkg/DeviceTree/DeviceTree.inf
  Silicon/NXP/Drivers/MmcHostDxe/MmcHostDxe.inf
  Silicon/NXP/Drivers/NandFlashDxe/NandFlashDxe.inf
  Silicon/NXP/Drivers/RngDxe/RngDxe.inf

  Silicon/NXP/Drivers/Dpaa1EthernetDxe/Dpaa1EthernetDxe.inf

  #
  # Acpi Support
  #
  Silicon/NXP/Drivers/NxpAcpiPlatformDxe/AcpiPlatformDxe.inf

  #
  # Platform
  #
!if $(DYNAMIC_ACPI_ENABLE) == FALSE
  Platform/NXP/LS1046aRdbPkg/AcpiTables/AcpiTables.inf
!endif
  Platform/NXP/LS1046aRdbPkg/AcpiTables/Icid.inf

  #
  # SMBIOS
  #
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
!if $(DYNAMIC_ACPI_ENABLE) == FALSE
  Platform/NXP/LS1046aRdbPkg/SmbiosPlatformDxe/SmbiosPlatformDxe.inf
!endif

!if $(CAPSULE_ENABLE)
  #
  # Firmware update
  #
  Platform/NXP/LS1046aRdbPkg/SystemFirmwareDescriptor/SystemFirmwareDescriptor.inf
!endif #$(CAPSULE_ENABLE)

  #
  # GOP Support
  #
  edk2-non-osi/Drivers/OptionRomPkg/AMDGop/AMDGop.inf

  #
  # X86 Emulation Support
  #
  edk2-non-osi/Emulator/X86EmulatorDxe/X86EmulatorDxe.inf

!if $(DYNAMIC_ACPI_ENABLE) == TRUE
  #
  # Dynamic Table Factory Dxe
  #
  DynamicTablesPkg/Drivers/DynamicTableFactoryDxe/DynamicTableFactoryDxe.inf {
  <LibraryClasses>
    NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiDbg2LibArm/AcpiDbg2LibArm.inf
    NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiFadtLibArm/AcpiFadtLibArm.inf
    NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiGtdtLibArm/AcpiGtdtLibArm.inf
    NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiIortLibArm/AcpiIortLibArm.inf
    NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiMadtLibArm/AcpiMadtLibArm.inf
    NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiMcfgLibArm/AcpiMcfgLibArm.inf
    NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiRawLibArm/AcpiRawLibArm.inf
    NULL|DynamicTablesPkg/Library/Acpi/Arm/AcpiSpcrLibArm/AcpiSpcrLibArm.inf

    #
    # SSDT fixup
    #
    NULL|DynamicTablesPkg/Library/Acpi/Nxp/AcpiSsdtLib/AcpiSsdtLib.inf

    #
    # SMBIOS
    #
    #
    # OEM Specific Generator for Smbios Type0 Table
    #
    NULL|Platform/NXP/LS1046aRdbPkg/PlatformSmbiosTablesLib/SmbiosType0GeneratorLib/PlatformSmbiosType0Lib.inf
    #
    # Default ARM Generators for other Smbios Tables
    #
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType1LibArm/DefaultType1LibArm.inf
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType3LibArm/DefaultType3LibArm.inf
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType4LibArm/DefaultType4LibArm.inf
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType7LibArm/DefaultType7LibArm.inf
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType9LibArm/DefaultType9LibArm.inf
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType16LibArm/DefaultType16LibArm.inf
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType17LibArm/DefaultType17LibArm.inf
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType19LibArm/DefaultType19LibArm.inf
    NULL|DynamicTablesPkg/Library/Smbios/Arm/DefaultType32LibArm/DefaultType32LibArm.inf
  }
!endif
 ##
