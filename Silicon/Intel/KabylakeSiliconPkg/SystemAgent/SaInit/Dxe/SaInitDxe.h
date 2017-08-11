/** @file
  Header file for SA Initialization Driver.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under
the terms and conditions of the BSD License that accompanies this distribution.
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#ifndef _SA_INITIALIZATION_DXE_DRIVER_H_
#define _SA_INITIALIZATION_DXE_DRIVER_H_

#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiTable.h>
#include "VTd.h"
#include "IgdOpRegion.h"
#include "GraphicsInit.h"
#include <Library/HobLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <SiConfigHob.h>
#include <Library/CpuPlatformLib.h>
#include <Library/MmPciLib.h>

///
/// Driver Consumed Protocol Prototypes
///
#include <Protocol/SaPolicy.h>

extern EFI_GUID gSaSsdtAcpiTableStorageGuid;

/**
  <b>System Agent Initialization DXE Driver Entry Point</b>
  - <b>Introduction</b> \n
    Based on the information/data in SA_POLICY_PROTOCOL, this module performs further SA initialization in DXE phase,
    e.g. internal devices enable/disable, SSVID/SID programming, graphic power-management, VT-d, IGD OpRegion initialization.
    From the perspective of a PCI Express hierarchy, the Broadwell System Agent and PCH together appear as a Root Complex with root ports the number of which depends on how the 8 PCH ports and 4 System Agent PCIe ports are configured [4x1, 2x8, 1x16].
    There is an internal link (DMI or OPI) that connects the System Agent to the PCH component. This driver includes initialization of SA DMI, PCI Express, SA & PCH Root Complex Topology.
    For iGFX, this module implements the initialization of the Graphics Technology Power Management from the Broadwell System Agent BIOS Specification and the initialization of the IGD OpRegion/Software SCI - BIOS Specification.
    The ASL files that go along with the driver define the IGD OpRegion mailboxes in ACPI space and implement the software SCI interrupt mechanism.
    The IGD OpRegion/Software SCI code serves as a communication interface between system BIOS, ASL, and Intel graphics driver including making a block of customizable data (VBT) from the Intel video BIOS available.
    Reference Code for the SCI service functions "Get BIOS Data" and "System BIOS Callback" can be found in the ASL files, those functions can be platform specific, the sample provided in the reference code are implemented for Intel CRB.
    This module implements the VT-d functionality described in the Broadwell System Agent BIOS Specification.
    This module publishes the LegacyRegion protocol to control the read and write accesses to the Legacy BIOS ranges.
    E000 and F000 segments are the legacy BIOS ranges and contain pointers to the ACPI regions, SMBIOS tables and so on. This is a private protocol used by Intel Framework.
    This module registers CallBack function that performs SA security registers lockdown at end of post as required from Broadwell Bios Spec.
    In addition, this module publishes the SaInfo Protocol with information such as current System Agent reference code version#.

  - @pre
    - EFI_FIRMWARE_VOLUME_PROTOCOL: Documented in Firmware Volume Specification, available at the URL: http://www.intel.com/technology/framework/spec.htm
    - SA_POLICY_PROTOCOL: A protocol published by a platform DXE module executed earlier; this is documented in this document as well.
    - EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL: Documented in the Unified Extensible Firmware Interface Specification, version 2.0, available at the URL: http://www.uefi.org/specs/
    - EFI_BOOT_SCRIPT_SAVE_PROTOCOL: A protocol published by a platform DXE module executed earlier; refer to the Sample Code section of the Framework PCH Reference Code.
    - EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL: Documented in the Unified Extensible Firmware Interface Specification, version 2.0, available at the URL: http://www.uefi.org/specs/
    - EFI_ACPI_TABLE_PROTOCOL : Documented in PI Specification 1.2
    - EFI_CPU_IO_PROTOCOL: Documented in CPU I/O Protocol Specification, available at the URL: http://www.intel.com/technology/framework/spec.htm
    - EFI_DATA_HUB_PROTOCOL: Documented in EFI Data Hub Infrastructure Specification, available at the URL: http://www.intel.com/technology/framework/spec.htm
    - EFI_HII_PROTOCOL (or EFI_HII_DATABASE_PROTOCOL for UEFI 2.1): Documented in Human Interface Infrastructure Specification, available at the URL: http://www.intel.com/technology/framework/spec.htm
    (For EFI_HII_DATABASE_PROTOCOL, refer to UEFI Specification Version 2.1 available at the URL: http://www.uefi.org/)

  - @result
    IGD power-management functionality is initialized;  VT-d is initialized (meanwhile, the DMAR table is updated); IGD OpRegion is initialized - IGD_OPREGION_PROTOCOL installed, IGD OpRegion allocated and mailboxes initialized, chipset initialized and ready to generate Software SCI for Internal graphics events. Publishes the SA_INFO_PROTOCOL with current SA reference code version #. Publishes the EFI_LEGACY_REGION_PROTOCOL documented in the Compatibility Support Module Specification, version 0.9, available at the URL: http://www.intel.com/technology/framework/spec.htm

  - <b>References</b> \n
    IGD OpRegion/Software SCI for Broadwell
    Advanced Configuration and Power Interface Specification Revision 4.0a.

  - <b>Porting Recommendations</b> \n
    No modification of the DXE driver should be typically necessary.
    This driver should be executed after all related devices (audio, video, ME, etc.) are initialized to ensure correct data in DMAR table and DMA-remapping registers.

  @param[in] ImageHandle             Handle for the image of this driver
  @param[in] SystemTable             Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SaInitEntryPointDxe (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

/**
  SystemAgent Acpi Initialization.

  @param[in] ImageHandle             Handle for the image of this driver

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SaAcpiInit (
  IN EFI_HANDLE         ImageHandle
  );

#endif