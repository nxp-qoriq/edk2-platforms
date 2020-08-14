/** @file

  Fixup the ICIDs of legacy devices in DCFG space and create
  iommus property in corresponding device's Device tree node.

  Copyright 2020 PureSoftware

  SPDX-License-Identifier: BSD-2-Clause

**/
#include "Platform.h"

#include <AcpiTableInclude/Icid.c>
