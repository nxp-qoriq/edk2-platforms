/** @file
*  Differentiated System Description Table Fields (DSDT)
*  This is the top level file refrencing component asl files
*  which together form the DSDT.
*
*  Copyright 2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "Platform.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 1, "NXP  ", "LS1046  ", EFI_ACPI_ARM_OEM_REVISION) {
  include ("CPU.asl")
  include ("Clk.asl")
  include ("Com.asl")
  include ("Pci.asl")
  include ("imx-wdt.asl")
  include ("Usb.asl")
  include ("QSPI.asl")
  include ("SPI.asl")
}
