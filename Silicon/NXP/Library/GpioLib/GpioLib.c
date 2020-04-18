/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/GpioLib.h>
#include <Library/IoAccessLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>

/**
   GetBaseAddr GPIO controller Base Address

   @param[in] Id  GPIO controller number

   @retval GPIO controller Base Address, if found
   @retval NULL, if not a valid controller number

 **/
VOID *
GetBaseAddr (
  IN  UINT8  Id
  )
{

  UINTN GpioBaseAddr;
  UINTN MaxGpioController;

  MaxGpioController = PcdGet32 (PcdNumGpioController);

  if (Id < MaxGpioController) {

    GpioBaseAddr = PcdGet64 (PcdGpioModuleBaseAddress) + (Id * PcdGet64 (PcdGpioControllerOffset));
    return (VOID *) GpioBaseAddr;
  }
  else {
      DEBUG((DEBUG_ERROR, "Invalid Gpio Controller Id %d, Allowed Ids are %d-%d",
                           Id, GPIO1, MaxGpioController));
      return NULL;
  }
}

/**
   SetDir Set GPIO direction as INPUT or OUTPUT

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number
   @param[in] Dir GPIO Direction as INPUT or OUTPUT

   @retval EFI_SUCCESS
 **/
EFI_STATUS
SetDir (
  IN  UINT8    Id,
  IN  UINT32   Bit,
  IN  BOOLEAN  Dir
  )
{
  GPIO_REGS *Regs;
  UINT32 BitMask;
  UINT32 Value;

  Regs = NULL;

  Regs = GetBaseAddr(Id);

  if (!FixedPcdGetBool (PcdGpioControllerBigEndian)) {
    Value = MmioRead32 ((UINTN)&Regs->GpDir);
    BitMask =  (1 << Bit);
    if (Dir) {
      MmioWrite32 ((UINTN)&Regs->GpDir, (Value | BitMask));
    }
    else {
      MmioWrite32 ((UINTN)&Regs->GpDir, (Value & (~BitMask)));
    }
  }
  else {
    Value = SwapMmioRead32 ((UINTN)&Regs->GpDir);
    BitMask =  (1 << (31 - Bit));

    if (Dir) {
      SwapMmioWrite32 ((UINTN)&Regs->GpDir, (Value | BitMask));
    }
    else {
      SwapMmioWrite32 ((UINTN)&Regs->GpDir, (Value & (~BitMask)));
    }
  }

  return EFI_SUCCESS;
}

/**
   GetDir  Retrieve GPIO direction

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number

   @retval GPIO Direction as INPUT or OUTPUT
 **/
UINT32
GetDir (
  IN  UINT8    Id,
  IN  UINT32   Bit
  )
{
  GPIO_REGS *Regs;
  UINT32 Value = 0;
  UINT32 BitMask;

  Regs = NULL;

  Regs = GetBaseAddr (Id);

  if (!FixedPcdGetBool (PcdGpioControllerBigEndian)) {
    Value = MmioRead32 ((UINTN)&Regs->GpDir);
    BitMask = (1 << Bit);
  }
  else {
    Value = SwapMmioRead32 ((UINTN)&Regs->GpDir);
    BitMask = (1 << (31 - Bit));
  }

  return (Value & BitMask);
}

/**
   GetData  Retrieve GPIO Value

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number

   @retval GPIO value  as HIGH or LOW
 **/
UINT32
GetData (
  IN  UINT8    Id,
  IN  UINT32   Bit
  )
{
  GPIO_REGS *Regs;
  UINT32 Value;
  UINT32 BitMask;

  Regs = (VOID *)GetBaseAddr (Id);


  if (!FixedPcdGetBool (PcdGpioControllerBigEndian)) {
    Value = MmioRead32 ((UINTN)&Regs->GpData);
    BitMask = (1 << Bit);
  }
  else {
    Value = SwapMmioRead32 ((UINTN)&Regs->GpData);
    BitMask = (1 << (31 - Bit));
  }

  if (Value & BitMask) {
    return 1;
  }
  else {
    return 0;
  }
}

/**
   SetData  Set GPIO data Value

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number
   @param[in] Data GPIO data value to set

   @retval GPIO value  as HIGH or LOW
 **/
EFI_STATUS
SetData (
  IN  UINT8    Id,
  IN  UINT32   Bit,
  IN  BOOLEAN  Data
  )
{
  GPIO_REGS *Regs;
  UINT32 BitMask;
  UINT32 Value;

  Regs = NULL;

  Regs = GetBaseAddr (Id);

  if (!FixedPcdGetBool (PcdGpioControllerBigEndian)) {
    Value = MmioRead32 ((UINTN)&Regs->GpData);
    BitMask = (1 << Bit);

    if (Data) {
      MmioWrite32 ((UINTN)&Regs->GpData, (Value | BitMask));
    }
    else {
      MmioWrite32 ((UINTN)&Regs->GpData, (Value & (~BitMask)));
    }
  }
  else {
    Value = SwapMmioRead32 ((UINTN)&Regs->GpData);
    BitMask = (1 << ( 31 -  Bit));

    if (Data) {
      SwapMmioWrite32 ((UINTN)&Regs->GpData, (Value | BitMask));
    }
    else {
      SwapMmioWrite32 ((UINTN)&Regs->GpData, (Value & (~BitMask)));
    }
  }
  return EFI_SUCCESS;
}

/**
   SetOpenDrain  Set GPIO as Open drain

   @param[in] Id  GPIO controller number
   @param[in] Bit GPIO number
   @param[in] OpenDrain Set as open drain

   @retval EFI_SUCCESS
 **/
EFI_STATUS
SetOpenDrain (
  IN  UINT8    Id,
  IN  UINT32   Bit,
  IN  BOOLEAN  OpenDrain
  )
{
  GPIO_REGS *Regs;
  UINT32 BitMask;
  UINT32 Value;

  Regs = NULL;
  Regs = GetBaseAddr (Id);

  if (!FixedPcdGetBool (PcdGpioControllerBigEndian)) {
    Value = MmioRead32 ((UINTN)&Regs->GpOdr);
    BitMask = (1 << Bit);
    if (OpenDrain) {
      MmioWrite32 ((UINTN)&Regs->GpOdr,  (Value | BitMask));
    }
    else {
      MmioWrite32 ((UINTN)&Regs->GpOdr,  (Value & (~BitMask)));
    }
  }
  else {
    Value = SwapMmioRead32 ((UINTN)&Regs->GpOdr);
    BitMask = (1 << ( 31 -  Bit));

    if (OpenDrain) {
      SwapMmioWrite32 ((UINTN)&Regs->GpOdr,  (Value | BitMask));
    }
    else {
      SwapMmioWrite32 ((UINTN)&Regs->GpOdr,  (Value & (~BitMask)));
    }
  }

  return EFI_SUCCESS;
}

