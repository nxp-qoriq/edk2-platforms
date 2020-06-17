/** @file
*  DSDT: CPU ACPI Information
*
*  Copyright 2019-2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

Scope(_SB)
{
  Method (_OSC, 4, Serialized)  { // _OSC: Operating System Capabilities
    CreateDWordField (Arg3, 0x00, STS0)
    CreateDWordField (Arg3, 0x04, CAP0)
    If ((Arg0 == ToUUID ("0811b06e-4a27-44f9-8d60-3cbbc22e7b48") /* Platform-wide Capabilities */)) {
      If (!(Arg1 == One)) {
        STS0 &= ~0x1F
        STS0 |= 0x0A
      } Else {
        If ((CAP0 & 0x100)) {
          CAP0 &= ~0x100 /* No support for OS Initiated LPI */
          STS0 &= ~0x1F
          STS0 |= 0x12
        }
      }
    } Else {
      STS0 &= ~0x1F
      STS0 |= 0x06
    }
    Return (Arg3)
  }

  Name(PLPI, Package() {
    0,                        // Version
    1,                        // Level Index
    2,                        // Count
    Package() {               // WFI for CPU
      1,                      // Min residency (uS)
      1,                      // Wake latency (uS)
      1,                      // Flags
      0,                      // Arch Context Flags
      100,                    //Residency Counter Frequency
      0,                      // No parent state
      ResourceTemplate () {
        Register (FFixedHW,   // Register Entry method
          0x20,               // Bit Width
          0x00,               // Bit Offset
          0xFFFFFFFF,         // Address
          0x03,               // Access Size
        )
      },
      ResourceTemplate() {    // Null Residency Counter
        Register (SystemMemory, 0, 0, 0, 0)
      },
      ResourceTemplate() {    // Null Usage Counter
        Register (SystemMemory, 0, 0, 0, 0)
      },
      "WFI",
    },
    Package() {               // Retention for CPU
      3000,                   // Min residency (uS)
      1000,                   // Wake latency (uS)
      1,                      // Flags
      0,                      // Arch Context Flags
      100,                    //Residency Counter Frequency
      0,                      // No parent state
      ResourceTemplate () {
        Register (FFixedHW,   // Register Entry method
          0x20,               // Bit Width
          0x00,               // Bit Offset
          0x01,               // Address
          0x03,               // Access Size
        )
      },
      ResourceTemplate() {    // Null Residency Counter
        Register (SystemMemory, 0, 0, 0, 0)
      },
      ResourceTemplate() {    // Null Usage Counter
        Register (SystemMemory, 0, 0, 0, 0)
      },
      "Retention",
    },
  })

  //
  // A72x4 Processor core declaration
  //
  Device(CPU0) {
    Name(_HID, "ACPI0007")
    Name(_UID, 0)
    Method (_LPI, 0, NotSerialized) {
      Return (PLPI)
    }
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
    Method (_LPI, 0, NotSerialized) {
      Return (PLPI)
    }
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
    Method (_LPI, 0, NotSerialized) {
      Return (PLPI)
    }
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
    Method (_LPI, 0, NotSerialized) {
      Return (PLPI)
    }
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
