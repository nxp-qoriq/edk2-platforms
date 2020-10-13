/** @file
*  DSDT : Basic DSDT for Bootup
*
*  Copyright (c) 2020, Puresoftware Ltd. All rights reserved.
*
* SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <AcpiTables/Platform.h>

DefinitionBlock("DsdtTable.aml", "DSDT", 1, "NXP   ", "LX2160  ", EFI_ACPI_ARM_OEM_REVISION) {
  include ("AcpiTables/Dsdt/Com.asl")
  include ("AcpiTables/Dsdt/CPU.asl")
  include ("AcpiTables/Dsdt/Clk.asl")
  include ("AcpiTables/Dsdt/Esdhc.asl")
  include ("AcpiTables/Dsdt/FSPI.asl")
  include ("AcpiTables/Dsdt/Guts.asl")
  include ("AcpiTables/Dsdt/I2c.asl")
  include ("AcpiTables/Dsdt/Mc.asl")
  include ("AcpiTables/Dsdt/Mdio.asl")
  include ("AcpiTables/Dsdt/Pci.asl")
  include ("AcpiTables/Dsdt/Sata.asl")
  include ("AcpiTables/Dsdt/SPI.asl")
  include ("AcpiTables/Dsdt/Usb.asl")
  include ("AcpiTables/Dsdt/Ftm.asl")
  include ("AcpiTables/Dsdt/Rcpm.asl")
  include ("AcpiTables/Dsdt/Tmu.asl")
}
