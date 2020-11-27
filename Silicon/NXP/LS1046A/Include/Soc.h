/** @file

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef SOC_H__
#define SOC_H__

#include <Chassis.h>

/**
  Soc Memory Map
**/
#define LS1046A_DRAM0_PHYS_ADDRESS   (BASE_2GB)
#define LS1046A_DRAM0_SIZE           (SIZE_2GB)
#define LS1046A_DRAM1_PHYS_ADDRESS   (BASE_32GB + BASE_2GB)
#define LS1046A_DRAM1_SIZE           (SIZE_32GB - SIZE_2GB)  // 30 GB

#define LS1046A_CCSR_PHYS_ADDRESS    (BASE_16MB)
#define LS1046A_CCSR_SIZE            (SIZE_256MB - SIZE_16MB) // 240MB

#define LS1046A_QSPI0_PHYS_ADDRESS   (BASE_1GB)
#define LS1046A_QSPI0_SIZE           (SIZE_512MB)

#define LS1046A_I2C0_PHYS_ADDRESS    0x2180000
#define LS1046A_I2C_SIZE             0x10000
#define LS1046A_I2C_NUM_CONTROLLERS  4

#define LS1046A_PCI1_PHYS_ADDRESS    FixedPcdGet64 (PcdPciExp1BaseAddr)
#define LS1046A_PCI_SIZE             SIZE_32GB

#define LS1046A_DCFG_ADDRESS         NXP_LAYERSCAPE_CHASSIS2_DCFG_ADDRESS
#define LS1046A_SCFG_ADDRESS         NXP_LAYERSCAPE_CHASSIS2_SCFG_ADDRESS

/**
  Reset Control Word (RCW) Bits

  RCWSR contains the Reset Configuration Word (RCW) information written with
  values read from flash memory by the device at power-on reset and read-only
  upon exiting reset.

  RCW bits in RCWSR registers are mirror of bit position in Little Endian (LE)

RCW Bits |
in RCWSR |
(MSBit 0)| 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
------------------------------------------------------------------------------------------------
LE       | 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
(LSBit 0)|

  Moreover the RCW bits are to be interpreted in below fasion

Bit(s) | Field Name  | Description                  | Notes/comments
----------------------------------------------------------------------
 2-6   | SYS_PLL_RAT | System PLL Multiplier/Ratio  | This field selects the platform
       |             |                              | clock:SYSCLK ratio.
       |             |                              | 0_0011 3:1
       |             |                              | 0_0100 4:1
       |             |                              | 0_1101 13:1
       |             |                              | 0_1111 15:1
       |             |                              | 1_0000 16:1

  which is why the RCW bits in RCWSR registers are parsed this way
**/
#define SYS_PLL_RAT(x)  (((x) >> 25) & 0x1f) // Bits 2-6

typedef NXP_LAYERSCAPE_CHASSIS2_DEVICE_CONFIG LS1046A_DEVICE_CONFIG;
typedef NXP_LAYERSCAPE_CHASSIS2_SUPPLEMENTAL_CONFIG LS1046A_SUPPLEMENTAL_CONFIG;

/* Supplemental Configuration Unit */
typedef struct {
  UINT8  Res000[0x070-0x000];
  UINT32 Usb1Prm1Cr;
  UINT32 Usb1Prm2Cr;
  UINT32 Usb1Prm3Cr;
  UINT32 Usb2Prm1Cr;
  UINT32 Usb2Prm2Cr;
  UINT32 Usb2Prm3Cr;
  UINT32 Usb3Prm1Cr;
  UINT32 Usb3Prm2Cr;
  UINT32 Usb3Prm3Cr;
  UINT8  Res094[0x100-0x094];
  UINT32 Usb2Icid;
  UINT32 Usb3Icid;
  UINT8  Res108[0x114-0x108];
  UINT32 DmaIcid;
  UINT32 SataIcid;
  UINT32 Usb1Icid;
  UINT32 QeIcid;
  UINT32 SdhcIcid;
  UINT32 EdmaIcid;
  UINT32 EtrIcid;
  UINT32 Core0SftRst;
  UINT32 Core1SftRst;
  UINT32 Core2SftRst;
  UINT32 Core3SftRst;
  UINT8  Res140[0x158-0x140];
  UINT32 AltCBar;
  UINT32 QspiCfg;
  UINT8  Res160[0x180-0x160];
  UINT32 DmaMcr;
  UINT8  Res184[0x188-0x184];
  UINT32 GicAlign;
  UINT32 DebugIcid;
  UINT8  Res190[0x1a4-0x190];
  UINT32 SnpCnfgCr;
#define CCSR_SCFG_SNPCNFGCR_SECRDSNP         BIT31
#define CCSR_SCFG_SNPCNFGCR_SECWRSNP         BIT30
#define CCSR_SCFG_SNPCNFGCR_SATARDSNP        BIT23
#define CCSR_SCFG_SNPCNFGCR_SATAWRSNP        BIT22
#define CCSR_SCFG_SNPCNFGCR_USB1RDSNP        BIT21
#define CCSR_SCFG_SNPCNFGCR_USB1WRSNP        BIT20
#define CCSR_SCFG_SNPCNFGCR_USB2RDSNP        BIT15
#define CCSR_SCFG_SNPCNFGCR_USB2WRSNP        BIT16
#define CCSR_SCFG_SNPCNFGCR_USB3RDSNP        BIT13
#define CCSR_SCFG_SNPCNFGCR_USB3WRSNP        BIT14
  UINT8  Res1a8[0x1ac-0x1a8];
  UINT32 IntpCr;
  UINT8  Res1b0[0x204-0x1b0];
  UINT32 CoreSrEnCr;
  UINT8  Res208[0x220-0x208];
  UINT32 RvBar00;
  UINT32 RvBar01;
  UINT32 RvBar10;
  UINT32 RvBar11;
  UINT32 RvBar20;
  UINT32 RvBar21;
  UINT32 RvBar30;
  UINT32 RvBar31;
  UINT32 LpmCsr;
  UINT8  Res244[0x400-0x244];
  UINT32 QspIdQScr;
  UINT32 EcgTxcMcr;
  UINT32 SdhcIoVSelCr;
  UINT32 RcwPMuxCr0;
  /**Setting RCW PinMux Register bits 17-19 to select USB2_DRVVBUS
  *Setting RCW PinMux Register bits 21-23 to select USB2_PWRFAULT
  *Setting RCW PinMux Register bits 25-27 to select USB3_DRVVBUS
  Setting RCW PinMux Register bits 29-31 to select USB3_DRVVBUS*/
#define CCSR_SCFG_RCWPMUXCRO_SELCR_USB      0x3333
  /**Setting RCW PinMux Register bits 17-19 to select USB2_DRVVBUS
  *Setting RCW PinMux Register bits 21-23 to select USB2_PWRFAULT
  *Setting RCW PinMux Register bits 25-27 to select IIC4_SCL
  Setting RCW PinMux Register bits 29-31 to select IIC4_SDA*/
#define CCSR_SCFG_RCWPMUXCRO_NOT_SELCR_USB  0x3300
  UINT32 UsbDrvVBusSelCr;
#define CCSR_SCFG_USBDRVVBUS_SELCR_USB1      0x00000000
#define CCSR_SCFG_USBDRVVBUS_SELCR_USB2      0x00000001
#define CCSR_SCFG_USBDRVVBUS_SELCR_USB3      0x00000003
  UINT32 UsbPwrFaultSelCr;
#define CCSR_SCFG_USBPWRFAULT_INACTIVE       0x00000000
#define CCSR_SCFG_USBPWRFAULT_SHARED         0x00000001
#define CCSR_SCFG_USBPWRFAULT_DEDICATED      0x00000002
#define CCSR_SCFG_USBPWRFAULT_USB3_SHIFT     4
#define CCSR_SCFG_USBPWRFAULT_USB2_SHIFT     2
#define CCSR_SCFG_USBPWRFAULT_USB1_SHIFT     0
  UINT32 UsbRefclkSelcr1;
  UINT32 UsbRefclkSelcr2;
  UINT32 UsbRefclkSelcr3;
  UINT8  Res424[0x600-0x424];
  UINT32 ScratchRw[4];
  UINT8  Res610[0x680-0x610];
  UINT32 CoreBCr;
  UINT8  Res684[0x1000-0x684];
  UINT32 Pex1MsiIr;
  UINT32 Pex1MsiR;
  UINT8  Res1008[0x2000-0x1008];
  UINT32 Pex2;
  UINT32 Pex2MsiR;
  UINT8  Res2008[0x3000-0x2008];
  UINT32 Pex3MsiIr;
  UINT32 Pex3MsiR;
} CCSR_SCFG;

#endif // SOC_H__
