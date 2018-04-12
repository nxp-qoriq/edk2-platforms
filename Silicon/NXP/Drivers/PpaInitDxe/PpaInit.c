/** @file
#
#  DXE driver for loading Primary Protected Application
#
#  Copyright 2017 NXP
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
**/

#include <Bitops.h>
#include <Chipset/AArch64.h>
#include <Library/ArmPlatformLib.h>
#include <Library/BaseMemoryLib/MemLibInternals.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ItbParse.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>

extern EFI_STATUS PpaInit(UINTN *);
extern VOID InitMmu(ARM_MEMORY_REGION_DESCRIPTOR*);
extern VOID DCacheCleanAllLevels(VOID);
extern CHAR8* GetSocName(VOID);

/**
 * Copying PPA firmware to DDR
 */
VOID
CopyPpaImage (
  const char *title,
  UINTN image_addr,
  UINTN image_size,
  UINTN *PpaRamAddr)
{
  DEBUG((EFI_D_INFO, "%a copied to address 0x%x\n", title, PpaRamAddr));

  InternalMemCopyMem((VOID *)PpaRamAddr, (VOID *)image_addr, image_size);

  InvalidateDataCacheRange((VOID *)(UINTN)PpaRamAddr, (UINTN)image_size);
}

UINTN *
GetPpaImagefromFlash (
  VOID
  )
{
  EFI_STATUS Status;
  EFI_PHYSICAL_ADDRESS FitImage;
  EFI_PHYSICAL_ADDRESS PpaImage;
  INT32 CfgNodeOffset;
  INT32 NodeOffset;
  INT32 PpaImageSize;
  UINTN *PpaRamAddr;
  UINTN PpaDdrSize = PcdGet64 (PcdPpaFwSize);

  FitImage = PcdGet64 (PcdPpaFlashBaseAddr);

  // PPA will be placed on DDR at this address:
  PpaRamAddr = (UINTN *)AllocateAlignedRuntimePages((PpaDdrSize / (4 * 1024)) /* no of 4 KB pages to allocate */,
							(64 * 1024 ) /* Alignment = 64 KB*/);
  PpaRamAddr = ZeroMem (PpaRamAddr, (PpaDdrSize));

  Status = FitCheckHeader(FitImage);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Bad FIT image header (0x%x).\n", Status));
    goto EXIT_FREE_FIT;
  }

  Status = FitGetConfNode(FitImage, (VOID *)(PcdGetPtr(PcdPpaFitConfiguration)), &CfgNodeOffset);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Did not find configuration node in FIT header (0x%x).\n", Status));
    goto EXIT_FREE_FIT;
  }

  Status = FitGetNodeFromConf(FitImage, CfgNodeOffset, FIT_FIRMWARE_IMAGE, &NodeOffset);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Did not find PPA node in FIT header (0x%x).\n", Status));
    goto EXIT_FREE_FIT;
  }

  Status = FitGetNodeData(FitImage, NodeOffset, (VOID*)&PpaImage, &PpaImageSize);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Did not find PPA f/w in FIT image (0x%x).\n", Status));
    goto EXIT_FREE_FIT;
  }

  CopyPpaImage ("PPA Firmware", PpaImage, PpaImageSize, PpaRamAddr);

  return PpaRamAddr;

EXIT_FREE_FIT:
  // Flow should never reach here
  ASSERT (Status == EFI_SUCCESS);

  return 0;
}

#define SVR_OFFSET          0xA4
#define REV1_1              0x11
#define GICD_BASE_64K       0x01410000
#define GICC_BASE_64K       0x01420000

VOID
FixGicPcds (
  VOID
  )
{
  UINT32 Rev;
  CHAR8 *Board;

  Board = GetSocName();
  if (NULL == Board)
    return;

  if (!AsciiStrCmp(Board, "LS1043A" )) {

    Rev = MmioReadBe32((UINTN)(PcdGet64(PcdGutsBaseAddr) + SVR_OFFSET)) & MASK_UPPER_8;
    DEBUG((EFI_D_ERROR, "REVISION 0x%x \n", Rev));

    if (Rev == REV1_1) {
      PcdSet64S(PcdGicDistributorBase, GICD_BASE_64K);
      PcdSet64S(PcdGicInterruptInterfaceBase, GICC_BASE_64K);
    }
  }
}

EFI_STATUS
PpaInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;
  UINTN *PpaRamAddr;
  ARM_MEMORY_REGION_DESCRIPTOR *MemoryTable;

  FixGicPcds();
  PpaRamAddr = GetPpaImagefromFlash();

  /* Clean D-cache (all levels) and Invalidate I-cache */
  DCacheCleanAllLevels ();
  ArmInvalidateInstructionCache ();

  Status = PpaInit(PpaRamAddr);
  DEBUG((EFI_D_ERROR, " ppa init done \n"));
  DEBUG((EFI_D_ERROR, " ppa init done \n"));
  DEBUG((EFI_D_ERROR, " ppa init done \n"));
  DEBUG((EFI_D_ERROR, " ppa init done \n"));
  ArmPlatformGetVirtualMemoryMap (&MemoryTable);
  DEBUG((EFI_D_ERROR, " ppa init done \n"));
  InitMmu(MemoryTable);
  DEBUG((EFI_D_ERROR, " ppa init done \n"));

  return Status;
}
