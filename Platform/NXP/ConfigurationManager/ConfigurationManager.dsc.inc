## @file
#  dsc include file for Configuration Manager
#
#  Copyright (c) 2020 Puresoftware Ltd. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]

[BuildOptions]

[LibraryClasses.common]

[Components.common]
  # Configuration Manager
  Platform/NXP/ConfigurationManager/ConfigurationManagerDxe/ConfigurationManagerDxe.inf {
    <LibraryClasses>
    # Platform ASL Tables
    # PlatformAslTablesLib|Platform/NXP/$(DYNAMIC_ACPI_PLATFORM)/ConfigurationManager/PlatformASLTablesLib/PlatformASLTablesLib.inf
  <BuildOptions>
   *_*_*_PLATFORM_FLAGS = -I$(BIN_DIR)/Platform/NXP/$(DYNAMIC_ACPI_PLATFORM)/PlatformASLTablesLib/PlatformASLTablesLib/OUTPUT
   *_*_*_PLATFORM_FLAGS = -I$(PACKAGES_PATH)/Platform/NXP/$(DYNAMIC_ACPI_PLATFORM)
   *_*_*_PLATFORM_FLAGS = -I$(PACKAGES_PATH)/Silicon/NXP/$(DYNAMIC_ACPI_INC)/Include
  }
