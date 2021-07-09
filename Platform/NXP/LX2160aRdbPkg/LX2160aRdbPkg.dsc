#  LX2160aRdbPkg.dsc
#
#  LX2160ARDB Board package.
#
#  Copyright 2018-2020 NXP
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
  PLATFORM_NAME                  = LX2160aRdbPkg
  PLATFORM_GUID                  = be06d8bc-05eb-44d6-b39f-191e93617ebd
  OUTPUT_DIRECTORY               = Build/LX2160aRdbPkg
  FLASH_DEFINITION               = Platform/NXP/LX2160aRdbPkg/LX2160aRdbPkg.fdf
  DEFINE MC_HIGH_MEM             = TRUE
  DEFINE CAPSULE_ENABLE          = FALSE
  DEFINE X64EMU_ENABLE           = FALSE
  DEFINE AARCH64_GOP_ENABLE      = FALSE

  #
  # Network definition
  #
  DEFINE NETWORK_TLS_ENABLE             = FALSE
  DEFINE NETWORK_HTTP_BOOT_ENABLE       = TRUE
  DEFINE NETWORK_ISCSI_ENABLE           = FALSE
  DEFINE NETWORK_ALLOW_HTTP_CONNECTIONS = TRUE

!include Platform/NXP/NxpQoriqLs.dsc
!include Silicon/NXP/Chassis/Chassis3V2/Chassis3V2.dsc
!include Silicon/NXP/LX2160A/LX2160A.dsc

!if $(DYNAMIC_ACPI_ENABLE) == TRUE
  !include DynamicTablesPkg/DynamicTables.dsc.inc
!endif

[LibraryClasses.common]
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  ArmPlatformLib|Platform/NXP/LX2160aRdbPkg/Library/PlatformLib/ArmPlatformLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  PL011UartLib|ArmPlatformPkg/Library/PL011UartLib/PL011UartLib.inf
  PL011UartClockLib|Silicon/NXP/Library/PL011UartClockLib/PL011UartClockLib.inf
  SerialPortLib|ArmPlatformPkg/Library/PL011SerialPortLib/PL011SerialPortLib.inf
  SocLib|Silicon/NXP/Chassis/LX2160aSocLib.inf
  RealTimeClockLib|Silicon/NXP/Library/Pcf2129RtcLib/Pcf2129RtcLib.inf
  FpgaLib|Platform/NXP/LX2160aRdbPkg/Library/FpgaLib/FpgaLib.inf
  IortLib|Silicon/NXP/LX2160A/Library/IortLib/IortLib.inf
  PciSegmentLib|Silicon/NXP/Library/PciSegmentLib/PciSegmentLib.inf
  PciHostBridgeLib|Silicon/NXP/Library/PciHostBridgeLib/PciHostBridgeLib.inf
  MmcLib|Silicon/NXP/Library/MmcLib/MmcLib.inf
  ItbParseLib|Silicon/NXP/Library/ItbParseLib/ItbParse.inf
  Dpaa2BoardSpecificLib|Platform/NXP/LX2160aRdbPkg/Library/Dpaa2BoardSpecificLib/Dpaa2BoardSpecificLib.inf
  Dpaa2EthernetMacLib|Silicon/NXP/Library/Dpaa2EthernetMacLib/Dpaa2EthernetMacLib.inf
  Dpaa2EthernetPhyLib|Silicon/NXP/Library/Dpaa2EthernetPhyLib/Dpaa2EthernetPhyLib.inf
  Dpaa2ManagementComplexLib|Silicon/NXP/Library/Dpaa2ManagementComplexLib/Dpaa2ManagementComplexLib.inf
  Dpaa2McInterfaceLib|Silicon/NXP/Library/Dpaa2McInterfaceLib/Dpaa2McInterfaceLib.inf
  SecureMonRngLib|Silicon/NXP/Library/SecureMonRngLib/SecureMonRngLib.inf
  MemoryInitPeiLib|Silicon/NXP/Library/MemoryInitPei/MemoryInitPeiLib.inf
  PlatformASLTablesLib|Platform/NXP/LX2160aRdbPkg/PlatformASLTablesLib/PlatformASLTablesLib.inf

[PcdsFeatureFlag.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

[PcdsFixedAtBuild.common]

!if $(MC_HIGH_MEM) == TRUE                                        # Management Complex loaded at the end of DDR2
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McHighRamSize|0x80000000      # 2GB (must be 512MB aligned)
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McLowRamSize|0x0
  gNxpQoriqLsTokenSpaceGuid.PcdMcHighMemSupport|1
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x0080000000             # Actual base
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x007BE00000             # 2G - 66MB (ATF)
!else
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McHighRamSize|0x0             # 512MB (Fixed)
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McLowRamSize|0x20000000       # 512MB (Fixed)
  gNxpQoriqLsTokenSpaceGuid.PcdMcHighMemSupport|0
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x0080000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x0040000000             # 2G - 512MB - 66MB (ATF), 512 MB aligned
!endif
  gArmPlatformTokenSpaceGuid.PcdSystemMemoryUefiRegionSize|0x02000000
  gEfiSecurityPkgTokenSpaceGuid.PcdOptionRomImageVerificationPolicy|0x00000001
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiExposedTableVersions|0x20

  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion|0x0320

  #
  # Board Specific Pcds
  #

  gNxpQoriqLsTokenSpaceGuid.PcdIn112525FwNorBaseAddr|0x20980000
  gNxpQoriqLsTokenSpaceGuid.PcdIn112525FwSize|0x40000

  # ARM SBSA WDT
  gArmTokenSpaceGuid.PcdGenericWatchdogControlBase|0x23A0000
  gArmTokenSpaceGuid.PcdGenericWatchdogRefreshBase|0x2390000
  gArmTokenSpaceGuid.PcdGenericWatchdogEl2IntrNum|91

  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x21C0000
  gArmPlatformTokenSpaceGuid.PcdSerialDbgRegisterBase|0x21D0000
  gArmPlatformTokenSpaceGuid.PL011UartClkInHz|175000000
  gNxpQoriqLsTokenSpaceGuid.PcdSerdes2Enabled|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdPlatformFreqDiv|0x4
  gNxpQoriqLsTokenSpaceGuid.PcdDdrClk|100000000

  #
  # RTC Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdI2cBus|4
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSpeed|100000
  gNxpQoriqLsTokenSpaceGuid.PcdI2cSlaveAddress|0x51

  gNxpQoriqLsTokenSpaceGuid.PcdSysEepromI2cBus|0
  gNxpQoriqLsTokenSpaceGuid.PcdSysEepromI2cAddress|0x57

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
  gNxpQoriqLsTokenSpaceGuid.PcdPcieExp1SysAddr|0x3400000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieExp2SysAddr|0x3500000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieExp3SysAddr|0x3600000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieExp4SysAddr|0x3700000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieExp5SysAddr|0x3800000
  gNxpQoriqLsTokenSpaceGuid.PcdPcieExp6SysAddr|0x3900000

  #
  # SATA Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdSataErratumA009185|TRUE

  #
  # DPAA2 Pcds
  #
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2Initialize|TRUE
  gNxpQoriqLsTokenSpaceGuid.PcdDisableMcLogging|FALSE
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McFwSrc|0x01
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McBootTimeoutMs|200000
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2UsedDpmacsMask|0xff00ff
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McLogMcDramOffset|0x1000000

  # Valid values for PcdDpaa2McLogLevel:
  # - 0x01: LOG_LEVEL_DEBUG
  # - 0x02: LOG_LEVEL_INFO
  # - 0x03: LOG_LEVEL_WARNING
  # - 0x04: LOG_LEVEL_ERROR
  # - 0x05: LOG_LEVEL_CRITICAL
  # - 0x06: LOG_LEVEL_ASSERT
  # - 0xFF: LOG_LEVEL_DEFAULT (default from DPC)
  gNxpQoriqLsTokenSpaceGuid.PcdDpaa2McLogLevel|0xff

  # Valid values for PcdDpaaDebugFlags:
  # - 0x0      DPAA debug logs are disabled.
  # - 0x1      Enable DPAA debugging messages
  # - 0x2      Dump values of RAM words or registers
  # - 0x4      Trace commands sent to the MC
  # - 0x8      Dump MC log fragment
  # - 0x10     Dump contents of the root DPRC
  # - 0x20     Perform extra checks
  # - 0x40     Trace network packets sent/received
  gNxpQoriqLsTokenSpaceGuid.PcdDpaaDebugFlags|0x0

  gNxpQoriqLsTokenSpaceGuid.PcdFdtAddress|0x20F00000
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

  ArmPkg/Drivers/GenericWatchdogDxe/GenericWatchdogDxe.inf
  Silicon/NXP/Drivers/I2cDxe/I2cDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  Silicon/NXP/Drivers/UsbHcdInitDxe/UsbHcd.inf
  Silicon/NXP/Drivers/PciCpuIo2Dxe/PciCpuIo2Dxe.inf
  Silicon/NXP/Library/Pcf2129RtcLib/Pcf2129RtcLib.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <PcdsFixedAtBuild>
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x8010004F
  }
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

  Silicon/NXP/Drivers/LanIntelE1000Dxe/LanIntelE1000Dxe.inf

  #
  # Networking stack
  #
!include NetworkPkg/Network.dsc.inc

  Silicon/NXP/Drivers/SataInitDxe/SataInitDxe.inf
  Silicon/NXP/Drivers/MmcHostDxe/MmcHostDxe.inf
  Silicon/NXP/Drivers/Dpaa2EthernetDxe/Dpaa2EthernetDxe.inf
  Silicon/NXP/Drivers/RngDxe/RngDxe.inf

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
  Platform/NXP/LX2160aRdbPkg/DeviceTree/DeviceTree.inf

  Silicon/NXP/Drivers/FlexSpiDxe/FspiDxe.inf
  Silicon/NXP/Drivers/SpiBusDxe/SpiBusDxe.inf
  Silicon/NXP/Drivers/SpiNorFlashDxe/SpiNorFlashDxe.inf
  Silicon/NXP/Drivers/SpiConfigurationDxe/SpiConfigurationDxe.inf

  #
  # Acpi Support
  #
  Silicon/NXP/Drivers/NxpAcpiPlatformDxe/AcpiPlatformDxe.inf

  #
  # Platform
  #
!if $(DYNAMIC_ACPI_ENABLE) == FALSE
  Platform/NXP/LX2160aRdbPkg/AcpiTables/AcpiTables.inf
!endif
  Platform/NXP/LX2160aRdbPkg/AcpiTables/Icid.inf

 #
 #SMBIOS
 #
 MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
!if $(DYNAMIC_ACPI_ENABLE) == FALSE
 Platform/NXP/LX2160aRdbPkg/SmbiosPlatformDxe/SmbiosPlatformDxe.inf
!endif

!if $(CAPSULE_ENABLE)
  #
  # Firmware update
  #
  Platform/NXP/LX2160aRdbPkg/SystemFirmwareDescriptor/SystemFirmwareDescriptor.inf
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
    NULL|Platform/NXP/LX2160aRdbPkg/PlatformSmbiosTablesLib/SmbiosType0GeneratorLib/PlatformSmbiosType0Lib.inf
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
 ##
