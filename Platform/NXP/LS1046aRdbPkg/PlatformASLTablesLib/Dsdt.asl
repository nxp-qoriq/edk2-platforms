/** @file
*  DSDT : Basic DSDT for Bootup
*
*  Copyright (c) 2020, Puresoftware Ltd. All rights reserved.
*
* SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <AcpiTables/Platform.h>

DefinitionBlock("DsdtTable.aml", "DSDT", 1, "NXP   ", "LS1046  ", EFI_ACPI_ARM_OEM_REVISION) {
   include ("AcpiTables/Dsdt/CPU.asl")
   include ("AcpiTables/Dsdt/Clk.asl")
   include ("AcpiTables/Dsdt/Com.asl")
   include ("AcpiTables/Dsdt/Pci.asl")
   include ("AcpiTables/Dsdt/I2c.asl")
   include ("AcpiTables/Dsdt/Imx-wdt.asl")
   include ("AcpiTables/Dsdt/Usb.asl")
   include ("AcpiTables/Dsdt/Qspi.asl")
   include ("AcpiTables/Dsdt/Spi.asl")
   include ("AcpiTables/Dsdt/Sata.asl")
   include ("AcpiTables/Dsdt/Esdhc.asl")
   include ("AcpiTables/Dsdt/Tmu.asl")
   include ("AcpiTables/Dsdt/Rcpm.asl")
   include ("AcpiTables/Dsdt/Fman.asl")
   include ("AcpiTables/Dsdt/Bman.asl")
   include ("AcpiTables/Dsdt/Qman.asl")
   include ("AcpiTables/Dsdt/Bportals.asl")
   include ("AcpiTables/Dsdt/Qportals.asl")
   include ("AcpiTables/Dsdt/Mdio.asl")
   include ("AcpiTables/Dsdt/Fman_port_10g_0.asl")
   include ("AcpiTables/Dsdt/Fman_port_10g_1.asl")
   include ("AcpiTables/Dsdt/Fman_port_1g_2.asl")
   include ("AcpiTables/Dsdt/Fman_port_1g_3.asl")
   include ("AcpiTables/Dsdt/Fman_port_1g_4.asl")
   include ("AcpiTables/Dsdt/Fman_port_1g_5.asl")
   include ("AcpiTables/Dsdt/Ftm.asl")

}
