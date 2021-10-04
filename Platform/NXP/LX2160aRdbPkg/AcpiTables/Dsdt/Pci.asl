/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*  Copyright 2019-2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*  Based on the files under ArmPlatformPkg/ArmJunoPkg/AcpiTables/
*
**/

Scope(_SB)
{
  // PCIe Root bus
  Device (PCI0)
  {
    Name(_UID, 0x0)
    Name(_SUN, 0x0)
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, LX2160A_PCI_SEG0) // Segment of this Root complex
    Name(_BBN, 0) // Base Bus Number
    Name(_CCA, 1) // Cache Coherency Attribute
    Name (RBUF, ResourceTemplate () {
      WordBusNumber ( // Bus numbers assigned to this root
        ResourceProducer, MinFixed, MaxFixed, PosDecode,
        0,                           // AddressGranularity
        LX2160A_PCI_SEG_BUSNUM_MIN,  // AddressMinimum - Minimum Bus Number
        LX2160A_PCI_SEG_BUSNUM_MAX,  // AddressMaximum - Maximum Bus Number
        0,                           // AddressTranslation
        LX2160A_PCI_SEG_BUSNUM_RANGE // RangeLength - # of Busses
      )
      QWordMemory ( // 32-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0,                           // Granularity
        LX2160A_PCI_SEG0_MMIO32_MIN,   // Min Base Address
        LX2160A_PCI_SEG0_MMIO32_MAX,   // Max Base Address
        LX2160A_PCI_SEG0_MMIO32_XLATE, // Translate
        LX2160A_PCI_SEG0_MMIO32_SIZE   // Length
      )
      QWordMemory ( // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0,                           // Granularity
        LX2160A_PCI_SEG0_MMIO64_MIN,   // Min Base Address
        LX2160A_PCI_SEG0_MMIO64_MAX,   // Max Base Address
        0x0,                           // Translate
        LX2160A_PCI_SEG0_MMIO64_SIZE   // Length
      )
      QWordIO ( // IO window
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        EntireRange,
        0x0,                            // Granularity
        LX2160A_PCI_SEG0_IO64_MIN,      // Min Base Address
        LX2160A_PCI_SEG0_IO64_MAX,      // Max Base Address
        LX2160A_PCI_SEG0_IO64_XLATE,    // Translate
        LX2160A_PCI_SEG0_IO64_SIZE      // Length
      )
    })

    Method (_CRS, 0, Serialized) { // Root complex resources
      Return (RBUF)
    }

    Device (RES0)
    {
      Name (_HID, "NXP0016") // NXP PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        Memory32Fixed (ReadWrite, LX2160A_PCI_SEG0_RC_CONFIG_BASE,
                       LX2160A_PCI_SEG0_RC_CONFIG_SIZE)
      })
    }

    // PCI Routing Table
    Name (_PRT, Package () {
            Package () { 0xFFFF, 0, Zero, LX2160A_PCI_SEG0_INTA },   // INTA
            Package () { 0xFFFF, 1, Zero, LX2160A_PCI_SEG0_INTB },   // INTB
            Package () { 0xFFFF, 2, Zero, LX2160A_PCI_SEG0_INTC },   // INTC
            Package () { 0xFFFF, 3, Zero, LX2160A_PCI_SEG0_INTD },   // INTD
            })
    //
    // OS Control Handoff
    //
    Name (SUPP, Zero) // PCI _OSC Support Field value
    Name (CTRL, Zero) // PCI _OSC Control Field value

    Method(_OSC,4)
    {
        // Check for proper UUID
        If (LEqual (Arg0, ToUUID ("33DB4D5B-1FF7-401C-9657-7441C03DD766")))
        {
            // Create DWord-adressable fields from the Capabilities Buffer
            CreateDWordField (Arg3, 0, CDW1)
            CreateDWordField (Arg3, 4, CDW2)
            CreateDWordField (Arg3, 8, CDW3)

            // Save Capabilities DWord2 & 3
            Store (CDW2, SUPP)
            Store (CDW3, CTRL)

            // Disable PCIe and SHPC hotplug, AER and PME
            // And (CTRL, 0x10, CTRL)

            // Capabilities bits were masked
            Or (CDW1, 0x10, CDW1)

            // Update DWORD3 in the buffer
            Store (CTRL, CDW3)
            Return (Arg3)
       } Else {
            Or (CDW1, 4, CDW1) // Unrecognized UUID
                Return (Arg3)
       }
    } // End _OSC

    //
    // _DSM Device Specific Method
    //
    // Arg0: UUID Unique function identifier
    // Arg1: Integer Revision Level
    // Arg2: Integer Function Index (0 = Return Supported Functions)
    // Arg3: Package Parameters
    //
    Method (_DSM, 4, Serialized) {
      If (LEqual(Arg0, ToUUID("E5C937D0-3553-4D7A-9117-EA4D19C3434D"))) {
        switch (ToInteger(Arg2)) {
          //
          // Function 0: Return supported functions
          //
          case(0) {
            Return (Buffer() {0xFF})
          }

          //
          // Function 1: Return PCIe Slot Information
          //
          case(1) {
            Return (Package(2) {
              One, // Success
              Package(3) {
                0x2,  // x4 PCIe link
                0x1,  // PCI express card slot
                0x1   // WAKE# signal supported
              }
            })
          }

          //
          // Function 2: Return PCIe Slot Number.
          //
          case(2) {
            Return (Package(1) {
              Package(4) {
                0,  // Source ID (_SUN method)
                4,  // Token ID: ID refers to a slot
                0,  // Start bit of the field to use.
                7   // End bit of the field to use.
              }
            })
          }

          //
          // Function 3: Return Vendor-specific Token ID Strings.
          //
          case(3) {
            Return (Package(0) {})
          }

          //
          // Function 4: Return PCI Bus Capabilities
          //
          case(4) {
            Return (Package(2) {
              One, // Success
              Buffer() {
                1,0,            // Version
                0,0,            // Status, 0:Success
                24,0,0,0,       // Length
                1,0,            // PCI
                16,0,           // Length
                0,              // Attributes
                0x0D,           // Current Speed/Mode
                0x3F,0,         // Supported Speeds/Modes
                0,              // Voltage
                0,0,0,0,0,0,0   // Reserved
              }
            })
          }

          //
          // Function 5: Return Do Not Ignore PCI Boot Configuration
          //
          case(5) {
            Return (Package(1) {0})
          }

          //
          // Function 6: Return LTR Maximum Latency
          //
          case(6) {
            Return (Package(4) {
              Package(1){0},  // Maximum Snoop Latency Scale
              Package(1){0},  // Maximum Snoop Latency Value
              Package(1){0},  // Maximum No-Snoop Latency Scale
              Package(1){0}   // Maximum No-Snoop Latency Value
            })
          }

          //
          // Function 7: Return PCI Express Naming
          //
          case(7) {
            Return (Package(2) {
              Package(1) {0},
              Package(1) {Unicode("PCI0")}
            })
          }

          //
          // Not supported
          //
          default {
          }
        }
      }
      Return (Buffer(){0})
      } // End _DSM
  } // Device(PCI0)

  Device (RES0)
  {
    Name (_HID, "NXP0016") // NXP PCIe RC config base address
    Name (_CID, "PNP0C02") // Motherboard reserved resource
    Name (_UID, LX2160A_PCI_SEG0)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, LX2160A_PCI_SEG0_RC_CONFIG_BASE,
              LX2160A_PCI_SEG0_RC_CONFIG_SIZE)
    })
  }

  Device (PCI1)
  {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name (_UID, 0x1)
    Name(_SEG, LX2160A_PCI_SEG1) // Segment of this Root complex
    Name(_BBN, 0) // Base Bus Number
    Name(_CCA, 1) // Cache Coherency Attribute
    Name (RBUF, ResourceTemplate () {
      WordBusNumber ( // Bus numbers assigned to this root
        ResourceProducer, MinFixed, MaxFixed, PosDecode,
        0,                           // AddressGranularity
        LX2160A_PCI_SEG_BUSNUM_MIN,  // AddressMinimum - Minimum Bus Number
        LX2160A_PCI_SEG_BUSNUM_MAX,  // AddressMaximum - Maximum Bus Number
        0,                           // AddressTranslation
        LX2160A_PCI_SEG_BUSNUM_RANGE // RangeLength - # of Busses
      )
      QWordMemory ( // 32-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0,                           // Granularity
        LX2160A_PCI_SEG1_MMIO32_MIN,   // Min Base Address
        LX2160A_PCI_SEG1_MMIO32_MAX,   // Max Base Address
        LX2160A_PCI_SEG1_MMIO32_XLATE, // Translate
        LX2160A_PCI_SEG1_MMIO32_SIZE   // Length
      )
      QWordMemory ( // 64-bit BAR Windows
        ResourceProducer,
        PosDecode,
        MinFixed,
        MaxFixed,
        NonCacheable,
        ReadWrite,
        0x0,                           // Granularity
        LX2160A_PCI_SEG1_MMIO64_MIN,   // Min Base Address
        LX2160A_PCI_SEG1_MMIO64_MAX,   // Max Base Address
        0x0,                           // Translate
        LX2160A_PCI_SEG1_MMIO64_SIZE   // Length
      )
      QWordIO ( // IO window
        ResourceProducer,
        MinFixed,
        MaxFixed,
        PosDecode,
        EntireRange,
        0x0,                            // Granularity
        LX2160A_PCI_SEG1_IO64_MIN,      // Min Base Address
        LX2160A_PCI_SEG1_IO64_MAX,      // Max Base Address
        LX2160A_PCI_SEG1_IO64_XLATE,    // Translate
        LX2160A_PCI_SEG1_IO64_SIZE      // Length
      )
    })

    Method (_CRS, 0, Serialized) { // Root complex resources
      Return (RBUF)
    }

    Device (RES1)
    {
      Name (_HID, "NXP0016") // NXP PCIe RC config base address
      Name (_CRS, ResourceTemplate (){
        Memory32Fixed (ReadWrite, LX2160A_PCI_SEG1_RC_CONFIG_BASE,
                       LX2160A_PCI_SEG1_RC_CONFIG_SIZE)
      })
    }

    // PCI Routing Table
    Name (_PRT, Package () {
            Package () { 0xFFFF, 0, Zero, LX2160A_PCI_SEG1_INTA },   // INTA
            Package () { 0xFFFF, 1, Zero, LX2160A_PCI_SEG1_INTB },   // INTB
            Package () { 0xFFFF, 2, Zero, LX2160A_PCI_SEG1_INTC },   // INTC
            Package () { 0xFFFF, 3, Zero, LX2160A_PCI_SEG1_INTD },   // INTD
            })
    //
    // OS Control Handoff
    //
    Name (SUPP, Zero) // PCI _OSC Support Field value
    Name (CTRL, Zero) // PCI _OSC Control Field value

    Method(_OSC,4)
    {
        // Check for proper UUID
        If (LEqual (Arg0, ToUUID ("33DB4D5B-1FF7-401C-9657-7441C03DD766")))
        {
            // Create DWord-adressable fields from the Capabilities Buffer
            CreateDWordField (Arg3, 0, CDW1)
            CreateDWordField (Arg3, 4, CDW2)
            CreateDWordField (Arg3, 8, CDW3)

            // Save Capabilities DWord2 & 3
            Store (CDW2, SUPP)
            Store (CDW3, CTRL)

            // Disable PCIe and SHPC hotplug, AER and PME
            // And (CTRL, 0x10, CTRL)

            // Capabilities bits were masked
            Or (CDW1, 0x10, CDW1)

            // Update DWORD3 in the buffer
            Store (CTRL, CDW3)
            Return (Arg3)
       } Else {
            Or (CDW1, 4, CDW1) // Unrecognized UUID
                Return (Arg3)
       }
    } // End _OSC

    //
    // _DSM Device Specific Method
    //
    // Arg0: UUID Unique function identifier
    // Arg1: Integer Revision Level
    // Arg2: Integer Function Index (0 = Return Supported Functions)
    // Arg3: Package Parameters
    //
    Method (_DSM, 4, Serialized) {
      If (LEqual(Arg0, ToUUID("E5C937D0-3553-4D7A-9117-EA4D19C3434D"))) {
        switch (ToInteger(Arg2)) {
          //
          // Function 0: Return supported functions
          //
          case(0) {
            Return (Buffer() {0xFF})
          }

          //
          // Function 1: Return PCIe Slot Information
          //
          case(1) {
            Return (Package(2) {
              One, // Success
              Package(3) {
                0x3,  // x8 PCIe link
                0x1,  // PCI express card slot
                0x1   // WAKE# signal supported
              }
            })
          }

          //
          // Function 2: Return PCIe Slot Number.
          //
          case(2) {
            Return (Package(1) {
              Package(4) {
                0,  // Source ID (_SUN method)
                4,  // Token ID: ID refers to a slot
                0,  // Start bit of the field to use.
                7   // End bit of the field to use.
              }
            })
          }

          //
          // Function 3: Return Vendor-specific Token ID Strings.
          //
          case(3) {
            Return (Package(0) {})
          }

          //
          // Function 4: Return PCI Bus Capabilities
          //
          case(4) {
            Return (Package(2) {
              One, // Success
              Buffer() {
                1,0,            // Version
                0,0,            // Status, 0:Success
                24,0,0,0,       // Length
                1,0,            // PCI
                16,0,           // Length
                0,              // Attributes
                0x0D,           // Current Speed/Mode
                0x3F,0,         // Supported Speeds/Modes
                0,              // Voltage
                0,0,0,0,0,0,0   // Reserved
              }
            })
          }

          //
          // Function 5: Return Do Not Ignore PCI Boot Configuration
          //
          case(5) {
            Return (Package(1) {0})
          }

          //
          // Function 6: Return LTR Maximum Latency
          //
          case(6) {
            Return (Package(4) {
              Package(1){0},  // Maximum Snoop Latency Scale
              Package(1){0},  // Maximum Snoop Latency Value
              Package(1){0},  // Maximum No-Snoop Latency Scale
              Package(1){0}   // Maximum No-Snoop Latency Value
            })
          }

          //
          // Function 7: Return PCI Express Naming
          //
          case(7) {
            Return (Package(2) {
              Package(1) {0},
              Package(1) {Unicode("PCI1")}
            })
          }

          //
          // Not supported
          //
          default {
          }
        }
      }
      Return (Buffer(){0})
    } // End _DSM
  } // Device(PCI1)

  Device (RES1)
  {
    Name (_HID, "NXP0016") // NXP PCIe RC config base address
    Name (_CID, "PNP0C02") // Motherboard reserved resource
    Name (_UID, LX2160A_PCI_SEG1)  //  Unique ID
    Name (_CRS, ResourceTemplate (){
      Memory32Fixed (ReadWrite, LX2160A_PCI_SEG1_RC_CONFIG_BASE,
              LX2160A_PCI_SEG1_RC_CONFIG_SIZE)
    })
  }

  Device (RESP)  //reserve for ecam resource
  {
     Name (_HID, "PNP0C02")
     Name (_CRS, ResourceTemplate (){
       QwordMemory ( //ECAM space for [bus 00-ff]
         ResourceConsumer,
         PosDecode,
         MinFixed,
         MaxFixed,
         NonCacheable,
         ReadWrite,
         0x0,                               // Granularity
         LX2160A_PCI_SEG0_CONFIG_BASE,      // Min Base Address
         LX2160A_PCI_SEG0_CONFIG_BASE_MAX,  // Max Base Address
         0x0,                               // Translate
         LX2160A_PCI_SEG0_CONFIG_SIZE       // Length
       )
       QwordMemory ( //ECAM space for [bus 00-ff]
         ResourceConsumer,
         PosDecode,
         MinFixed,
         MaxFixed,
         NonCacheable,
         ReadWrite,
         0x0,                               // Granularity
         LX2160A_PCI_SEG1_CONFIG_BASE,      // Min Base Address
         LX2160A_PCI_SEG1_CONFIG_BASE_MAX,  // Max Base Address
         0x0,                               // Translate
         LX2160A_PCI_SEG1_CONFIG_SIZE       // Length
       )
     })
  }
}

