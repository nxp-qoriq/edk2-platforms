/** @file
*  (DSDT): PCI Express Controllers ACPI information
*
*  Copyright 2019-2020 NXP
*  Copyright 2020 Puresoftware Ltd
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
*/

Scope(_SB)
{
  Device (PCI1) {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 1) // Segment of this Root complex
    Name(_BBN, 0) // Base Bus Number
    Name(_CCA, 1) // Cache Coherency Attribute
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0,                           // AddressGranularity
          LS1046A_PCI_SEG_BUSNUM_MIN,  // AddressMinimum - Minimum Bus Number
          LS1046A_PCI_SEG_BUSNUM_MAX,  // AddressMaximum - Maximum Bus Number
          0,                           // AddressTranslation
          LS1046A_PCI_SEG_BUSNUM_RANGE // RangeLength - # of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,                           // Granularity
          LS1046A_PCI_SEG1_MMIO64_MIN,   // Min Base Address
          LS1046A_PCI_SEG1_MMIO64_MAX,   // Max Base Address
          LS1046A_PCI_SEG1_MMIO64_XLATE, // Translate
          LS1046A_PCI_SEG1_MMIO64_SIZE   // Length
        )
      })
      Return (RBUF)
    }

    Device (RES1) {
      Name (_HID, "NXP0016") // NXP PCIe RC config base address
      Name (_CRS, ResourceTemplate () {
        Memory32Fixed (ReadWrite, LS1046A_PCI_SEG1_RC_CONFIG_BASE,
                       LS1046A_PCI_SEG1_RC_CONFIG_SIZE)
      })
    }

    // PCI Routing Table
    Name (_PRT, Package () {
            Package () { 0xFFFF, 0, Zero, LS1046A_PCI_SEG1_INTA },   // INTA
            Package () { 0xFFFF, 1, Zero, LS1046A_PCI_SEG1_INTA },   // INTA
            Package () { 0xFFFF, 2, Zero, LS1046A_PCI_SEG1_INTA },   // INTA
            Package () { 0xFFFF, 3, Zero, LS1046A_PCI_SEG1_INTA },   // INTA
            })
    //
    // OS Control Handoff
    //
    Name (SUPP, Zero) // PCI _OSC Support Field value
    Name (CTRL, Zero) // PCI _OSC Control Field value

    Method(_OSC,4) {
        // Check for proper UUID
        If (LEqual (Arg0, ToUUID ("33DB4D5B-1FF7-401C-9657-7441C03DD766"))) {
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
  } // Device(PCI1)

  Device (RES1) {
    Name (_HID, "NXP0016") // NXP PCIe RC config base address
    Name (_CID, "PNP0C02") // Motherboard reserved resource
    Name (_UID, 0x1)  //  Unique ID
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, LS1046A_PCI_SEG1_RC_CONFIG_BASE,
              LS1046A_PCI_SEG1_RC_CONFIG_SIZE)
    })
  }

  Device (PCI2) {
    Name (_HID, "PNP0A08") // PCI Express Root Bridge
    Name (_CID, "PNP0A03") // Compatible PCI Root Bridge
    Name(_SEG, 2) // Segment of this Root complex
    Name(_BBN, 0) // Base Bus Number
    Name(_CCA, 1) // Cache Coherency Attribute
    Method (_CRS, 0, Serialized) { // Root complex resources
      Name (RBUF, ResourceTemplate () {
        WordBusNumber ( // Bus numbers assigned to this root
          ResourceProducer, MinFixed, MaxFixed, PosDecode,
          0,                           // AddressGranularity
          LS1046A_PCI_SEG_BUSNUM_MIN,  // AddressMinimum - Minimum Bus Number
          LS1046A_PCI_SEG_BUSNUM_MAX,  // AddressMaximum - Maximum Bus Number
          0,                           // AddressTranslation
          LS1046A_PCI_SEG_BUSNUM_RANGE // RangeLength - # of Busses
        )
        QWordMemory ( // 64-bit BAR Windows
          ResourceProducer,
          PosDecode,
          MinFixed,
          MaxFixed,
          NonCacheable,
          ReadWrite,
          0x0,                           // Granularity
          LS1046A_PCI_SEG2_MMIO64_MIN,   // Min Base Address
          LS1046A_PCI_SEG2_MMIO64_MAX,   // Max Base Address
          LS1046A_PCI_SEG2_MMIO64_XLATE, // Translate
          LS1046A_PCI_SEG2_MMIO64_SIZE   // Length
        )
      })
      Return (RBUF)
    }

    Device (RES2) {
      Name (_HID, "NXP0016") // NXP PCIe RC config base address
      Name (_CRS, ResourceTemplate () {
        Memory32Fixed (ReadWrite, LS1046A_PCI_SEG2_RC_CONFIG_BASE,
                       LS1046A_PCI_SEG2_RC_CONFIG_SIZE)
      })
    }

    // PCI Routing Table
    Name (_PRT, Package () {
            Package () { 0xFFFF, 0, Zero, LS1046A_PCI_SEG2_INTA },   // INTA
            Package () { 0xFFFF, 1, Zero, LS1046A_PCI_SEG2_INTA },   // INTA
            Package () { 0xFFFF, 2, Zero, LS1046A_PCI_SEG2_INTA },   // INTA
            Package () { 0xFFFF, 3, Zero, LS1046A_PCI_SEG2_INTA },   // INTA
            })
    //
    // OS Control Handoff
    //
    Name (SUPP, Zero) // PCI _OSC Support Field value
    Name (CTRL, Zero) // PCI _OSC Control Field value

    Method(_OSC,4) {
        // Check for proper UUID
        If (LEqual (Arg0, ToUUID ("33DB4D5B-1FF7-401C-9657-7441C03DD766"))) {
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
  } // Device(PCI1)

  Device (RES2) {
    Name (_HID, "NXP0016") // NXP PCIe RC config base address
    Name (_CID, "PNP0C02") // Motherboard reserved resource
    Name (_UID, 0x2)  //  Unique ID
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, LS1046A_PCI_SEG2_RC_CONFIG_BASE,
              LS1046A_PCI_SEG2_RC_CONFIG_SIZE)
    })
  }


  Device (RESP) {//reserve for ecam resource
     Name (_HID, "PNP0C02")
     Name (_CRS, ResourceTemplate () {
       QwordMemory ( //ECAM space for [bus 00-ff]
         ResourceConsumer,
         PosDecode,
         MinFixed,
         MaxFixed,
         NonCacheable,
         ReadWrite,
         0x0,                               // Granularity
         LS1046A_PCI_SEG0_CONFIG_BASE,      // Min Base Address
         LS1046A_PCI_SEG0_CONFIG_BASE_MAX,  // Max Base Address
         0x0,                               // Translate
         LS1046A_PCI_SEG0_CONFIG_SIZE       // Length
       )
       QwordMemory ( //ECAM space for [bus 00-ff]
         ResourceConsumer,
         PosDecode,
         MinFixed,
         MaxFixed,
         NonCacheable,
         ReadWrite,
         0x0,                               // Granularity
         LS1046A_PCI_SEG1_CONFIG_BASE,      // Min Base Address
         LS1046A_PCI_SEG1_CONFIG_BASE_MAX,  // Max Base Address
         0x0,                               // Translate
         LS1046A_PCI_SEG1_CONFIG_SIZE       // Length
       )
       QwordMemory ( //ECAM space for [bus 00-ff]
         ResourceConsumer,
         PosDecode,
         MinFixed,
         MaxFixed,
         NonCacheable,
         ReadWrite,
         0x0,                               // Granularity
         LS1046A_PCI_SEG2_CONFIG_BASE,      // Min Base Address
         LS1046A_PCI_SEG2_CONFIG_BASE_MAX,  // Max Base Address
         0x0,                               // Translate
         LS1046A_PCI_SEG2_CONFIG_SIZE       // Length
       )
     })
  }
}
