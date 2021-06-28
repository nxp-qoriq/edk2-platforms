/** @file
  PCI Host Bridge Library instance for NXP SoCs

  Copyright 2018-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiDxe.h>
#include <libfdt.h>
#include <IndustryStandard/Pci22.h>
#include <Library/IoAccessLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/IortLib.h>
#include <Library/ItbParse.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Pcie.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciIo.h>

#include "PciHostBridgeLib.h"

#pragma pack(1)
typedef struct {
  ACPI_HID_DEVICE_PATH     AcpiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_PCI_ROOT_BRIDGE_DEVICE_PATH;
#pragma pack ()

//
// Protocol notify related globals
//
VOID          *PlatformHasPciIoNotifyReg;
EFI_EVENT     PlatformHasPciIoEvent;

STATIC CONST EFI_PCI_ROOT_BRIDGE_DEVICE_PATH mEfiPciRootBridgeDevicePath[] = {
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      PCI_SEG0_NUM
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  },
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      PCI_SEG1_NUM
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  },
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      PCI_SEG2_NUM
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  },
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      PCI_SEG3_NUM
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  },
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      PCI_SEG4_NUM
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  },
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_DP,
        {
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH)),
          (UINT8)(sizeof (ACPI_HID_DEVICE_PATH) >> 8)
        }
      },
      EISA_PNP_ID (0x0A08), // PCI Express
      PCI_SEG5_NUM
    },

    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        END_DEVICE_PATH_LENGTH,
        0
      }
    }
  }
};

STATIC
GLOBAL_REMOVE_IF_UNREFERENCED
CHAR16 *mPciHostBridgeLibAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
};

#define PCI_ALLOCATION_ATTRIBUTES       EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM | \
                                        EFI_PCI_HOST_BRIDGE_MEM64_DECODE

#define PCI_SUPPORT_ATTRIBUTES          EFI_PCI_ATTRIBUTE_ISA_IO_16 | \
                                        EFI_PCI_ATTRIBUTE_ISA_MOTHERBOARD_IO | \
                                        EFI_PCI_ATTRIBUTE_VGA_MEMORY | \
                                        EFI_PCI_ATTRIBUTE_VGA_IO_16  | \
                                        EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16

PCI_ROOT_BRIDGE mPciRootBridges[NUM_PCIE_CONTROLLER];

/**
  Function to select page among the 48 1KB pages for
  AXI access

  @param  Dbi    GPEX host controller address.
  @param  PgIdx  The page index to select

**/
VOID
CcsrSetPg (
  IN EFI_PHYSICAL_ADDRESS Dbi,
  IN UINT8 PgIdx
  )
{
  UINT32 Val;
  Val = MmioRead32 (Dbi + PAB_CTRL);
  // Bit 18:13 of Bridge Control Register(PAB) denotes page select
  // Mask is 6 bits and shift is 13 to select page
  Val &= ~(PAB_CTRL_PAGE_SEL_MASK << PAB_CTRL_PAGE_SEL_SHIFT);
  Val |= (PgIdx & PAB_CTRL_PAGE_SEL_MASK) << PAB_CTRL_PAGE_SEL_SHIFT;

  MmioWrite32 (Dbi + PAB_CTRL, Val);
}

/**
  Function to read PCIe config address

  @param  Dbi     GPEX host controller address.
  @param  Offset  Offset to read from

**/
INTN
CcsrRead32 (
 IN EFI_PHYSICAL_ADDRESS Dbi,
 IN UINT32 Offset
  )
{

  // If Offset < 3KB direct addressing
  // is used
  if (Offset < INDIRECT_ADDR_BNDRY) {
    CcsrSetPg (Dbi, 0);
    return MmioRead32 (Dbi + Offset);
  } else {
    // If Offset > 3KB than paging mechanism is used
    // with indirect address mechanism
    // Select page index and offset within the page
    CcsrSetPg (Dbi, OFFSET_TO_PAGE_IDX (Offset));
    return MmioRead32 (Dbi + OFFSET_TO_PAGE_ADDR (Offset));
  }
}

/**
  Function to write PCIe Controller config address

  @param  Dbi     GPEX host controller address
  @param  Offset  Offset to read from

**/
VOID
CcsrWrite32 (
 IN EFI_PHYSICAL_ADDRESS Dbi,
 IN UINT32 Offset,
 IN UINT32 Value
  )
{

  if (Offset < INDIRECT_ADDR_BNDRY) {
    CcsrSetPg (Dbi, 0);
    MmioWrite32 (Dbi + Offset, Value);
  } else {
    CcsrSetPg (Dbi, OFFSET_TO_PAGE_IDX (Offset));
    MmioWrite32 (Dbi + OFFSET_TO_PAGE_ADDR (Offset), Value);
  }
}


/*
This function create the in-bound window for
RC for mem transfer for entire 1TB space

*/

STATIC
VOID
PciSetupInBoundWin (
  IN EFI_PHYSICAL_ADDRESS Pcie,
  IN UINT32 Idx,
  IN UINT32  Type,
  IN UINT64 Phys,
  IN UINT64 BusAddr,
  IN UINT64 Size)
{
  UINT32 Val;
  UINT64 WinSize;

  WinSize = ~(Size - 1);

  Val = CcsrRead32 ((UINTN)Pcie, PAB_PEX_AMAP_CTRL(Idx));

  Val &= ~(PEX_AMAP_CTRL_TYPE_MASK << PEX_AMAP_CTRL_TYPE_SHIFT);
  Val &= ~(PEX_AMAP_CTRL_EN_MASK << PEX_AMAP_CTRL_EN_SHIFT);
  Val = (Val | (Type << PEX_AMAP_CTRL_TYPE_SHIFT));
  Val = (Val | (1 << PEX_AMAP_CTRL_EN_SHIFT));

  CcsrWrite32 ((UINTN)Pcie, PAB_PEX_AMAP_CTRL(Idx),
               (Val | (UINT32)WinSize));

  CcsrWrite32 ((UINTN)Pcie, PAB_EXT_PEX_AMAP_SIZE(Idx), (WinSize>>32));
  CcsrWrite32 ((UINTN)Pcie, PAB_PEX_AMAP_AXI_WIN(Idx), (UINT32)Phys);
  CcsrWrite32 ((UINTN)Pcie, PAB_EXT_PEX_AMAP_AXI_WIN(Idx), (Phys>>32));
  CcsrWrite32 ((UINTN)Pcie, PAB_PEX_AMAP_PEX_WIN_L(Idx), (UINT32)BusAddr);
  CcsrWrite32 ((UINTN)Pcie, PAB_PEX_AMAP_PEX_WIN_H(Idx), (BusAddr >>32));

}

/**
  Function to set-up iATU outbound window for PCIe controller

  @param Dbi     Address of PCIe host controller.
  @param Idx     Index of iATU outbound window.
  @param Type    Type(Cfg0/Cfg1/Mem/IO) of iATU outbound window.
  @param Phys    PCIe controller phy address for outbound window.
  @param BusAdr  PCIe controller bus address for outbound window.
  @param Pcie    Size of PCIe controller space(Cfg0/Cfg1/Mem/IO).

**/
STATIC
VOID
PcieOutboundSet (
  IN EFI_PHYSICAL_ADDRESS Dbi,
  IN UINT32 Idx,
  IN UINT32 Type,
  IN UINT64 Phys,
  IN UINT64 BusAddr,
  IN UINT64 Size
  )
{
  if (PCI_LS_GEN4_CTRL) {
    UINT32 Val;
    Size = ~(Size -1 );

    // Mapping AXI transactions to PEX address
    Val = CcsrRead32 ((UINTN)Dbi, PAB_AXI_AMAP_CTRL (Idx));
    Val &= ~((AXI_AMAP_CTRL_TYPE_MASK << AXI_AMAP_CTRL_TYPE_SHIFT) |
           (AXI_AMAP_CTRL_SIZE_MASK << AXI_AMAP_CTRL_SIZE_SHIFT) |
            AXI_AMAP_CTRL_EN);
    // Type indicates the type of AXI transaction to the window address is mapped
    // to CFG/IO/MEM
    Val |= ((Type & AXI_AMAP_CTRL_TYPE_MASK) << AXI_AMAP_CTRL_TYPE_SHIFT) |
         (((UINT32)Size >> AXI_AMAP_CTRL_SIZE_SHIFT) <<
           AXI_AMAP_CTRL_SIZE_SHIFT) | AXI_AMAP_CTRL_EN;
    // Program Address mapping enable, other fields with desired values in Bridge
    // control Register
    // Contols the mapping of address for AXI transactions to PEX address
    CcsrWrite32 ((UINTN)Dbi, PAB_AXI_AMAP_CTRL (Idx), Val);

    // Program AXI window base with appropriate PCIe physical address space
    // values in Bridge Address mapping window Register
    CcsrWrite32 ((UINTN)Dbi, PAB_AXI_AMAP_AXI_WIN (Idx), (UINT32)Phys);
    CcsrWrite32 ((UINTN)Dbi, PAB_EXT_AXI_AMAP_AXI_WIN (Idx), Phys >> 32);
    // Program PEX Address base with appropriate Bus Address values in
    // Bridge address mapping window
    CcsrWrite32 ((UINTN)Dbi, PAB_AXI_AMAP_PEX_WIN_L (Idx), (UINT32)BusAddr);
    CcsrWrite32 ((UINTN)Dbi, PAB_AXI_AMAP_PEX_WIN_H (Idx), BusAddr >> 32);
    // Program the size of window
    CcsrWrite32 ((UINTN)Dbi, PAB_EXT_AXI_AMAP_SIZE (Idx), Size >> 32);

  } else {
    MmioWrite32 (Dbi + IATU_VIEWPORT_OFF,
                (UINT32)(IATU_VIEWPORT_OUTBOUND | Idx));
    MmioWrite32 (Dbi + IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0,
                (UINT32)Phys);
    MmioWrite32 (Dbi + IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_0,
                (UINT32)(Phys >> 32));
    MmioWrite32 (Dbi + IATU_LIMIT_ADDR_OFF_OUTBOUND_0,
                (UINT32)(Phys + Size - BIT0));
    MmioWrite32 (Dbi + IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0,
                (UINT32)BusAddr);
    MmioWrite32 (Dbi + IATU_UPPER_TARGET_ADDR_OFF_OUTBOUND_0,
                (UINT32)(BusAddr >> 32));
    MmioWrite32 (Dbi + IATU_REGION_CTRL_1_OFF_OUTBOUND_0,
                (UINT32)Type);
    if (CFG_SHIFT_ENABLE &&
            ((Type == IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG0) ||
            (Type == IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG1))) {
             MmioWrite32 (Dbi + IATU_REGION_CTRL_2_OFF_OUTBOUND_0,
                (IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN |
                 IATU_ENABLE_CFG_SHIFT_FEATURE));
    } else {
        MmioWrite32 (Dbi + IATU_REGION_CTRL_2_OFF_OUTBOUND_0,
                IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN);
    }
 }
}

/**

   Function to check PCIe controller LTSSM state

   @param Pcie Address of PCIe host controller.

**/
STATIC
INTN
PcieLinkState (
  IN EFI_PHYSICAL_ADDRESS Pcie
  )
{
  UINT32 State;
  UINT32 LtssmMask;

  if (PCI_LS_GEN4_CTRL) {
      LtssmMask = 0x7f;
  } else {
      LtssmMask = 0x3f;
  }
  //
  // Reading PCIe controller LTSSM state
  //
  if (FeaturePcdGet (PcdPciLutBigEndian)) {
    State = SwapMmioRead32 ((UINTN)Pcie + PCI_LUT_BASE + PCI_LUT_DBG) &
            LtssmMask;
  } else {
   State = MmioRead32 ((UINTN)Pcie + PCI_LUT_BASE + PCI_LUT_DBG) &
           LtssmMask;
  }

  if (State < LTSSM_PCIE_L0) {
    DEBUG ((DEBUG_INFO," Pcie Link error. LTSSM=0x%2x\n", State));
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}

/**
   Helper function to check PCIe link state

   @param Pcie Address of PCIe host controller.

**/
STATIC
INTN
PcieLinkUp (
  IN EFI_PHYSICAL_ADDRESS Pcie
  )
{
  INTN State;
  UINT32 Cap;

  State = PcieLinkState (Pcie);
  if (State) {
    return State;
  }

  //
  // Try to download speed to gen1
  //
  Cap = MmioRead32 ((UINTN)Pcie + PCI_LINK_CAP);
  MmioWrite32 ((UINTN)Pcie + PCI_LINK_CAP, (UINT32)(Cap & (~PCI_LINK_SPEED_MASK)) | BIT0);
  State = PcieLinkState (Pcie);
  if (State) {
    return State;
  }

  MmioWrite32 ((UINTN)Pcie + PCI_LINK_CAP, Cap);

  return EFI_SUCCESS;
}

/**
   This function checks whether PCIe is enabled or not
   depending upon SoC serdes protocol map

   @param  PcieNum PCIe number.

   @return The     PCIe number enabled in map.
   @return FALSE   PCIe number is disabled in map.

**/
STATIC
BOOLEAN
IsPcieNumEnabled(
  IN UINTN PcieNum
  )
{
  UINT64 SerDes1ProtocolMap;

  SerDes1ProtocolMap = 0x0;

  //
  // Reading serdes map
  //
  GetSerdesProtocolMaps (&SerDes1ProtocolMap);

  //
  // Verify serdes line is configured in the map
  //
  if (PcieNum < NUM_PCIE_CONTROLLER) {
    return IsSerDesLaneProtocolConfigured (SerDes1ProtocolMap, (PcieNum + BIT0));
  } else {
    DEBUG ((DEBUG_ERROR, "Device not supported\n"));
  }

  return FALSE;
}

STATIC
VOID
PcieSetupWindow (
  IN EFI_PHYSICAL_ADDRESS Pcie,
  IN EFI_PHYSICAL_ADDRESS Cfg0Base,
  IN EFI_PHYSICAL_ADDRESS Cfg1Base,
  IN EFI_PHYSICAL_ADDRESS MemBase,
  IN EFI_PHYSICAL_ADDRESS Mem64Base,
  IN EFI_PHYSICAL_ADDRESS IoBase
  )
{
  // ATU : OUTBOUND WINDOW 1 : CFG0
  PcieOutboundSet (Pcie, IATU_REGION_INDEX0,
                         PAB_AXI_TYPE_CFG,
                         Cfg0Base,
                         SEG_CFG_BUS,
                         SEG_CFG_SIZE);

  // ATU : OUTBOUND WINDOW 2 : IO
  PcieOutboundSet (Pcie, IATU_REGION_INDEX1,
                         PAB_AXI_TYPE_IO,
                         IoBase,
                         SEG_IO_BUS,
                         SEG_IO_SIZE);

  // ATU : OUTBOUND WINDOW 3 : MEM
  PcieOutboundSet (Pcie, IATU_REGION_INDEX2,
                         PAB_AXI_TYPE_MEM,
                         MemBase,
                         SEG_MEM_BUS,
                         SEG_MEM_SIZE);

  // ATU : OUTBOUND WINDOW 4 : MMIO64
  PcieOutboundSet (Pcie, IATU_REGION_INDEX3,
                            PAB_AXI_TYPE_MEM,
                            Mem64Base,
                            Mem64Base,
                            SIZE_4GB);
  Mem64Base += SIZE_4GB;

  // ATU : OUTBOUND WINDOW 5 : MMIO64
  PcieOutboundSet (Pcie, IATU_REGION_INDEX4,
                            PAB_AXI_TYPE_MEM,
                            Mem64Base,
                            Mem64Base,
                            SIZE_4GB);
  Mem64Base += SIZE_4GB;

  // ATU : OUTBOUND WINDOW 6 : MMIO64
  PcieOutboundSet (Pcie, IATU_REGION_INDEX5,
                            PAB_AXI_TYPE_MEM,
                            Mem64Base,
                            Mem64Base,
                            SIZE_4GB);
  Mem64Base += SIZE_4GB;

  // ATU : OUTBOUND WINDOW 7 : MMIO64
  PcieOutboundSet (Pcie, IATU_REGION_INDEX6,
                            PAB_AXI_TYPE_MEM,
                            Mem64Base,
                            Mem64Base,
                            SIZE_4GB);

  if (FeaturePcdGet (PcdPciDebug) == TRUE) {
    INTN Cnt;
    for (Cnt = 0; Cnt <= IATU_REGION_INDEX2; Cnt++) {
      DEBUG ((DEBUG_INFO,"APIO WINDOW%d:\n", Cnt));
      DEBUG ((DEBUG_INFO,"\tLOWER PHYS 0x%08x\n",
              CcsrRead32 ((UINTN)Pcie, PAB_AXI_AMAP_AXI_WIN (Cnt))));
      DEBUG ((DEBUG_INFO,"\tUPPER PHYS 0x%08x\n",
              CcsrRead32 ((UINTN)Pcie, PAB_EXT_AXI_AMAP_AXI_WIN (Cnt))));
      DEBUG ((DEBUG_INFO,"\tLOWER BUS  0x%08x\n",
              CcsrRead32 ((UINTN)Pcie, PAB_AXI_AMAP_PEX_WIN_L (Cnt))));
      DEBUG ((DEBUG_INFO,"\tUPPER BUS  0x%08x\n",
              CcsrRead32 ((UINTN)Pcie, PAB_AXI_AMAP_PEX_WIN_H (Cnt))));
      DEBUG ((DEBUG_INFO,"\tSIZE      0x%08x\n",
              CcsrRead32 ((UINTN)Pcie, PAB_AXI_AMAP_CTRL (Cnt)) & (AXI_AMAP_CTRL_SIZE_MASK << AXI_AMAP_CTRL_SIZE_SHIFT)));
      DEBUG ((DEBUG_INFO,"\tEXT_SIZE        0x%08x\n",
              CcsrRead32 ((UINTN)Pcie, PAB_EXT_AXI_AMAP_SIZE (Cnt))));
      DEBUG ((DEBUG_INFO,"\tCTRL:        0x%08x\n",
              CcsrRead32 ((UINTN)Pcie, PAB_AXI_AMAP_CTRL (Cnt))));
    }
  }
}

/**
  Function to set-up iATU outbound window for PCIe controller

  @param Pcie     Address of PCIe host controller
  @param Cfg0Base PCIe controller phy address Type0 Configuration Space.
  @param Cfg1Base PCIe controller phy address Type1 Configuration Space.
  @param MemBase  PCIe controller phy address Memory Space.
  @param Mem64Base PCIe controller phy address MMIO64 Space.
  @param IoBase   PCIe controller phy address IO Space.
**/
STATIC
VOID
PcieSetupAtu (
  IN EFI_PHYSICAL_ADDRESS Pcie,
  IN EFI_PHYSICAL_ADDRESS Cfg0Base,
  IN EFI_PHYSICAL_ADDRESS Cfg1Base,
  IN EFI_PHYSICAL_ADDRESS MemBase,
  IN EFI_PHYSICAL_ADDRESS Mem64Base,
  IN EFI_PHYSICAL_ADDRESS IoBase
  )
{
  UINT64 Cfg0BaseAddr;
  UINT64 Cfg1BaseAddr;
  UINT64 Cfg0BusAddress;
  UINT64 Cfg1BusAddress;
  UINT64 Cfg0Size;
  UINT64 Cfg1Size;

  if (CFG_SHIFT_ENABLE) {
    DEBUG ((DEBUG_INFO, "PCIe: CFG Shit Method Enabled \n"));
    Cfg0BaseAddr = Cfg0Base + SIZE_1MB;
    Cfg1BaseAddr = Cfg0Base + SIZE_2MB;
    Cfg0BusAddress = SIZE_1MB;
    Cfg1BusAddress = SIZE_2MB;
    if (PcdGetBool (PcdPciHideRootPort)) {
      Cfg0Size = SIZE_32KB;
    } else {
      Cfg0Size = SIZE_1MB;
    }
    Cfg1Size = (SIZE_256MB - SIZE_1MB); // 255MB

  } else {
      Cfg0BaseAddr = Cfg0Base;
      Cfg1BaseAddr = Cfg1Base;
      Cfg0BusAddress = SEG_CFG_BUS;
      Cfg1BusAddress = SEG_CFG_BUS;
      Cfg0Size = SEG_CFG_SIZE;
      Cfg1Size = SEG_CFG_SIZE;
  }
  //
  // iATU : OUTBOUND WINDOW 1 : CFG0
  //
  PcieOutboundSet (Pcie, IATU_REGION_INDEX0,
                            IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG0,
                            Cfg0BaseAddr,
                            Cfg0BusAddress,
                            Cfg0Size);

  //
  // iATU : OUTBOUND WINDOW 2 : CFG1
  PcieOutboundSet (Pcie, IATU_REGION_INDEX1,
                            IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG1,
                            Cfg1BaseAddr,
                            Cfg1BusAddress,
                            Cfg1Size);

  //
  // iATU : OUTBOUND WINDOW 3 : MEM
  //
  PcieOutboundSet (Pcie, IATU_REGION_INDEX2,
                            IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM,
                            MemBase,
                            SEG_MEM_BUS,
                            SEG_MEM_SIZE);

  //
  // iATU : OUTBOUND WINDOW 4 : MMIO64
  //
  PcieOutboundSet (Pcie, IATU_REGION_INDEX3,
                            IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM,
                            Mem64Base,
                            Mem64Base,
                            SIZE_4GB);
  Mem64Base += SIZE_4GB;

  //
  // iATU : OUTBOUND WINDOW 5 : MMIO64
  //
  PcieOutboundSet (Pcie, IATU_REGION_INDEX4,
                            IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM,
                            Mem64Base,
                            Mem64Base,
                            SIZE_4GB);
  Mem64Base += SIZE_4GB;

  //
  // iATU : OUTBOUND WINDOW 6 : MMIO64
  //
  PcieOutboundSet (Pcie, IATU_REGION_INDEX5,
                            IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM,
                            Mem64Base,
                            Mem64Base,
                            SIZE_4GB);
  Mem64Base += SIZE_4GB;

  //
  // iATU : OUTBOUND WINDOW 7 : MMIO64
  //
  PcieOutboundSet (Pcie, IATU_REGION_INDEX6,
                            IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM,
                            Mem64Base,
                            Mem64Base,
                            SIZE_4GB);

  //
  //
  // iATU : OUTBOUND WINDOW 8: IO
  //
  PcieOutboundSet (Pcie, IATU_REGION_INDEX7,
          IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_IO,
          IoBase,
          SEG_IO_BUS,
          SEG_IO_SIZE);
}

/**
  Helper function to set-up PCIe controller

  @param Pcie     Address of PCIe host controller
  @param Cfg0Base PCIe controller phy address Type0 Configuration Space.
  @param Cfg1Base PCIe controller phy address Type1 Configuration Space.
  @param MemBase  PCIe controller phy address Memory Space.
  @param IoBase   PCIe controller phy address IO Space.
  @param Mem64Base  PCIe controller phy address MMIO64 Space.

**/
STATIC
VOID
PcieSetupCntrl (
  IN EFI_PHYSICAL_ADDRESS Pcie,
  IN EFI_PHYSICAL_ADDRESS Cfg0Base,
  IN EFI_PHYSICAL_ADDRESS Cfg1Base,
  IN EFI_PHYSICAL_ADDRESS MemBase,
  IN EFI_PHYSICAL_ADDRESS Mem64Base,
  IN EFI_PHYSICAL_ADDRESS IoBase
  )
{
  UINT32 Val;

  if (PCI_LS_GEN4_CTRL) {

    // Set ACK Latency Timeout
    Val = CcsrRead32 ((UINTN)Pcie, GPEX_ACK_REPLAY_TO);
    Val &= ~(ACK_LAT_TO_VAL_MASK << ACK_LAT_TO_VAL_SHIFT);
    Val |= (4 << ACK_LAT_TO_VAL_SHIFT);
    CcsrWrite32 ((UINTN)Pcie, GPEX_ACK_REPLAY_TO, Val);

    //Fix Class Code
    Val = CcsrRead32 ((UINTN)Pcie, GPEX_CLASSCODE);
    Val &= ~(GPEX_CLASSCODE_MASK << GPEX_CLASSCODE_SHIFT);
    Val |= PCI_CLASS_BRIDGE_PCI << GPEX_CLASSCODE_SHIFT;
    CcsrWrite32 ((UINTN)Pcie, GPEX_CLASSCODE, Val);

    // Enable APIO and Memory/IO/CFG Wins
    Val = CcsrRead32 ((UINTN)Pcie, PAB_AXI_PIO_CTRL (0));
    // AXI PIO programming
    // AXI PIO is used to generate PIO traffic from AXI to PCIe Link
    // Enable AXI-PIO/MEM/IO/CFG
    Val |= APIO_EN | MEM_WIN_EN | IO_WIN_EN | CFG_WIN_EN;
    // Programm MEM/IO/CFG window in bridge control register
    CcsrWrite32 ((UINTN)Pcie, PAB_AXI_PIO_CTRL (0), Val);

    if (FeaturePcdGet (PcdPciDebug) == TRUE) {
      DEBUG ((DEBUG_INFO, "Going to SetUp PCIe Space Windows\n\n"));
    }

    PciSetupInBoundWin (Pcie, 0, PAB_AXI_TYPE_MEM, 0 , 0, SIZE_1TB);
    PcieSetupWindow (Pcie, Cfg0Base, Cfg1Base, MemBase, Mem64Base, IoBase);

    // Enable AMBA & PEX PIO
    // PEX PIO is used to generate PIO traffic from PCIe Link to AXI
    Val = CcsrRead32 ((UINTN)Pcie, PAB_CTRL);
    // Enable AXI-PIO/PEX PIO
    Val |= PAB_CTRL_APIO_EN | PAB_CTRL_PPIO_EN;
    // Programm APIO/PPIO in Bridge control register
    CcsrWrite32 ((UINTN)Pcie, PAB_CTRL, Val);

     Val = CcsrRead32((UINTN)Pcie, PAB_PEX_PIO_CTRL(0));
     Val |= PPIO_EN;
     CcsrWrite32((UINTN)Pcie, PAB_PEX_PIO_CTRL(0), Val);

  } else {
    //
    // iATU outbound set-up
    //
    PcieSetupAtu (Pcie, Cfg0Base, Cfg1Base, MemBase, Mem64Base, IoBase);

    //
    // program correct class for RC
    //
    MmioWrite32 ((UINTN)Pcie + PCI_BASE_ADDRESS_0, (BIT0 - BIT0));
    MmioWrite32 ((UINTN)Pcie + PCI_DBI_RO_WR_EN, (UINT32)BIT0);
    Val = MmioRead32 ((UINTN)Pcie + PCI_CLASS_DEVICE);
    Val &= ~(CLASS_CODE_MASK << CLASS_CODE_SHIFT);
    Val |= (PCI_CLASS_BRIDGE_PCI << CLASS_CODE_SHIFT);
    MmioWrite32 ((UINTN)Pcie + PCI_CLASS_DEVICE, Val);
    MmioWrite32 ((UINTN)Pcie + PCI_DBI_RO_WR_EN, (UINT32)(BIT0 - BIT0));
  }
}

/**
  Find the Pcie Node Offset in device tree, whose address matches with the given address

  @param[in] Dtb         Device tree to fixup
  @param[in] Address     addresses of registers of pcie controller which is to be matched

  @retval  Pcie Node Offset in device tree. if no node found then -FDT_ERR_NOTFOUND
**/
INT32
FdtFindPcie (
  IN  VOID    *Dtb,
  IN  UINTN   Address
  )
{
  INTN    NodeOffset;
  UINT64  PcieAddress;
  INT32   RegIndex;
  EFI_STATUS Status;

  /* find pci controller node */
  for (NodeOffset = fdt_node_offset_by_compatible (Dtb, -1, (VOID *)(PcdGetPtr (PcdPciFdtCompatible)));
       NodeOffset != -FDT_ERR_NOTFOUND;
       NodeOffset = fdt_node_offset_by_compatible (Dtb, NodeOffset, (VOID *)(PcdGetPtr (PcdPciFdtCompatible)))) {
    // Get the Index of Controllers' registers
    RegIndex = fdt_stringlist_search (Dtb, NodeOffset, "reg-names", "regs");
    if (RegIndex < 0) {
      RegIndex = 0;
    }
    // Get the controller's registers' address from node.
    Status = FdtGetAddressSize (Dtb, NodeOffset, "reg", RegIndex, &PcieAddress, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: can't get regs base address(Status = %r)!\n", Status));
      continue;
    }
    // Search the address in received array from PciHostBridgeLib
    if (Address == PcieAddress) {
      break;
    }
  }

  return NodeOffset;
}

/**
  Program a single LUT entry
 **/
STATIC
VOID
PcieLutSetMapping (
  IN   LS_PCIE   *LsPcie,
  IN   INT32     Index,
  IN   UINT32    BusDevFuc,
  IN   INT32     StreamId
  )
{
  /* leave mask as all zeroes, want to match all bits */
  if (FeaturePcdGet (PcdPciLutBigEndian)) {
    SwapMmioWrite32 ((UINTN)&LsPcie->LsPcieLut->PexLut[Index].PexLudr, BusDevFuc << 16);
    SwapMmioWrite32 ((UINTN)&LsPcie->LsPcieLut->PexLut[Index].PexLldr, StreamId | PCIE_LUT_ENABLE);
  } else {
    MmioWrite32 ((UINTN)&LsPcie->LsPcieLut->PexLut[Index].PexLudr, BusDevFuc << 16);
    MmioWrite32 ((UINTN)&LsPcie->LsPcieLut->PexLut[Index].PexLldr, StreamId | PCIE_LUT_ENABLE);
  }
}

/**
  Return next available LUT index.
 **/
STATIC
INT32
PcieNextLutIndex (
 IN  LS_PCIE   *LsPcie
 )
{
  if (LsPcie->NextLutIndex < ARRAY_SIZE (LsPcie->LsPcieLut->PexLut)) {
    return LsPcie->NextLutIndex++;
  } else {
    return -1;;  /* LUT is full */
  }
}

/**
  returns the available streamid for pcie, -1 if failed
 **/
STATIC
INT32
PcieGetStreamId (LS_PCIE  *LsPcie)
{
  STATIC INT32 NextStreamid = FixedPcdGet32 (PcdPcieStreamIdStart);
  INT32        StreamId;

  if (PCI_STREAMID_PER_CTRL) {
    StreamId = LsPcie->CurrentStreamId;
    if (StreamId > FixedPcdGet32 (PcdPcieStreamIdEnd)) {
      return -1;
    }

    LsPcie->CurrentStreamId++;

    return StreamId | ((LsPcie->ControllerIndex + 1) << 11);
  } else {
    if (NextStreamid > FixedPcdGet32 (PcdPcieStreamIdEnd)) {
      return -1;
    }

    return NextStreamid++;
  }
}

/**
  An msi-map is a property to be added to the pci controller
  node.  It is a table, where each entry consists of 4 fields
  e.g.:

       msi-map = <[devid] [phandle-to-msi-ctrl] [stream-id] [count]
                  [devid] [phandle-to-msi-ctrl] [stream-id] [count]>;
 **/
EFI_STATUS
FdtPcieSetMsiMapEntry (
  VOID      *Dtb,
  INT32     PcieNodeOffset,
  UINT32    BusDevFuc,
  INT32     StreamId
  )
{
  INT32          FdtStatus;
  CONST fdt32_t  *Property;
  UINT32         PHandle;

  /* get phandle to MSI controller */
  Property = fdt_getprop (Dtb, PcieNodeOffset, "msi-parent", 0);
  if (Property == NULL) {
    DEBUG ((DEBUG_WARN, "missing msi-parent for %a\n", fdt_get_name (Dtb, PcieNodeOffset, NULL)));
    return EFI_NOT_FOUND;
  }
  PHandle = fdt32_to_cpu (*Property);

  /* set one msi-map row */
  FdtStatus = fdt_appendprop_u32 (Dtb, PcieNodeOffset, "msi-map", BusDevFuc);
  if (FdtStatus) {
    goto Error;
  }
  FdtStatus = fdt_appendprop_u32 (Dtb, PcieNodeOffset, "msi-map", PHandle);
  if (FdtStatus) {
    goto Error;
  }
  FdtStatus = fdt_appendprop_u32 (Dtb, PcieNodeOffset, "msi-map", StreamId);
  if (FdtStatus) {
    goto Error;
  }
  FdtStatus = fdt_appendprop_u32 (Dtb, PcieNodeOffset, "msi-map", 1);
  if (FdtStatus) {
    goto Error;
  }

  return EFI_SUCCESS;

Error:
    DEBUG ((
      DEBUG_ERROR, "error %a setting msi-map for %a\n",
      fdt_strerror (FdtStatus), fdt_get_name (Dtb, PcieNodeOffset, NULL)
      ));
    return EFI_DEVICE_ERROR;
}

/**
  An iommu-map is a property to be added to the pci controller
  node.  It is a table, where each entry consists of 4 fields
  e.g.:

       iommu-map = <[devid] [phandle-to-iommu-ctrl] [stream-id] [count]
                  [devid] [phandle-to-iommu-ctrl] [stream-id] [count]>;
 **/
EFI_STATUS
FdtPcieSetIommuMapEntry (
  VOID    *Dtb,
  INT32   PcieNodeOffset,
  UINT32  BusDevFuc,
  INT32   StreamId
  )
{
  INT32          FdtStatus;
  CONST fdt32_t  *Property;
  UINT32         IommuMap[4];

  /* get phandle to iommu controller */
  Property = fdt_getprop (Dtb, PcieNodeOffset, "iommu-map", NULL);
  if (Property == NULL) {
    DEBUG ((DEBUG_WARN, "missing iommu-map for %a\n", fdt_get_name (Dtb, PcieNodeOffset, NULL)));
    return EFI_NOT_FOUND;
  }

  /* set iommu-map row */
  IommuMap[0] = cpu_to_fdt32 (BusDevFuc);
  IommuMap[1] = *++Property;
  IommuMap[2] = cpu_to_fdt32 (StreamId);
  IommuMap[3] = cpu_to_fdt32 (1);

  if (BusDevFuc == 0) {
    FdtStatus = fdt_setprop_inplace (Dtb, PcieNodeOffset, "iommu-map", IommuMap, 16);
  } else {
    FdtStatus = fdt_appendprop (Dtb, PcieNodeOffset, "iommu-map", IommuMap, 16);
  }

  if (FdtStatus) {
    DEBUG ((
      DEBUG_ERROR, "error %a setting iommu-map for %a\n",
      fdt_strerror (FdtStatus), fdt_get_name (Dtb, PcieNodeOffset, NULL)
      ));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  fix the pci controller node in device tree with msi-map and iommu-map property

  @param[in] Dtb           Device tree to fixup
  @param[in] LsPcie        Array of type LS_PCIE for all PCIE controllers in SOC
  @param[in] SegmentNumber SegmentNumber corresponding to PCIE controller. Used
                           to find PCIE controller structure in LsPcie array.
  @param[in] InputId       The device as identified by BusDeviceFunc Triplet
  @param[in] OutputId      StreamId assigned to the Pcie device.

  @retval EFI_SUCCESS       device tree fixed up successfully
  @retval EFI_NOT_FOUND     Controller node not found in device tree
  @retval EFI_DEVICE_ERROR  Couldn't fix the device tree
**/
STATIC
EFI_STATUS
FdtPcieSetUp (
  VOID      *Dtb,
  LS_PCIE   *LsPcie,
  UINTN     SegmentNumber,
  UINT32    InputId,
  UINT32    OutputId
  )
{
  INTN                  PcieNodeOffset;
  EFI_STATUS            Status;

  // Segment Number denotes the controller number
  // Find the controller node offset in Device tree based on this
  PcieNodeOffset = FdtFindPcie (Dtb, LsPcie[SegmentNumber].ControllerAddress);
  if (PcieNodeOffset < 0) {
    DEBUG ((
      DEBUG_WARN,
      "Pcie node with regs address %p not found in Dtb\n",
      LsPcie[SegmentNumber].ControllerAddress
      ));
    return EFI_NOT_FOUND;
  }

  Status = FdtPcieSetIommuMapEntry (Dtb, PcieNodeOffset, InputId, OutputId);
  if (EFI_ERROR (Status) && (Status != EFI_NOT_FOUND)) {
    return Status;
  }

  Status = FdtPcieSetMsiMapEntry (Dtb, PcieNodeOffset, InputId, OutputId);
  if (EFI_ERROR (Status) && (Status != EFI_NOT_FOUND)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  ID mappings represent the formula by which an ID from a source is converted to
  an ID in a destination.

  This function doesn't check the table for memory footprint. i.e. it's assumed
  that the memory for ID mapping is avaliable in IORT table.

  @param[in] CurrentTable  IORT table to fixup
  @param[in] InputId       Input Id to ITS block
  @param[in] OutputId      Output Id from ITS block

  @retval EFI_SUCCESS      IORT table fixed up successfully
**/
EFI_STATUS
IortPcieSetItsIdMapping (
  VOID    *CurrentTable,
  UINT32  InputId,
  UINT32  OutputId
  )
{
  (void)SetItsIdMapping(CurrentTable, InputId, OutputId);

  return EFI_SUCCESS;
}

/**
  ID mappings represent the formula by which an ID from a source is converted to
  an ID in a destination.

  This function doesn't check the table for memory footprint. i.e. it's assumed
  that the memory for ID mapping is avaliable in IORT table.

  @param[in] CurrentTable  IORT table to fixup
  @param[in] SegmentNumber SegmentNumber corresponding to PCIE controller. Used
                           to find PCIE controller structure in Iort->PciRcNode
                           array.
  @param[in] InputId       The device as identified by BusDeviceFunc Triplet
  @param[in] OutputId      StreamId assigned to the Pcie device.

  @retval EFI_SUCCESS      IORT table fixed up successfully
  @retval EFI_NOT_FOUND    Controller node not found in IORT table
**/
EFI_STATUS
IortPcieSetIommuIdMapping (
  VOID    *CurrentTable,
  UINTN   SegmentNumber,
  UINT32  InputId,
  UINT32  OutputId
  )
{
  if (SetIommuIdMapping(CurrentTable, SegmentNumber, InputId, OutputId)) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  ID mappings represent the formula by which an ID from a source is converted to
  an ID in a destination. For example, for a root complex behind an SMMU, the
  RID originating from that root complex must be converted to a StreamID in the
  destination SMMU. With IORT, ID mappings are declared in the source node.

  This function doesn't check the table for memory footprint. i.e. it's assumed
  that the memory for ID mapping is avaliable in IORT table.

  @param[in] CurrentTable  IORT table to fixup
  @param[in] SegmentNumber SegmentNumber corresponding to PCIE controller. Used
                           to find PCIE controller structure in Iort->PciRcNode
                           array.
  @param[in] InputId       Input Id to IOMMU block
  @param[in] OutputId      Output Id from IOMMU block

  @retval EFI_SUCCESS      IORT table fixed up successfully
  @retval EFI_NOT_FOUND    Controller node not found in IORT table
**/
EFI_STATUS
IortPcieSetUp (
  VOID      *CurrentTable,
  UINTN     SegmentNumber,
  UINT32    InputId,
  UINT32    OutputId
  )
{
  EFI_STATUS            Status;

  Status = IortPcieSetIommuIdMapping (CurrentTable, SegmentNumber, InputId, OutputId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PcdGetBool (PcdNoITS) == 0) {
    Status = IortPcieSetItsIdMapping (CurrentTable, OutputId, OutputId);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return EFI_SUCCESS;
}

/**
  This notification function is invoked when an instance of the
  EFI_PCI_IO_PROTOCOL is produced.  It searches the devices on the IO
  Protocol and fixes the device tree with msi-map and iommu-map property
  corresponding to that device.

  @param  Event                 The event that occured
  @param  Context               The address of PCIE Registers

**/
VOID
EFIAPI
OnPlatformHasPciIo (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
{
  EFI_HANDLE            Handle;
  EFI_STATUS            Status;
  UINTN                 BufferSize;
  EFI_PCI_IO_PROTOCOL   *PciIo;
  UINTN                 SegmentNumber;
  UINTN                 BusNumber;
  UINTN                 DeviceNumber;
  UINTN                 FunctionNumber;
  UINTN                 BusDevFuc;
  VOID                  *Dtb;
  INT32                 StreamId;
  INT32                 LutIndex;
  LS_PCIE               *LsPcie;
  VOID                  *Dev;

  LsPcie = (LS_PCIE *)Context;

  Status = EfiGetSystemConfigurationTable (&gFdtTableGuid, &Dtb);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Did not find the Dtb Blob.\n"));
    return;
  }

  //
  // Examine all new handles
  //
  for (;;) {
    //
    // Get the next handle
    //
    BufferSize = sizeof (Handle);
    Status = gBS->LocateHandle (
                    ByRegisterNotify,
                    NULL,
                    PlatformHasPciIoNotifyReg,
                    &BufferSize,
                    &Handle
                    );

    //
    // If not found, we're done
    //
    if (EFI_NOT_FOUND == Status) {
      break;
    }

    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Get the PciIo protocol on that handle
    //
    Status = gBS->HandleProtocol (Handle, &gEfiPciIoProtocolGuid, (VOID **)&PciIo);
    ASSERT_EFI_ERROR (Status);
    ASSERT (PciIo != NULL);

    // Check if this PciIo is installed for a pcie controller root bridge
    // or a Non discoverable Pci Io Instance
    Status = gBS->HandleProtocol (Handle, &gEdkiiNonDiscoverableDeviceProtocolGuid, (VOID **)&Dev);
    if (Status == EFI_SUCCESS) {
      // Io protocol is for Non discoverable Pci device. no fixups for this
      continue;
    }

    //
    // Get the Bus Device and Function
    //
    Status = PciIo->GetLocation (
                      PciIo,
                      &SegmentNumber,
                      &BusNumber,
                      &DeviceNumber,
                      &FunctionNumber
                      );

    // Combine Bus, Device and Function number
    // Bus       PCI Bus number. Range 0..255.
    // Device    PCI Device number. Range 0..31.
    // Function  PCI Function number. Range 0..7.
    BusDevFuc = ((BusNumber & 0xff) << 8) | ((DeviceNumber & 0x1f) << 3) | (FunctionNumber & 0x07);
    if (BusDevFuc == 0) {
      // For RC, already fixup has been applied.
      continue;
    }

    LutIndex = PcieNextLutIndex (&LsPcie[SegmentNumber]);
    if (LutIndex < 0) {
      DEBUG ((DEBUG_WARN, "No free Lut Index for Pcie\n"));
      continue;
    }

    // Get a free StreamId from pool, if not found we are done
    StreamId = PcieGetStreamId (&LsPcie[SegmentNumber]);
    if (StreamId < 0) {
      DEBUG ((DEBUG_WARN, "No free StreamId for Pcie\n"));
      break;
    }

    Status = FdtPcieSetUp (Dtb, LsPcie, SegmentNumber, BusDevFuc, StreamId);
    if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
      break;
    }

    if (PcdGetBool (PcdDynamicIortTable)) {
      DEBUG ((DEBUG_INFO, "Dynamic Iort Table is Enabled.\n"));
      PcdSet64 (PcdIortTablePtr, PcdGet64 (PcdDynamicIortTablePtr));
    }

    if (PcdGet64 (PcdIortTablePtr) != 0) {
      Status = IortPcieSetUp ((VOID *)PcdGet64 (PcdIortTablePtr), SegmentNumber,
                              BusDevFuc, StreamId);
      if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
        break;
      }
    }

    PcieLutSetMapping (&LsPcie[SegmentNumber], LutIndex, BusDevFuc, StreamId);
  }

  return;
}


/**
  Fixup the pcie controller status disabled or okay in device tree

  @param[in] Dtb      Device tree to fixup
  @param[in] LsPcie   pointer to a particular LS_PCIE node in array of all
                      controllers
  @param[in] Enabled  Weather the PCIE controller is enabled or not (by serdes
                      protocol)? based on this the status property in controller
                      node is set as "disabled" or "okay".

  @retval EFI_SUCCESS       Controller status set successfully
  @retval EFI_DEVICE_ERROR  Couldn't set the status
**/

EFI_STATUS
FdtFixupPcieStatus (
  IN  VOID      *Dtb,
  IN  LS_PCIE   *LsPcie,
  IN  BOOLEAN   Enabled
  )
{
  INTN        PcieNodeOffset;
  INT32       FdtStatus;
  INT32       StreamId;
  INT32       LutIndex;
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  PcieNodeOffset = FdtFindPcie (Dtb, LsPcie->ControllerAddress);
  if (PcieNodeOffset < 0) {
    DEBUG ((
      DEBUG_WARN,
      "Pcie node with regs address %p not found in Dtb\n",
      LsPcie->ControllerAddress
      ));
    return EFI_DEVICE_ERROR;
  }

  if (Enabled == TRUE) {
    FdtStatus = fdt_setprop_string (Dtb, PcieNodeOffset, "status", "okay");
    // set the MSI interrupt map and IO MMU map for BusDevFunc = 0
    // In the current fixup, RC is also assigned a steam ID
    // because in theory we can access the RC itself config space on PCIe link
    LutIndex = PcieNextLutIndex (LsPcie);
    if (LutIndex < 0) {
      DEBUG ((DEBUG_WARN, "No free Lut Index for Pcie\n"));
      return EFI_DEVICE_ERROR;
    }

    // Get a free StreamId from pool, if not found we are done
    StreamId = PcieGetStreamId (LsPcie);
    if (StreamId < 0) {
      DEBUG ((DEBUG_WARN, "No free StreamId for Pcie\n"));
      return EFI_DEVICE_ERROR;
    }

    Status = FdtPcieSetIommuMapEntry (Dtb, PcieNodeOffset, 0, StreamId);
    if (EFI_ERROR (Status) && (Status != EFI_NOT_FOUND)) {
      return Status;
    }

    Status = FdtPcieSetMsiMapEntry (Dtb, PcieNodeOffset, 0, StreamId);
    if (EFI_ERROR (Status) && (Status != EFI_NOT_FOUND)) {
      return Status;
    }

    PcieLutSetMapping (LsPcie, LutIndex, 0, StreamId);
  } else {
    FdtStatus = fdt_setprop_string (Dtb, PcieNodeOffset, "status", "disabled");
  }

  if (FdtStatus) {
    DEBUG ((DEBUG_ERROR, "Error: couldn't set the status %a!\n", fdt_strerror (FdtStatus)));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Return all the root bridge instances in an array.

  @param Count  Return the count of root bridge instances.

  @return All the root bridge instances in an array.

**/
PCI_ROOT_BRIDGE *
EFIAPI
PciHostBridgeGetRootBridges (
  OUT UINTN     *Count
  )
{
  UINTN         Idx;
  INTN          LinkUp;
  UINT64        PciPhyMemAddr;
  UINT64        PciPhyMem64Addr;
  UINT64        PciPhyCfg0Addr;
  UINT64        PciPhyCfg1Addr;
  UINT64        PciPhyIoAddr;
  UINT64        Regs;
  LS_PCIE       *LsPcie;
  EFI_STATUS    Status;
  VOID          *Dtb;

  *Count = 0;

  LsPcie = AllocateZeroPool (sizeof (LS_PCIE) * NUM_PCIE_CONTROLLER);
  if (!LsPcie) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for Pcie controller data\n"));
    return NULL;
  }

  Status = EfiGetSystemConfigurationTable (&gFdtTableGuid, &Dtb);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Did not find the Dtb Blob.\n"));
  }

  if (PcdGet64 (PcdIortTablePtr) == 0) {
    DEBUG ((DEBUG_ERROR, "Could not get IORT table Ptr\n"));
  }

  for (Idx = 0; Idx < NUM_PCIE_CONTROLLER; Idx++) {
    PciPhyMemAddr = PCI_SEG0_PHY_MEM_BASE + (PCI_BASE_DIFF * Idx);
    PciPhyMem64Addr = PCI_SEG0_PHY_MEM64_BASE + (PCI_BASE_DIFF * Idx);
    PciPhyCfg0Addr = PCI_SEG0_PHY_CFG0_BASE + (PCI_BASE_DIFF * Idx);
    PciPhyCfg1Addr = PCI_SEG0_PHY_CFG1_BASE + (PCI_BASE_DIFF * Idx);
    PciPhyIoAddr  =  PCI_SEG0_PHY_IO_BASE + (PCI_BASE_DIFF * Idx);
    Regs =  PCI_SEG0_DBI_BASE + (PCI_DBI_SIZE_DIFF * Idx);

    // Filling local array for
    // PCIe controller Physical address space for Host Contoller address
    //
    LsPcie[Idx].ControllerAddress = Regs;
    LsPcie[Idx].NextLutIndex = 0;
    LsPcie[Idx].LsPcieLut = (LS_PCIE_LUT *)(Regs + PCI_LUT_BASE);
    LsPcie[Idx].ControllerIndex = Idx;
    LsPcie[Idx].CurrentStreamId = 0;

    //
    // Verify PCIe controller is enabled in Soc Serdes Map
    //
    if (!IsPcieNumEnabled (Idx)) {
      DEBUG ((DEBUG_ERROR, "PCIE%d is disabled\n", (Idx + BIT0)));

      if (Dtb != NULL) {
        Status = FdtFixupPcieStatus (Dtb, &LsPcie[Idx], FALSE);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Could not set Pcie status retval %r\n", Status));
        }
      }

      //
      // Continue with other PCIe controller
      //
      continue;
    }

    DEBUG ((DEBUG_INFO, "PCIE%d is Enabled\n", Idx + BIT0));

    if (Dtb != NULL) {
      Status = FdtFixupPcieStatus (Dtb, &LsPcie[Idx], TRUE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Could not set Pcie status retval %r\n", Status));
      }
    }

    //
    // Verify PCIe controller LTSSM state
    //
    LinkUp = PcieLinkUp(Regs);
    if (!LinkUp) {
      //
      // Let the user know there's no PCIe link
      //
      DEBUG ((DEBUG_INFO,"no link, regs @ 0x%lx\n", Regs));
      //
      // Continue with other PCIe controller
      //
      continue;
    }
    DEBUG ((DEBUG_INFO, "PCIE%d Passed Linkup Phase\n", Idx + BIT0));

    //
    // Function to set up address translation unit outbound window for
    // PCIe Controller
    //
    PcieSetupCntrl (Regs,
                    PciPhyCfg0Addr,
                    PciPhyCfg1Addr,
                    PciPhyMemAddr,
                    PciPhyMem64Addr,
                    PciPhyIoAddr);

    mPciRootBridges[*Count].Segment               = Idx;
    mPciRootBridges[*Count].Supports              = PCI_SUPPORT_ATTRIBUTES;
    mPciRootBridges[*Count].Attributes            = PCI_SUPPORT_ATTRIBUTES;
    mPciRootBridges[*Count].DmaAbove4G            = TRUE;
    mPciRootBridges[*Count].NoExtendedConfigSpace = FALSE;
    mPciRootBridges[*Count].ResourceAssigned      = FALSE;
    mPciRootBridges[*Count].AllocationAttributes  = PCI_ALLOCATION_ATTRIBUTES;

    mPciRootBridges[*Count].Bus.Base              = PCI_SEG_BUSNUM_MIN;
    mPciRootBridges[*Count].Bus.Limit             = PCI_SEG_BUSNUM_MAX;

    mPciRootBridges[*Count].Io.Base               = PCI_SEG_PORTIO_MIN;
    mPciRootBridges[*Count].Io.Limit              = PCI_SEG_PORTIO_MAX;
    mPciRootBridges[*Count].Io.Translation        = MAX_UINT64 -
                                                    (SEG_IO_SIZE * Idx) + 1;

    mPciRootBridges[*Count].Mem.Base              = SEG_MEM_BASE;
    mPciRootBridges[*Count].Mem.Limit             = SEG_MEM_LIMIT;
    mPciRootBridges[*Count].Mem.Translation       = MAX_UINT64 -
                                                    (PCI_SEG0_MMIO_MEMBASE +
                                                    (PCI_BASE_DIFF * Idx)) + 1;

    mPciRootBridges[*Count].MemAbove4G.Base       = PciPhyMem64Addr;
    mPciRootBridges[*Count].MemAbove4G.Limit      = PciPhyMem64Addr +
                                                    (SIZE_16GB - 1);

    //
    // No separate ranges for prefetchable and non-prefetchable BARs
    //
    mPciRootBridges[*Count].PMem.Base             = MAX_UINT64;
    mPciRootBridges[*Count].PMem.Limit            = 0;
    mPciRootBridges[*Count].PMemAbove4G.Base      = MAX_UINT64;
    mPciRootBridges[*Count].PMemAbove4G.Limit     = 0;
    mPciRootBridges[*Count].DevicePath            = (EFI_DEVICE_PATH_PROTOCOL *)&mEfiPciRootBridgeDevicePath[Idx];

    ++*Count;
  }

  if (*Count == 0) {
    return NULL;
  }

  if (!PlatformHasPciIoEvent) {
    PlatformHasPciIoEvent = EfiCreateProtocolNotifyEvent (
                              &gEfiPciIoProtocolGuid,
                              TPL_CALLBACK,
                              OnPlatformHasPciIo,
                              LsPcie,
                              &PlatformHasPciIoNotifyReg
                              );
  }

  return mPciRootBridges;
}

/**
  Free the root bridge instances array returned from PciHostBridgeGetRootBridges().

  @param Bridges The root bridge instances array.
  @param Count   The count of the array.
**/
VOID
EFIAPI
PciHostBridgeFreeRootBridges (
  PCI_ROOT_BRIDGE *Bridges,
  UINTN           Count
  )
{
}

/**
  Inform the platform that the resource conflict happens.

  @param HostBridgeHandle Handle of the Host Bridge.
  @param Configuration    Pointer to PCI I/O and PCI memory resource
                          descriptors. The Configuration contains the resources
                          for all the root bridges. The resource for each root
                          bridge is terminated with END descriptor and an
                          additional END is appended indicating the end of the
                          entire resources. The resource descriptor field
                          values follow the description in
                          EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
                          .SubmitResources().

**/
VOID
EFIAPI
PciHostBridgeResourceConflict (
  EFI_HANDLE                        HostBridgeHandle,
  VOID                              *Configuration
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Descriptor;
  UINTN                             RootBridgeIndex;
  DEBUG ((DEBUG_ERROR, "PciHostBridge: Resource conflict happens!\n"));

  RootBridgeIndex = 0;
  Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Configuration;
  while (Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    DEBUG ((DEBUG_ERROR, "RootBridge[%d]:\n", RootBridgeIndex++));
    for (; Descriptor->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR; Descriptor++) {
      ASSERT (Descriptor->ResType <
              ARRAY_SIZE (mPciHostBridgeLibAcpiAddressSpaceTypeStr));
      DEBUG ((DEBUG_ERROR, " %s: Length/Alignment = 0x%lx / 0x%lx\n",
              mPciHostBridgeLibAcpiAddressSpaceTypeStr[Descriptor->ResType],
              Descriptor->AddrLen, Descriptor->AddrRangeMax
              ));
      if (Descriptor->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        DEBUG ((DEBUG_ERROR, "     Granularity/SpecificFlag = %ld / %02x%s\n",
                Descriptor->AddrSpaceGranularity, Descriptor->SpecificFlag,
                ((Descriptor->SpecificFlag &
                  EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE
                  ) != 0) ? L" (Prefetchable)" : L""
                ));
      }
    }
    //
    // Skip the END descriptor for root bridge
    //
    ASSERT (Descriptor->Desc == ACPI_END_TAG_DESCRIPTOR);
    Descriptor = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)(
                   (EFI_ACPI_END_TAG_DESCRIPTOR *)Descriptor + 1
                   );
  }

  return;
}
