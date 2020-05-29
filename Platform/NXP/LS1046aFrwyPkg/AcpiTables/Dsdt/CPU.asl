/** @file
*  DSDT: CPU Entries
*
*  Copyright 2019-2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
    //
    // A72x4 Processor core declaration
    //
    Device(CPU0) {
      Name(_HID, "ACPI0007")
      Name(_UID, 0)
      /*
      *   _PTC: Processor Throttling Control
      *   Package() {ControlRegister // Buffer (Resource Descriptor)
      *              StatusRegister // Buffer (Resource Descriptor)}
      */
      Name(_PTC, Package () {
        ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK_CSSR_REG,
                            CPU_DWORD_ACCESS)},
        ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK_CSSR_REG,
                            CPU_DWORD_ACCESS)}
        })
      /*
      *  _TSS: Throttling Supported States
      *  Package() {Percent, Power, Latency, Control, Status }
      */
      Name(_TSS, Package() {
        Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                   CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
        Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                   CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
      })
      /*
      *  _TPC: Throttling Present Capabilities
      *  0 – states 0 ... nth state available (all states available)
      *  1 – states 1 ... nth state available
      *  2 – states 2 ... nth state available
      */
      Method(_TPC, 0) {
        Return (Zero)
      }
      /*
      *  _TSD: T-State Dependency
      *  Package() {NumEntries, Revision, Domain, CoordType, NumProcessors}
      */
      Name (_TSD, Package() {
        Package() {CPU_NUM_ENTRIES, 0, CPU_DOMAIN_0, CPU_SW_ANY_CORD,
                  CPU_NUM_PROCESSOR}
      })
    }

    Device(CPU1) {
      Name(_HID, "ACPI0007")
      Name(_UID, 1)
      Name(_PTC, Package () {
        ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK_CSSR_REG,
                            CPU_DWORD_ACCESS)},
        ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK_CSSR_REG,
                            CPU_DWORD_ACCESS)}
      })
      Name(_TSS, Package() {
        Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                   CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
        Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                   CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
      })
      Method(_TPC, 0) {
        Return (Zero)
      }
      Name (_TSD, Package() {
        Package() {CPU_NUM_ENTRIES, 0, CPU_DOMAIN_0, CPU_SW_ANY_CORD,
                  CPU_NUM_PROCESSOR}
      })
    }

    Device(CPU2) {
      Name(_HID, "ACPI0007")
      Name(_UID, 2)
      Name(_PTC, Package () {
        ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK_CSSR_REG,
                            CPU_DWORD_ACCESS)},
        ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK_CSSR_REG,
                            CPU_DWORD_ACCESS)}
      })
      Name(_TSS, Package() {
        Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                   CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
        Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                   CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
      })
      Method(_TPC, 0) {
        Return (Zero)
      }
      Name (_TSD, Package() {
        Package() {CPU_NUM_ENTRIES, 0, CPU_DOMAIN_0, CPU_SW_ANY_CORD,
                  CPU_NUM_PROCESSOR}
      })
    }

    Device(CPU3) {
      Name(_HID, "ACPI0007")
      Name(_UID, 3)
      Name(_PTC, Package () {
        ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK_CSSR_REG,
                            CPU_DWORD_ACCESS)},
        ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK_CSSR_REG,
                            CPU_DWORD_ACCESS)}
      })
      Name(_TSS, Package() {
        Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                   CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
        Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                   CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
      })
      Method(_TPC, 0) {
        Return (Zero)
      }
      Name (_TSD, Package() {
        Package() {CPU_NUM_ENTRIES, 0, CPU_DOMAIN_0, CPU_SW_ANY_CORD,
                  CPU_NUM_PROCESSOR}
      })
    }
}
