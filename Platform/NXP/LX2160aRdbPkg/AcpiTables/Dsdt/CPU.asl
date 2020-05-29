/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
  Copyright 2017-2018, 2020 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/

**/

/* acpi object tree
	/root
	  | - (_PR)
		| Processors (CPU 0-7)
*/

Scope(_SB)
{
    //
    // A72x16 Processor core declaration
    //
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

    Device (CLU0) { // Cluster0 state
      Name(_HID, "ACPI0010")
      Name(_UID, 1)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          6000, // Min residency (uS)
          2000, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        1, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x0,                // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CPU0) { // A72-0: Cluster 0, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 0)
        Method (_STA) {
          Return (0x0F)
        }
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK1CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK1CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_4, CPU_CGA_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_0, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
      Device(CPU1) { // A72-1: Cluster 0, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 1)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK1CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK1CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_4, CPU_CGA_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_0, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
    }

    Device (CLU1) { // Cluster1 state
      Name(_HID, "ACPI0010")
      Name(_UID, 2)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          6000, // Min residency (uS)
          2000, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        1, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x0,                // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CPU2) { // A72-0: Cluster 1, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 2)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK2CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK2CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_4, CPU_CGA_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_1, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
      Device(CPU3) { // A72-1: Cluster 1, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 3)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK2CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK2CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_4, CPU_CGA_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_1, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
    }

    Device (CLU2) { // Cluster2 state
      Name(_HID, "ACPI0010")
      Name(_UID, 3)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          6000, // Min residency (uS)
          2000, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        1, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x0,                // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CPU4) { // A72-0: Cluster 2, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 4)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK3CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK3CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_4, CPU_CGA_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_2, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
      Device(CPU5) { // A72-1: Cluster 2, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 5)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK3CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK3CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_4, CPU_CGA_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_2, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
    }

    Device (CLU3) { // Cluster3 state
      Name(_HID, "ACPI0010")
      Name(_UID, 4)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          6000, // Min residency (uS)
          2000, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        1, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x0,                // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CPU6) { // A72-0: Cluster 3, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 6)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK4CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK4CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_4, CPU_CGA_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_3, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
      Device(CPU7) { // A72-1: Cluster 3, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 7)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK4CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK4CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_1, CPU_CGA_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_2, CPU_CGA_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGA_PLL_DIV_4, CPU_CGA_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_3, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
    }

    Device (CLU4) { // Cluster4 state
      Name(_HID, "ACPI0010")
      Name(_UID, 5)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          6000, // Min residency (uS)
          2000, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        1, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x0,                // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CPU8) { // A72-0: Cluster 4, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 8)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK5CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK5CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_1, CPU_CGB_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_2, CPU_CGB_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_4, CPU_CGB_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_4, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
      Device(CPU9) { // A72-1: Cluster 4, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 9)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK5CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK5CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_1, CPU_CGB_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_2, CPU_CGB_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_4, CPU_CGB_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_4, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
    }

    Device (CLU5) { // Cluster5 state
      Name(_HID, "ACPI0010")
      Name(_UID, 6)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          6000, // Min residency (uS)
          2000, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        1, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x0,                // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CP10) { // A72-0: Cluster 5, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 10)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK6CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK6CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_1, CPU_CGB_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_2, CPU_CGB_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_4, CPU_CGB_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_5, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
      Device(CP11) { // A72-1: Cluster 5, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 11)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK6CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK6CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_1, CPU_CGB_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_2, CPU_CGB_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_4, CPU_CGB_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_5, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
    }

    Device (CLU6) { // Cluster6 state
      Name(_HID, "ACPI0010")
      Name(_UID, 7)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          6000, // Min residency (uS)
          2000, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        1, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x0,                // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CP12) { // A72-0: Cluster 6, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 12)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK7CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK7CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_1, CPU_CGB_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_2, CPU_CGB_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_4, CPU_CGB_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_6, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
      Device(CP13) { // A72-1: Cluster 6, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 13)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK7CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK7CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_1, CPU_CGB_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_2, CPU_CGB_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_4, CPU_CGB_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_6, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
    }

    Device (CLU7) { // Cluster7 state
      Name(_HID, "ACPI0010")
      Name(_UID, 8)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          6000, // Min residency (uS)
          2000, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        1, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x0,                // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CP14) { // A72-0: Cluster 7, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 14)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK8CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK8CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_1, CPU_CGB_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_2, CPU_CGB_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_4, CPU_CGB_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_7, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
      Device(CP15) { // A72-1: Cluster 7, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 15)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
        Name(_PTC, Package () {
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK8CSR_BASE,
                              CPU_DWORD_ACCESS)},
          ResourceTemplate() {Register(SystemMemory, 32, 0, CPU_CLK8CSR_BASE,
                              CPU_DWORD_ACCESS)}
        })
        Name(_TSS, Package() {
          Package() {CPU_FREQ_100_PRCNT, CPU_PWR_DISSP_PLL_DIV_1,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_1, CPU_CGB_PLL_DIV_1},
          Package() {CPU_FREQ_50_PRCNT, CPU_PWR_DISSP_PLL_DIV_2,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_2, CPU_CGB_PLL_DIV_2},
          Package() {CPU_FREQ_25_PRCNT, CPU_PWR_DISSP_PLL_DIV_4,
                     CPU_PLL_TRNS_LATENCY, CPU_CGB_PLL_DIV_4, CPU_CGB_PLL_DIV_4}
        })
        Method(_TPC, 0) {
          Return (Zero)
        }
        Name (_TSD, Package() {
          Package(){CPU_NUM_ENTRIES, 0, CPU_DOMAIN_7, CPU_SW_ANY_CORD,
                    CPU_NUM_PROCESSOR}
        })
      }
    }
}
