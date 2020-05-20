/** @file
  PCI memory configuration for NXP

  Copyright 2018-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PCI_H__
#define __PCI_H__

// SVR Versions
#define CFG_SHIFT_ENABLE            (PcdGetBool (PcdPciCfgShiftEnable))
#define PCI_LS_GEN4_CTRL            (PcdGetBool (PcdPciLsGen4Ctrl))
#define PCI_STREAMID_PER_CTRL       (PcdGetBool (PcdPciStreamIdPerCtrl))

// Segment 0
#define PCI_SEG0_NUM              0
#define PCI_SEG0_MMIO_MEMBASE     FixedPcdGet64 (PcdPciExp1BaseAddr)
#define PCI_SEG0_DBI_BASE         0x03400000

// Segment 1
#define PCI_SEG1_NUM              1
#define PCI_SEG1_MMIO_MEMBASE     FixedPcdGet64 (PcdPciExp2BaseAddr)
#define PCI_SEG1_DBI_BASE         0x03500000

// Segment 2
#define PCI_SEG2_NUM              2
#define PCI_SEG2_MMIO_MEMBASE     FixedPcdGet64 (PcdPciExp3BaseAddr)
#define PCI_SEG2_DBI_BASE         0x03600000

// Segment 3
#define PCI_SEG3_NUM              3
#define PCI_SEG3_MMIO_MEMBASE     FixedPcdGet64 (PcdPciExp4BaseAddr)
#define PCI_SEG3_DBI_BASE         0x03700000

// Segment 4
#define PCI_SEG4_NUM              4
#define PCI_SEG4_MMIO_MEMBASE     FixedPcdGet64 (PcdPciExp5BaseAddr)
#define PCI_SEG4_DBI_BASE         0x03800000

// Segment 5
#define PCI_SEG5_NUM              5
#define PCI_SEG5_MMIO_MEMBASE     FixedPcdGet64 (PcdPciExp6BaseAddr)
#define PCI_SEG5_DBI_BASE         0x03900000

// Segment configuration
#define PCI_SEG0_PORTIO_MIN        0x0
#define PCI_SEG0_PORTIO_MAX        0xffff
#define PCI_SEG0_PORTIO_OFFSET     0x0
#define PCI_SEG1_PORTIO_MIN        0x0
#define PCI_SEG1_PORTIO_MAX        0xffff
#define PCI_SEG1_PORTIO_OFFSET     0x10000
#define PCI_SEG2_PORTIO_MIN        0x0
#define PCI_SEG2_PORTIO_MAX        0xffff
#define PCI_SEG2_PORTIO_OFFSET     0x20000
#define PCI_SEG3_PORTIO_MIN        0x0
#define PCI_SEG3_PORTIO_MAX        0xffff
#define PCI_SEG3_PORTIO_OFFSET     0x30000
#define PCI_SEG4_PORTIO_MIN        0x0
#define PCI_SEG4_PORTIO_MAX        0xffff
#define PCI_SEG4_PORTIO_OFFSET     0x40000
#define PCI_SEG5_PORTIO_MIN        0x0
#define PCI_SEG5_PORTIO_MAX        0xffff
#define PCI_SEG5_PORTIO_OFFSET     0x50000

#define PCI_SEG_BUSNUM_MIN        0x0
#define PCI_SEG_BUSNUM_MAX        0xff
#define PCI_SEG_PORTIO_MIN        0x0
#define PCI_SEG_PORTIO_MAX        0xffff
#define PCI_SEG_MMIO32_MIN        0x40000000UL
#define PCI_SEG_MMIO32_MAX        0x4fffffffUL
#define SEG_CFG_SIZE              0x00001000
#define SEG_CFG_BUS               0x00000000
#define SEG_MEM_BASE              0x40000000
#define SEG_MEM_SIZE              0xC0000000
#define SEG_MEM_LIMIT             SEG_MEM_BASE + (SEG_MEM_SIZE -1)
#define SEG_MEM_BUS               0x40000000
#define SEG_IO_BASE               0x10000000
#define SEG_MEM64_BASE            0x400000000
#define SEG_IO_SIZE               0x10000
#define SEG_IO_BUS                0x0
#define PCI_SEG_PORTIO_LIMIT      PCI_SEG5_PORTIO_MAX + PCI_SEG5_PORTIO_OFFSET
#define PCI_BASE_DIFF             0x800000000
#define PCI_DBI_SIZE_DIFF         0x100000
#define PCI_SEG0_PHY_CFG0_BASE    PCI_SEG0_MMIO_MEMBASE
#define PCI_SEG0_PHY_CFG1_BASE    PCI_SEG0_PHY_CFG0_BASE + SEG_CFG_SIZE
#define PCI_SEG0_PHY_MEM_BASE     PCI_SEG0_MMIO_MEMBASE + SEG_MEM_BASE
#define PCI_SEG0_PHY_MEM64_BASE   PCI_SEG0_MMIO_MEMBASE + SEG_MEM64_BASE
#define PCI_SEG0_PHY_IO_BASE      PCI_SEG0_MMIO_MEMBASE + SEG_IO_BASE

#define IATU_REGION_INDEX0        0x0
#define IATU_REGION_INDEX1        0x1
#define IATU_REGION_INDEX2        0x2
#define IATU_REGION_INDEX3        0x3
#define IATU_REGION_INDEX4        0x4
#define IATU_REGION_INDEX5        0x5
#define IATU_REGION_INDEX6        0x6
#define IATU_REGION_INDEX7        0x7

// PCIe Controller configuration
#define NUM_PCIE_CONTROLLER       FixedPcdGet32 (PcdNumPciController)
#define PCI_LUT_DBG               FixedPcdGet32 (PcdPcieLutDbg)
#define PCI_LUT_BASE              FixedPcdGet32 (PcdPcieLutBase)
#define LTSSM_PCIE_L0             0x11
#define PCI_LINK_CAP              0x7c
#define PCI_LINK_SPEED_MASK       0xf


#define IATU_VIEWPORT_OFF                            0x900
#define IATU_VIEWPORT_OUTBOUND                       0

#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0            0x904
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM   0x0
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_IO    0x2
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG0  0x4
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG1  0x5

#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0            0x908
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN  BIT31
#define IATU_ENABLE_CFG_SHIFT_FEATURE                BIT28

#define IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0            0x90C
#define IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_0          0x910
#define IATU_LIMIT_ADDR_OFF_OUTBOUND_0               0x914
#define IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0          0x918
#define IATU_UPPER_TARGET_ADDR_OFF_OUTBOUND_0        0x91C

#define PCI_CLASS_BRIDGE_PCI                         0x0604
#define PCI_CLASS_DEVICE                             0x8
#define PCI_DBI_RO_WR_EN                             0x8bc
#define PCI_BASE_ADDRESS_0                           0x10
#define CLASS_CODE_MASK                              0xffff
#define CLASS_CODE_SHIFT                             0x10

// PAB CSR
#define PAB_CTRL                                     0x808
#define PAB_CTRL_APIO_EN                             0x1
#define PAB_CTRL_PPIO_EN                             (0x1 << 1)
#define PAB_CTRL_PAGE_SEL_SHIFT                      13
#define PAB_CTRL_PAGE_SEL_MASK                       0x3f

/* PPIO WINs RC mode */
#define PAB_PEX_AMAP_CTRL(Idx)                  (0x4ba0 + 0x10 * Idx)
#define PAB_EXT_PEX_AMAP_SIZE(Idx)              (0xbef0 + 0x04 * Idx)
#define PAB_PEX_AMAP_AXI_WIN(Idx)               (0x4ba4 + 0x10 * Idx)
#define PAB_EXT_PEX_AMAP_AXI_WIN(Idx)           (0xb4a0 + 0x04 * Idx)
#define PAB_PEX_AMAP_PEX_WIN_L(Idx)             (0x4ba8 + 0x10 * Idx)
#define PAB_PEX_AMAP_PEX_WIN_H(Idx)             (0x4bac + 0x10 * Idx)
#define IB_TYPE_MEM_F                           0x2
#define IB_TYPE_MEM_NF                          0x3

#define PEX_AMAP_CTRL_TYPE_SHIFT                0x1
#define PEX_AMAP_CTRL_EN_SHIFT                  0x0
#define PEX_AMAP_CTRL_TYPE_MASK                 0x3
#define PEX_AMAP_CTRL_EN_MASK                   0x1


/* PEX PIO Engines */
#define PAB_PEX_PIO_CTRL(Idx)                   (0x8c0 + 0x10 * Idx)
#define PPIO_EN                                 (0x1 << 0)
#define PAB_PEX_PIO_STAT(Idx)                   (0x8c4 + 0x10 * Idx)
#define PAB_PEX_PIO_MT_STAT(Idx)                (0x8c8 + 0x10 * Idx)

// AXI PIO
#define PAB_AXI_PIO_CTRL(Idx)                        (0x840 + 0x10 * Idx)
#define APIO_EN                                      0x1
#define MEM_WIN_EN                                   0x1 << 1
#define IO_WIN_EN                                    0x1 << 2
#define CFG_WIN_EN                                   0x1 << 3

/* GPEX CSR */
#define GPEX_CLASSCODE                          0x474
#define GPEX_CLASSCODE_SHIFT                    16
#define GPEX_CLASSCODE_MASK                     0xffff

// GPEX ACK timeout
#define GPEX_ACK_REPLAY_TO                      0x438
#define ACK_LAT_TO_VAL_SHIFT                    0
#define ACK_LAT_TO_VAL_MASK                     0x1fff


#define INDIRECT_ADDR_BNDRY                          0xc00
#define PAGE_IDX_SHIFT                               10
#define PAGE_ADDR_MASK                               0x3ff

#define OFFSET_TO_PAGE_IDX(Off)                \
       ((Off >> PAGE_IDX_SHIFT) & PAB_CTRL_PAGE_SEL_MASK)

#define OFFSET_TO_PAGE_ADDR(Off)                \
       ((Off & PAGE_ADDR_MASK) | INDIRECT_ADDR_BNDRY)

// APIO WINDOW
#define PAB_AXI_AMAP_CTRL(Idx)                       (0xba0 + 0x10 * Idx)
#define PAB_EXT_AXI_AMAP_SIZE(Idx)                   (0xbaf0 + 0x4 * Idx)
#define PAB_AXI_AMAP_AXI_WIN(Idx)                    (0xba4 + 0x10 * Idx)
#define PAB_EXT_AXI_AMAP_AXI_WIN(Idx)                (0x80a0 + 0x4 * Idx)
#define PAB_AXI_AMAP_PEX_WIN_L(Idx)                  (0xba8 + 0x10 * Idx)
#define PAB_AXI_AMAP_PEX_WIN_H(Idx)                  (0xbac + 0x10 * Idx)

#define AXI_AMAP_CTRL_EN                             0x1
#define AXI_AMAP_CTRL_TYPE_SHIFT                     1
#define AXI_AMAP_CTRL_TYPE_MASK                      0x3
#define AXI_AMAP_CTRL_SIZE_SHIFT                     10
#define AXI_AMAP_CTRL_SIZE_MASK                      0x3fffff

#define PAB_AXI_TYPE_CFG                             0x00
#define PAB_AXI_TYPE_IO                              0x01
#define PAB_AXI_TYPE_MEM                             0x02

#define LX_PEX_CFG_SIZE                              0x00020000

// CCSR registers
#define PCIE_LINK_CTRL_STA                           0x5c
#define PCIE_LINK_SPEED_SHIFT                        16
#define PCIE_LINK_SPEED_MASK                         0x0f
#define PCIE_LINK_WIDTH_SHIFT                        20
#define PCIE_LINK_WIDTH_MASK                         0x3f

// PF control registers
#define PCIE_LTSSM_STA                               0x7fc

VOID GetSerdesProtocolMaps (UINT64 *);

BOOLEAN IsSerDesLaneProtocolConfigured (UINT64, UINT16);

VOID SocPcieCfgShift ();
VOID SocGetPcieCtrl ();
VOID SocGetStreamIdAllocationScheme ();
VOID SocSetPciRootPort ();
#endif
