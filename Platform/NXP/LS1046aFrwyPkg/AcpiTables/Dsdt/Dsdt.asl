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
  include ("Clk.asl")
  include ("CPU.asl")
  include ("Com.asl")
  include ("Pci.asl")
  include ("I2c.asl")
  include ("Imx-wdt.asl")
  include ("Usb.asl")
  include ("Spi.asl")
  include ("Sata.asl")
  include ("Esdhc.asl")
  include ("Qspi.asl")
  include ("Tmu.asl")
  include ("Fman.asl")
  include ("Bman.asl")
  include ("Qman.asl")
  include ("Bportals.asl")
  include ("Qportals.asl")
  include ("Mdio.asl")
  include ("Fman_port_1g_0.asl")
  include ("Fman_port_1g_1.asl")
  include ("Fman_port_1g_2.asl")
  include ("Fman_port_1g_3.asl")
  include ("Rcpm.asl")
  include ("Ftm.asl")
}
