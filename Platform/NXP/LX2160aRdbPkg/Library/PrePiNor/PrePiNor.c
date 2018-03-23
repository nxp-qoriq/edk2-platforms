/** @LX2160aPrePiNor.c
#
#  Copyright 2018 NXP
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

#include <Library/ArmLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PlatformLib.h>
#include <Library/DebugLib.h>

extern VOID DramInit();
extern INTN TimerInit (VOID);
extern VOID ApplyErratum (VOID);
extern RETURN_STATUS EFIAPI SerialPortInitialize (VOID);


VOID
SetUpTimer (
  VOID
  )
{
    UINTN CntFrq;

    CntFrq = 25000000; // use fix one, this will be removed by ATF

    /* Update with accurate clock frequency */
    asm volatile ("msr cntfrq_el0, %0" : : "r" (CntFrq) : "memory");
}


INTN
TimerInit (
  VOID
  )
{
  UINT32 *TimerBase;

  TimerBase = (UINT32 *)0x023e0000;

  SetUpTimer ();

  /* Enable timebase for the cluster */
  MmioWrite32 ((UINTN)TIME_BASE_ENABLE, 0xFF);

  /* Set the bit corresponding to our watchDog-id in the
   * PMU-Physical Core Time Base Enable Register (PCTBENR)
   * to allow the WDT counter to decrement and raise a reset
   * request (if configured in the WDTCONTROL register).
   */

  MmioWrite32 ((WDOG1_PMU_BASE_ADDR + FSL_PMU_PCTBENR_OFFSET), PMU_PCTBEN0_WDT_BIT_MASK);

  /* Enable clock for timer. This is a global setting. */
  MmioWrite32 ((UINTN)TimerBase, 0x1);

  return 0;
}


UINTN mGlobalVariableBase = 0;

VOID CopyImage(UINT8* Dest, UINT8* Src, UINTN Size)
{
  UINTN Count;
  DEBUG((EFI_D_INFO, "Copy Start  Src:0x%llx Dest:0x%llx, UefiMemorySize 0x%x\n", Src, Dest, Size));
  for(Count = 0; Count < Size; Count++) {
    Dest[Count] = Src[Count];
    if (!(Count % 100000))
	DEBUG((EFI_D_INFO, "Wait copy in progress Count %d \n", Count));
  }
  DEBUG((EFI_D_INFO, "Copy to DDR Done \n"));
}

VOID CEntryPoint(
  UINTN UefiMemoryBase,
  UINTN UefiNorBase,
  UINTN UefiMemorySize
)
{
  VOID (*PrePiStart)(VOID);

  // Data Cache enabled on Primary core when MMU is enabled.
  ArmDisableDataCache ();

  // Invalidate instruction cache
  ArmInvalidateInstructionCache ();

  // Enable Instruction Caches on all cores.
  ArmEnableInstructionCache ();
  
  SerialPortInitialize ();
  
  ApplyErratum ();

  TimerInit();
//  DramInit();

  // If Logs of PrePi phase are needed, please hardcode the value of SysClk to 0x5F5E100 in GetSysInfo function (Ch3Soc.c) 
  DEBUG((EFI_D_INFO, "UefiMemoryBase:0x%llx UefiNorBase:0x%llx, UefiMemorySize 0x%x\n", UefiMemoryBase, UefiNorBase, UefiMemorySize));
  CopyImage((VOID*)UefiMemoryBase, (VOID*)UefiNorBase, UefiMemorySize);

  PrePiStart = (VOID (*)())((UINT64)PcdGet64(PcdFvBaseAddress));
  PrePiStart();
}
