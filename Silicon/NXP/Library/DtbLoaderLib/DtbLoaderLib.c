/** @file
*
*  Copyright 2018 NXP
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <libfdt.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/MmcHost.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/SimpleNetwork.h>

extern UINT64 GetSdxcFrequency();
extern UINTN GetBoardSysClk (VOID);
extern BOOLEAN IsPcieEnabled (UINTN);

extern BOOLEAN IsSerDesLaneProtocolConfigured (UINT64, UINT16);
extern VOID GetSerdesProtocolMaps (UINT64 *);

typedef struct PcieChildInfo {
  UINT8 HasChild;
  UINT8 SeqNum;
} PCI_CHILD_INFO;

PCI_CHILD_INFO PciChildInfo[FixedPcdGet32 (PcdNumPciController)];

#define JR3_OFFSET                 0x400
#define PCIE_COMPAT                "fsl,lx2160a-pcie"

/* PCI - programmed in PEXn_LUT by OS */
/*   4 IDs per controller */
#define NXP_PEX_STREAM_ID_START    0
#define NXP_PEX_STREAM_ID_END      0x100

/* LUT registers */
#define LS_PCIE_LUT_BASE           FixedPcdGet32 (PcdPcieLutBase)
#define LS_PCIE_LUT_DBG            FixedPcdGet32 (PcdPcieLutDbg)
#define PCIE_LUT_UDR(n)            (0x800 + (n) * 8)
#define PCIE_LUT_LDR(n)            (0x804 + (n) * 8)
#define PCIE_LUT_ENABLE            (1 << 31)

#define PCI_BDF(B,D,F)             ((B) << 16 | (D) << 11 | (F) << 8)

/* Stream Ids for DPAA2 */
#define DPAA2_STREAM_ID_START      23
#define DPAA2_STREAM_ID_END        63

/**
 * Find a node who's reg property matches value 'CompatOff'
 * at 'Index' and matches compatiable 
 *
 * @Blob      : ptr to device tree
 * @Compat    : compatible string to match
 * @CompatOff : property value to match
 * @Index     : Reg value index
 */
STATIC
INT32
fdt_node_offset_by_compat_reg (
  IN  VOID        *Blob,
  IN  CONST CHAR8 *Compat,
  IN  UINT32      CompatOff,
  IN  UINT32      Index
  )
{
  INT32           Len;
  INT32           Off;
  CONST fdt32_t   *Reg;
  UINT32          NodeValue;

  Off = fdt_node_offset_by_compatible (Blob, -1, Compat);
  while (Off != -FDT_ERR_NOTFOUND) {
    Reg = fdt_getprop (Blob, Off, "reg", &Len);
    if (Reg) {
      NodeValue = *(Reg + Index);
      NodeValue = SwapBytes32 (NodeValue);
      if (CompatOff == NodeValue) {
        break;
      }
    }
    Off = fdt_node_offset_by_compatible (Blob, Off, Compat);
  }
  return Off;
}

/**
 * Find a node matches compatiable 'Compat' and
 * set property 'Prop' with value 'Val'
 *
 * @Fdt       : ptr to device tree
 * @compat    : compatibe string to match
 * @Prop      : property to set
 * @Val       : value of property to set
 * @Len       : length of value
 * @Create    : create property node if not available
 */
STATIC
VOID
fdt_fixup_by_compatible_field (
  VOID        *Fdt,
  CONST CHAR8 *Compat,
  CONST CHAR8 *Prop,
  CONST VOID  *Val,
  INTN        Len,
  INTN        Create
  )
{
  INTN Offset;

  Offset = fdt_node_offset_by_compatible (Fdt, -1, Compat);
  while (Offset != -FDT_ERR_NOTFOUND) {
    if (Create || (fdt_get_property (Fdt, Offset, Prop, NULL) != NULL))
      fdt_setprop (Fdt, Offset, Prop, Val, Len);
    Offset = fdt_node_offset_by_compatible (Fdt, Offset, Compat);
  }
}

/**
 * Find a node matches compatiable 'Compat' and
 * set property 'Prop' with 32 bit value 'Val'
 *
 * @Fdt       : ptr to device tree
 * @compat    : compatibe string to match
 * @Prop      : property to set
 * @Val       : value of property to set
 * @Len       : length of value
 * @Create    : create property node if not available
 */
STATIC
VOID
fdt_fixup_by_compatible_field_32 (
  VOID        *Fdt,
  CONST CHAR8 *Compat,
  CONST CHAR8 *Prop,
  UINT32      Val,
  INTN        Create
  )
{
  fdt32_t Tmp = cpu_to_fdt32 (Val);
  fdt_fixup_by_compatible_field (Fdt, Compat, Prop, &Tmp, 4, Create);
}

/**
 * Delete jobreing node with reg value JR3_OFFSET
 * This node is used by secure firmware.
 *
 * @BLob       : ptr to device tree
 */
STATIC
VOID
FixupJR (
  VOID  *Blob
  )
{
  INT32 JRNode;
  INT32 Parent;
  INT32 Length;
  INT32 CryptoPath;
  fdt32_t *Reg;

  JRNode = 0;
  Parent = 0;

  CryptoPath = fdt_path_offset (Blob, "crypto");
  if (CryptoPath < 0) {
    DEBUG ((DEBUG_ERROR, "Crypto node not found \n"));
    return;
  }
  Parent = CryptoPath;

  for (;;) {
    JRNode = fdt_node_offset_by_compatible (Blob, Parent, "fsl,sec-v4.0-job-ring");
    if (JRNode == -FDT_ERR_NOTFOUND) {
      DEBUG ((DEBUG_ERROR, "WARNING : sec node not found \n"));
      return;
    }
    Reg = (fdt32_t *)fdt_getprop (Blob, JRNode, "reg", &Length);
    if (*Reg == JR3_OFFSET) {
      fdt_del_node (Blob, JRNode);
      return;
    }
    Parent = JRNode;
  }
}

/**
 * Function to return if SD/MMC card is present.
 */
STATIC
UINTN
SdxcSupported (
  VOID
  )
{
  EFI_MMC_HOST_PROTOCOL  *Host;
  EFI_STATUS             Status;

  Status = gBS->LocateProtocol (&gEfiMmcHostProtocolGuid,
             NULL,
             (VOID **)&Host);

  if (Status != EFI_SUCCESS) {
    DEBUG((DEBUG_ERROR,"Failed to locate Mmc host protocol\n"));
    return Status;
  }

  Status = Host->IsCardPresent(Host);

  return Status;
}

STATIC
VOID
FixupSdhc  (
  VOID  *Blob
  )
{

  if (!SdxcSupported ()) {
    fdt_fixup_by_compatible_field (Blob, "fsl,esdhc", "status",
      "disabled", 4 + 1, 1 );

    return;
  }

  fdt_fixup_by_compatible_field_32 (Blob, "fsl,esdhc", "clock-frequency",
    GetSdxcFrequency (), 1);

  fdt_fixup_by_compatible_field(Blob, "fsl,esdhc", "status", "okay",
    AsciiStrSize ("okay"), 1);
}

STATIC
VOID
FixupPsci (
  VOID  *Blob
  )
{
  INTN  Node;
  INTN  N;
  INTN  Status;
  UINT32 *DevType;

  Node = fdt_subnode_offset (Blob, 0, "cpus");
  if (Node < 0) {
    DEBUG ((DEBUG_ERROR, "Fdt: No cpus node found!!\n"));
    ASSERT(0);
  }
  N = fdt_first_subnode (Blob, Node);
  if (N < 0) {
    DEBUG ((DEBUG_ERROR, "No cpu nodes found!!!\n"));
    ASSERT (0);
  }

  while (1) {
    Status = fdt_setprop_string (Blob, N, "enable-method", "psci");
    if (Status) {
      DEBUG ((DEBUG_ERROR, "fdt_setprop/psci: Could not add enable-method, %a!!\n",
        fdt_strerror (Status)));
      ASSERT (0);
    }
    N = fdt_next_subnode (Blob, N);
    DevType = (fdt32_t *)fdt_getprop (Blob, N, "device_type", 0);

    if (N < 0 || DevType == NULL) {
      break;
    }
  }

  // Create the /psci node if it doesn't exist
  Node = fdt_subnode_offset (Blob, 0, "psci");
  if (Node < 0) {
    Node = fdt_add_subnode(Blob, 0, "psci");
    if (Node < 0) {
      DEBUG ((DEBUG_ERROR, "fdt_add_node: Could not add psci!!, %a\n",
        fdt_strerror (Status)));
      ASSERT (0);
    }

    Status = fdt_setprop_string (Blob, Node, "compatible", "arm,psci-0.2");
    if (Status) {
      DEBUG ((DEBUG_ERROR, "fdt_setprop/psci: Could not add compatiblity, %a!!\n",
        fdt_strerror (Status)));
      ASSERT (0);
    }

    Status = fdt_setprop_string (Blob, Node, "method", "smc");
    if (Status) {
      DEBUG ((DEBUG_ERROR, "fdt_setprop/psci: Could not add method, %a!!\n",
        fdt_strerror (Status)));
      ASSERT (0);
    }

    if (Node < 0) {
      DEBUG ((DEBUG_ERROR, "Fdt: Could not add psci node!!\n"));
      ASSERT(0);
    }
  }
}

STATIC
VOID
FdtPcieLsSetup (
  IN  VOID        *Blob,
  IN  CONST CHAR8 *PciCompat,
  IN  UINTN       CtrlAddr,
  IN  UINT32      Dev,
  IN  UINT64      SerDesProtocolMap
  )
{
  UINTN           Offset;

  Offset = fdt_node_offset_by_compat_reg (
             Blob, PciCompat, (UINT32)CtrlAddr, 1);
  if (Offset < 0) {
    return;
  }

  if (!IsSerDesLaneProtocolConfigured (SerDesProtocolMap, Dev)) {
    fdt_setprop_string (Blob, Offset, "status", "disabled");
  } else {
    fdt_setprop_string (Blob, Offset, "status", "okay");
  }
}

STATIC
VOID
FdtPciSetup (
  IN  VOID  *Blob
  )
{
  UINT64 SerDesProtocolMap;

  GetSerdesProtocolMaps (&SerDesProtocolMap);

  FdtPcieLsSetup (Blob, PCIE_COMPAT, PcdGet64 (PcdPcieExp1SysAddr),
    1, SerDesProtocolMap);

  FdtPcieLsSetup (Blob, PCIE_COMPAT, PcdGet64 (PcdPcieExp2SysAddr),
    2, SerDesProtocolMap);

  FdtPcieLsSetup (Blob, PCIE_COMPAT, PcdGet64 (PcdPcieExp3SysAddr),
    3, SerDesProtocolMap);

  FdtPcieLsSetup (Blob, PCIE_COMPAT, PcdGet64 (PcdPcieExp4SysAddr),
    4, SerDesProtocolMap);

  FdtPcieLsSetup (Blob, PCIE_COMPAT, PcdGet64 (PcdPcieExp5SysAddr),
    5, SerDesProtocolMap);

  FdtPcieLsSetup (Blob, PCIE_COMPAT, PcdGet64 (PcdPcieExp6SysAddr),
    6, SerDesProtocolMap);
}

/*
 * Update PCIe Child Info
 */
STATIC
VOID
UpdatePciChildInfo ()
{
  EFI_STATUS Status;
  UINTN Size;
  UINT32 Id;
  EFI_HANDLE *Handle;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciProtocol;

  Status = gBS->LocateHandleBuffer (ByProtocol,
             &gEfiPciRootBridgeIoProtocolGuid,
             NULL, &Size, &Handle);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to locate PciRootBridgeIoProtocolGuid 0x%x\n",
      Status));
    return;
  }

  if (Size == 0) {
    return;
  }

  for (Id = 0; Id < Size; Id++) {
    Status = gBS->OpenProtocol (Handle[Id],
               &gEfiPciRootBridgeIoProtocolGuid,
               (VOID **)&PciProtocol, Handle[Id],
               Handle[Id], EFI_OPEN_PROTOCOL_GET_PROTOCOL);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "OpenProtocol failed for handle 0x%x \n", Handle[Id]));
      continue;
    }

    PciChildInfo[PciProtocol->SegmentNumber -1].HasChild = 1;
  }
}

/*
 * msi-map is a property to be added to the pci controller
 * node.  It is a table, where each entry consists of 4 fields
 * e.g.:
 *
 *      msi-map = <[DevId] [Phandle-to-msi-ctrl] [stream-id] [count]
 *                 [DevId] [Phandle-to-msi-ctrl] [stream-id] [count]>;
 */
STATIC
VOID
SetMsiMapEntry (
  IN  VOID   *Blob,
  IN  UINTN  PciBase,
  IN  UINT32 DevId,
  IN  UINT32 StreamId
  )
{
  UINT32     *Prop;
  UINT32     Phandle;
  INT32      NodeOffset;
  CHAR8      *Compat;

  Compat = NULL;

  /* find pci controller node */
  NodeOffset = fdt_node_offset_by_compat_reg (Blob, PCIE_COMPAT, PciBase, 1);

  if (NodeOffset < 0) {
    Compat = PCIE_COMPAT;

    NodeOffset = fdt_node_offset_by_compat_reg (Blob, Compat, PciBase, 1);
    if (NodeOffset < 0) {
        DEBUG ((DEBUG_ERROR, "PCI %a node not found \n", Compat));
        return;
    }
  }

  Prop = (UINT32 *)fdt_getprop (Blob, NodeOffset, "msi-parent", 0);
  if (Prop == NULL) {
      DEBUG ((DEBUG_ERROR, "missing msi-parent: PCIe 0x%x\n", PciBase));
      return;
  }
  Phandle = fdt32_to_cpu (*Prop);

  /* set one msi-map row */
  fdt_appendprop_u32 (Blob, NodeOffset, "msi-map", DevId);
  fdt_appendprop_u32 (Blob, NodeOffset, "msi-map", Phandle);
  fdt_appendprop_u32 (Blob, NodeOffset, "msi-map", StreamId);
  fdt_appendprop_u32 (Blob, NodeOffset, "msi-map", 1);
}

/*
 * An iommu-map is a property to be added to the pci controller
 * node.  It is a table, where each entry consists of 4 fields
 * e.g.:
 *
 * iommu-map = <[DevId] [phandle-to-iommu-ctrl] [stream-id] [count]
 *              [DevId] [phandle-to-iommu-ctrl] [stream-id] [count]>;
 */
STATIC
VOID
SetIommuMapEntry (
  IN  VOID   *Blob,
  IN  UINTN  PciBase,
  IN  UINT32 DevId,
  IN  UINT32 StreamId,
  IN  UINT32 StreamEndId,
  IN  INT32  Offset
  )
{
  UINT32 *Prop;
  UINT32 IommuMap[4];
  INT32  Lenp;
  INT32  NodeOffset;
  CHAR8 *Compat;

  Compat = NULL;

  NodeOffset = Offset;
  if (NodeOffset == 0) {
    /* find pci controller node */
    NodeOffset = fdt_node_offset_by_compat_reg (Blob, PCIE_COMPAT, PciBase, 1);

    if (NodeOffset < 0) {
      Compat = PCIE_COMPAT;

      NodeOffset = fdt_node_offset_by_compat_reg (Blob, Compat, PciBase, 1);

      if (NodeOffset < 0) {
        DEBUG ((DEBUG_ERROR, "%a node not found \n", Compat));
        return;
      }
    }
  }

  /* get phandle to iommu controller */
  Prop = fdt_getprop_w (Blob, NodeOffset, "iommu-map", &Lenp);

  if (Prop == NULL) {
    DEBUG ((DEBUG_ERROR, "missing iommu-map: PCIe 0x%x\n", PciBase));
    return;
  }

  /* set iommu-map row */
  IommuMap[0] = cpu_to_fdt32 (DevId);
  IommuMap[1] = *++Prop;
  IommuMap[2] = cpu_to_fdt32 (StreamId);
  IommuMap[3] = cpu_to_fdt32 (StreamEndId + 1);

  if (DevId == 0 || StreamEndId) {
    fdt_setprop_inplace (Blob, NodeOffset, "iommu-map", IommuMap, 16);
  } else {
    fdt_appendprop (Blob, NodeOffset, "iommu-map", IommuMap, 16);
  }
}

STATIC
VOID
LutUpdate (
  IN  UINTN  LutBase,
  IN  UINT32 Value,
  IN  UINT32 Offset
  )
{
  MmioWrite32 ((UINTN)(LutBase + Offset), Value);
}

/*
 * Program a single LUT entry
 */
STATIC
VOID
SetPcieLutMapping (
  IN  UINTN   PciBase,
  IN  UINT32  Index,
  IN  UINT32  DevId,
  IN  UINT32  StreamId
  )
{
    UINTN LutBase;

    LutBase = PciBase + LS_PCIE_LUT_BASE;
    LutUpdate(LutBase, DevId << 16, PCIE_LUT_UDR(Index));
    LutUpdate(LutBase, StreamId | PCIE_LUT_ENABLE, PCIE_LUT_LDR(Index));
}

STATIC
VOID
FdtFixupPcie (
  VOID  *Blob
  )
{
  UINT32 StreamId;
  UINT32 MsiStreamId;
  UINT32 Bdf;
  UINT32 PciNo;
  UINT32 SeqNum;
  UINT32 NextLutIndex;
  UINTN  PciBaseAddress;

  StreamId = NXP_PEX_STREAM_ID_START;
  Bdf = 0;
  NextLutIndex = 0;

  /* Update Pcie Child Info */
  UpdatePciChildInfo();

  /* Scan all known buses */
  for (PciNo = 0, SeqNum = 0; PciNo < FixedPcdGet32 (PcdNumPciController);
       PciNo++, SeqNum++) {
      PciBaseAddress = PcdGet64 (PcdPcieExp1SysAddr) + PciNo * 0x100000;
      if (!IsPcieEnabled (PciBaseAddress)) {
        continue;
      }

      if (StreamId > NXP_PEX_STREAM_ID_END) {
          DEBUG ((DEBUG_ERROR, "No free stream ids available\n"));
          return;
      }

      MsiStreamId =  StreamId | ((PciNo + 1) << 11);

      /* map PCI b.d.f to streamID in LUT */
      SetPcieLutMapping(PciBaseAddress, NextLutIndex, Bdf >> 8, MsiStreamId);

      /* update msi-map in device tree */
      SetMsiMapEntry(Blob, PciBaseAddress, Bdf >> 8, MsiStreamId);

      SetIommuMapEntry(Blob, PciBaseAddress, Bdf >> 8, MsiStreamId, 0 , 0);

      /* Check for child */
      if (PciChildInfo[PciNo].HasChild) {
          SeqNum++;
          PciChildInfo[PciNo].SeqNum = SeqNum;
      }
  }

  /* Scan all children */
  NextLutIndex = 1;
  StreamId = NXP_PEX_STREAM_ID_START + 1;
  for (PciNo = 0; PciNo < FixedPcdGet32 (PcdNumPciController); PciNo++) {
      if (!PciChildInfo[PciNo].HasChild) {
        continue;
      }

      if (StreamId > NXP_PEX_STREAM_ID_END) {
        DEBUG ((DEBUG_ERROR, "No free stream ids available\n"));
        return;
      }

      /* the DT fixup must be relative to the hose first_busno */
      Bdf = (PciChildInfo[PciNo].SeqNum << 16) - 
              PCI_BDF (PciChildInfo[PciNo].SeqNum - 1, 0, 0);

      MsiStreamId =  StreamId | ((PciNo + 1) << 11);

      /* map PCI b.d.f to streamID in LUT */
      SetPcieLutMapping (PciBaseAddress, NextLutIndex, Bdf >> 8, MsiStreamId);

      /* update msi-map in device tree */
      SetMsiMapEntry (Blob, PciBaseAddress, Bdf >> 8, MsiStreamId);

      SetIommuMapEntry (Blob, PciBaseAddress, Bdf >> 8, MsiStreamId, 0, 0);
  }
}

STATIC
VOID
FdtFixupMc (
  VOID *Blob
  )
{
  EFI_SIMPLE_NETWORK_PROTOCOL  *Snp;
  EFI_STATUS                   Status;
  INT32                        NodeOffset;

  NodeOffset = fdt_node_offset_by_compatible (Blob, -1, "fsl,qoriq-mc");
  if (NodeOffset < 0) {
    DEBUG ((DEBUG_ERROR, "Could not locate MC node \n"));
    return;
  }

  SetIommuMapEntry (Blob, 0, DPAA2_STREAM_ID_START, DPAA2_STREAM_ID_START,
      (DPAA2_STREAM_ID_END - DPAA2_STREAM_ID_START), NodeOffset);


  Status = gBS->LocateProtocol (&gEfiSimpleNetworkProtocolGuid,
             NULL,
             (VOID **)&Snp);

  if (Status != EFI_SUCCESS) {
    fdt_setprop_string (Blob, NodeOffset, "status", "disabled");
  } else {
    fdt_setprop_string (Blob, NodeOffset, "status", "okay");
  }
}

STATIC
EFI_STATUS
PrepareFdt (
  IN OUT VOID                 *Fdt,
  IN     UINTN                FdtSize
  )
{
  UINTN SysClk;

  SysClk = GetBoardSysClk ();

  fdt_fixup_by_compatible_field_32 (Fdt, "/sysclk", "clock-frequency", SysClk, 1);

  /* Job ring 3 is used by PPA, so remove it from dtb */
  FixupJR (Fdt);

  FixupSdhc (Fdt);

  FixupPsci (Fdt);

  // Setup PCIe
  FdtPciSetup (Fdt);

  // Fixup for PCIe
  FdtFixupPcie (Fdt);

  // Fixup for MC
  FdtFixupMc (Fdt);

  // Update the real size of the Device Tree
  fdt_pack (Fdt);

  return EFI_SUCCESS;
}

/**
  Return a pool allocated copy of the DTB image that is appropriate for
  booting the current platform via DT.

  @param[out]   Dtb                   Pointer to the DTB copy
  @param[out]   DtbSize               Size of the DTB copy

  @retval       EFI_SUCCESS           Operation completed successfully
  @retval       EFI_NOT_FOUND         No suitable DTB image could be located
  @retval       EFI_OUT_OF_RESOURCES  No pool memory available

**/
EFI_STATUS
EFIAPI
DtPlatformLoadDtb (
  OUT   VOID        **Dtb,
  OUT   UINTN       *DtbSize
  )
{
  EFI_STATUS      Status;
  VOID            *OrigDtb;
  VOID            *CopyDtb;
  UINTN           OrigDtbSize;
  UINTN           CopyDtbSize;
  INT32           Error;

  Status = GetSectionFromAnyFv (&gDtPlatformDefaultDtbFileGuid,
             EFI_SECTION_RAW, 0, &OrigDtb, &OrigDtbSize);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  //
  // Allocate space for the DTB: add a page of slack space to make some room
  // for our modifications.
  //
  CopyDtbSize = OrigDtbSize + EFI_PAGE_SIZE;
  CopyDtb = AllocatePool (CopyDtbSize);
  if (CopyDtb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Error = fdt_open_into (OrigDtb, CopyDtb, CopyDtbSize);
  if (Error != 0) {
    //
    // fdt_open_into() validates the DTB header, so if it fails, the template
    // is most likely invalid.
    //
    DEBUG ((DEBUG_ERROR, "Invalid DTB\n"));
    return EFI_NOT_FOUND;
  }

  Status = PrepareFdt (CopyDtb, CopyDtbSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Dtb = CopyDtb;
  *DtbSize = CopyDtbSize;

  return EFI_SUCCESS;
}
