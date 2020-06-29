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
  Platform/NXP/LX2160aRdbPkg/ConfigurationManager/ConfigurationManagerDxe/ConfigurationManagerDxe.inf {
    <LibraryClasses>
    # Platform ASL Tables
    PlatformAslTablesLib|Platform/NXP/LX2160aRdbPkg/ConfigurationManager/PlatformASLTablesLib/PlatformASLTablesLib.inf
  <BuildOptions>
   *_*_*_PLATFORM_FLAGS = -I$(BIN_DIR)/Platform/NXP/LX2160aRdbPkg/ConfigurationManager/PlatformASLTablesLib/PlatformASLTablesLib/OUTPUT
  }
