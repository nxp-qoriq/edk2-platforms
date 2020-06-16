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
  include ("I2c.asl")
  include ("imx-wdt.asl")
  include ("Usb.asl")
  include ("SPI.asl")
  include ("Sata.asl")
  include ("esdhc.asl")
  include ("QSPI.asl")
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
