/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
  Copyright 2017-2020 NXP
  Copyright 2020 Puresoftware Ltd

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Platform.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 1, "NXP  ", "LX2160  ", EFI_ACPI_ARM_OEM_REVISION) {
  include ("Com.asl")
  include ("CPU.asl")
  include ("Clk.asl")
  include ("Esdhc.asl")
  include ("FSPI.asl")
  include ("Guts.asl")
  include ("I2c.asl")
  include ("Mc.asl")
  include ("Mdio.asl")
  include ("Pci.asl")
  include ("Sata.asl")
  include ("SPI.asl")
  include ("Usb.asl")
  include ("Ftm.asl")
  include ("Rcpm.asl")
  include ("Tmu.asl")
}
