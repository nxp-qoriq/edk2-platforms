/** @file

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef CHASSIS_H__
#define CHASSIS_H__

#include <Uefi.h>

#define TP_ITYP_AV_MASK            0x00000001  /* Initiator available */
#define TP_ITYP_TYPE_MASK(x)       (((x) & 0x6) >> 1) /* Initiator Type */
#define TP_ITYP_TYPE_ARM           0x0
#define TP_ITYP_TYPE_PPC           0x1
#define TP_ITYP_TYPE_OTHER         0x2  /* StarCore DSP */
#define TP_ITYP_TYPE_HA            0x3  /* HW Accelerator */
#define TP_ITYP_THDS(x)            (((x) & 0x18) >> 3)  /* # threads */
#define TP_ITYP_VERSION(x)         (((x) & 0xe0) >> 5)  /* Initiator Version */
#define TP_CLUSTER_INIT_MASK       0x0000003f  /* initiator mask */
#define TP_INIT_PER_CLUSTER        4

#define TY_ITYP_VERSION_A7         0x1
#define TY_ITYP_VERSION_A53        0x2
#define TY_ITYP_VERSION_A57        0x3
#define TY_ITYP_VERSION_A72        0x4

#define CPU_TYPE_ENTRY(N, V, NC) \
           { .Name = #N, .SocVer = SVR_##V, .NumCores = (NC)}

#define SVR_WO_E                    0xFFFFFE
#define SVR_LS1043A                 0x879200
#define SVR_LS1046A                 0x870700
#define SVR_LS2088A                 0x870901
#define SVR_LX2160A                 0x873600
#define SVR_LS1028A                 0x870B00
#define SVR_LX2160A_REV_MASK        0xffff00ff
#define SVR_LX2160A_REV1            0x87360010
#define SVR_LX2160A_REV2            0x87360020

#define SVR_MAJOR(svr)              (((svr) >> 4) & 0xf)
#define SVR_MINOR(svr)              (((svr) >> 0) & 0xf)
#define IS_E_PROCESSOR(svr)         (!((svr >> 8) & 0x1))

#define MEGA_HZ                     1000000

typedef struct {
  CHAR8  Name[16];
  UINT32 SocVer;
  UINT32 NumCores;
} CPU_TYPE;

typedef struct {
  UINTN CpuClk;  /* CPU clock in Hz! */
  UINTN BusClk;
  UINTN MemClk;
  UINTN PciClk;
  UINTN SdhcClk;
} SOC_CLOCK_INFO;

/*
 * Print Soc information
 */
VOID
PrintSoc (
  VOID
  );

/*
 * Initialize Clock structure
 */
VOID
ClockInit (
  VOID
  );
/*
 * Print CPU information
 */
VOID
PrintCpuInfo (
  VOID
  );

/*
 * Dump RCW (Reset Control Word) on console
 */
VOID
PrintRCW (
  VOID
  );

UINT32
InitiatorType (
  IN UINT32 Cluster,
  IN UINTN InitId
  );

/*
 *  Return the mask for number of cores on this SOC.
 */
UINT32
CpuMask (
  VOID
  );

/*
 *  Return the number of cores on this SOC.
 */
UINTN
CpuNumCores (
  VOID
  );

/*
 * Return the type of initiator for core/hardware accelerator for given core index.
 */
UINT32
QoriqCoreToType (
  IN UINTN Core
  );

/*
 *  Return the cluster of initiator for core/hardware accelerator for given core index.
 */
UINT32
QoriqCoreToCluster (
  IN UINTN Core
  );

#define  NXP_LAYERSCAPE_CHASSIS2_DCFG_ADDRESS  0x1EE0000
#define  NXP_LAYERSCAPE_CHASSIS2_SCFG_ADDRESS  0x1570000

#define SVR_SOC_VER(svr)            (((svr) >> 8) & 0xFFFFFE)
#define SVR_MAJOR(svr)              (((svr) >> 4) & 0xf)
#define SVR_MINOR(svr)              (((svr) >> 0) & 0xf)
#define IS_E_PROCESSOR(svr)         (!((svr >> 8) & 0x1))

/* SMMU Defintions */
#define SMMU_BASE_ADDR             0x09000000
#define SMMU_REG_SCR0              (SMMU_BASE_ADDR + 0x0)
#define SMMU_REG_SACR              (SMMU_BASE_ADDR + 0x10)
#define SMMU_REG_NSCR0             (SMMU_BASE_ADDR + 0x400)

#define SCR0_USFCFG_MASK           0x00000400
#define SCR0_CLIENTPD_MASK         0x00000001
#define SACR_PAGESIZE_MASK         0x00010000

#define USB_PHY1_BASE_ADDRESS      0x084F0000
#define USB_PHY2_BASE_ADDRESS      0x08500000
#define USB_PHY3_BASE_ADDRESS      0x08510000

/**
  The Device Configuration Unit provides general purpose configuration and
  status for the device. These registers only support 32-bit accesses.
**/
#pragma pack(1)
typedef struct {
  UINT8   Reserved0[0x70 - 0x0];
  UINT32  DeviceDisableRegister1;  // Device Disable Register 1
  UINT32  DeviceDisableRegister2;  // Device Disable Register 2
  UINT32  DeviceDisableRegister3;  // Device Disable Register 3
  UINT32  DeviceDisableRegister4;  // Device Disable Register 4
  UINT32  DeviceDisableRegister5;  // Device Disable Register 5
  UINT8   Reserved1[0xa4 - 0x84];
  UINT32  Svr;                     // System Version Register
  UINT8   Reserved2[0x100 - 0xa8];
  UINT32  RcwSr[16]; // Reset Control Word Status Register
} NXP_LAYERSCAPE_CHASSIS2_DEVICE_CONFIG;
#pragma pack()

/* Supplemental Configuration Unit (SCFG) */
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
#define SCFG_SNPCNFGCR_SECRDSNP         BIT31
#define SCFG_SNPCNFGCR_SECWRSNP         BIT30
#define SCFG_SNPCNFGCR_SATARDSNP        BIT23
#define SCFG_SNPCNFGCR_SATAWRSNP        BIT22
#define SCFG_SNPCNFGCR_USB1RDSNP        BIT21
#define SCFG_SNPCNFGCR_USB1WRSNP        BIT20
#define SCFG_SNPCNFGCR_USB2RDSNP        BIT15
#define SCFG_SNPCNFGCR_USB2WRSNP        BIT16
#define SCFG_SNPCNFGCR_USB3RDSNP        BIT13
#define SCFG_SNPCNFGCR_USB3WRSNP        BIT14
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
     Setting RCW PinMux Register bits 21-23 to select USB2_PWRFAULT
     Setting RCW PinMux Register bits 25-27 to select USB3_DRVVBUS
     Setting RCW PinMux Register bits 29-31 to select USB3_DRVVBUS
     **/
#define SCFG_RCWPMUXCRO_SELCR_USB       0x3333
  /**Setting RCW PinMux Register bits 17-19 to select USB2_DRVVBUS
     Setting RCW PinMux Register bits 21-23 to select USB2_PWRFAULT
     Setting RCW PinMux Register bits 25-27 to select IIC4_SCL
     Setting RCW PinMux Register bits 29-31 to select IIC4_SDA
     **/
#define SCFG_RCWPMUXCRO_NOT_SELCR_USB   0x3300
  UINT32 UsbDrvVBusSelCr;
#define SCFG_USBDRVVBUS_SELCR_USB1      0x00000000
#define SCFG_USBDRVVBUS_SELCR_USB2      0x00000001
#define SCFG_USBDRVVBUS_SELCR_USB3      0x00000003
  UINT32 UsbPwrFaultSelCr;
#define SCFG_USBPWRFAULT_INACTIVE       0x00000000
#define SCFG_USBPWRFAULT_SHARED         0x00000001
#define SCFG_USBPWRFAULT_DEDICATED      0x00000002
#define SCFG_USBPWRFAULT_USB3_SHIFT     4
#define SCFG_USBPWRFAULT_USB2_SHIFT     2
#define SCFG_USBPWRFAULT_USB1_SHIFT     0
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
} NXP_LAYERSCAPE_CHASSIS2_SUPPLEMENTAL_CONFIG;

UINT32
EFIAPI
GurRead (
  IN  UINTN     Address
  );

#endif // CHASSIS_H__
