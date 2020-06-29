/** @file
*  DSDT : Basic DSDT for Bootup
*
*  Copyright (c) 2020, Puresoftware Ltd. All rights reserved.
*
* SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <AcpiTables/Platform.h>

DefinitionBlock("DsdtTable.aml", "DSDT", 1, "NXPLTD", "LX2160A ", EFI_ACPI_ARM_OEM_REVISION) {
  include ("AcpiTables/Dsdt/CPU.asl")
}
