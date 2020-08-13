/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/GpioLib.h>
#include <Library/IoAccessLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>

STATIC MMIO_OPERATIONS *mGpioOps;

/* Structure for GPIO Regsters */
typedef struct GpioRegs {
  UINT32 GpDir;
  UINT32 GpOdr;
  UINT32 GpData;
  UINT32 GpIer;
  UINT32 GpImr;
  UINT32 GpIcr;
} GPIO_REGS;

/**
   GetBaseAddr GPIO controller Base Address

   @param[in] Id  GPIO controller number

   @retval GPIO controller Base Address, if found
   @retval NULL, if not a valid controller number

 **/
STATIC
VOID *
GetBaseAddr (
  IN  UINT8  Id
  )
{

  UINTN      GpioBaseAddr;
  UINTN      MaxGpioController;

  mGpioOps = GetMmioOperations (FeaturePcdGet (PcdGpioControllerBigEndian));

  MaxGpioController = PcdGet32 (PcdNumGpioController);

  if (Id < MaxGpioController) {
    GpioBaseAddr = PcdGet64 (PcdGpioModuleBaseAddress) +
                     (Id * PcdGet64 (PcdGpioControllerOffset));
    return (VOID *) GpioBaseAddr;
  }
  else {
    DEBUG((DEBUG_ERROR, "Invalid Gpio Controller Id %d, Allowed Ids are %d-%d",
      Id, GPIO1, MaxGpioController));
      return NULL;
  }
}

/**
   GetBitMask:  Return Bit Mask

   @param[in] Bit  Bit to create bitmask
   @retval         Bitmask

 **/

STATIC
UINT32
GetBitMask (
  IN  UINT32  Bit
  )
{

  if (!FeaturePcdGet (PcdGpioControllerBigEndian)) {
    return (1 << Bit);
  } else {
    return (1 << (31 - Bit));
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
  GPIO_REGS    *Regs;
  UINT32       BitMask;
  UINT32       Value;

  Regs = GetBaseAddr(Id);
  BitMask = GetBitMask(Bit);

  Value = mGpioOps->Read32 ((UINTN)&Regs->GpDir);

  if (Dir) {
    mGpioOps->Write32 ((UINTN)&Regs->GpDir, (Value | BitMask));
  }
  else {
    mGpioOps->Write32 ((UINTN)&Regs->GpDir, (Value & (~BitMask)));
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
  GPIO_REGS    *Regs;
  UINT32       Value;
  UINT32       BitMask;

  Regs = GetBaseAddr (Id);
  BitMask = GetBitMask(Bit);

  Value = mGpioOps->Read32 ((UINTN)&Regs->GpDir);

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
  GPIO_REGS    *Regs;
  UINT32       Value;
  UINT32       BitMask;

  Regs = (VOID *)GetBaseAddr (Id);
  BitMask = GetBitMask(Bit);


  Value = mGpioOps->Read32 ((UINTN)&Regs->GpData);

  if (Value & BitMask) {
    return 1;
  } else {
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
  GPIO_REGS    *Regs;
  UINT32       BitMask;
  UINT32       Value;

  Regs = GetBaseAddr (Id);
  BitMask = GetBitMask(Bit);

  Value = mGpioOps->Read32 ((UINTN)&Regs->GpData);

  if (Data) {
    mGpioOps->Write32 ((UINTN)&Regs->GpData, (Value | BitMask));
  } else {
    mGpioOps->Write32 ((UINTN)&Regs->GpData, (Value & (~BitMask)));
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
  GPIO_REGS    *Regs;
  UINT32       BitMask;
  UINT32       Value;

  Regs = GetBaseAddr (Id);
  BitMask = GetBitMask(Bit);

  Value = mGpioOps->Read32 ((UINTN)&Regs->GpOdr);
  if (OpenDrain) {
    mGpioOps->Write32 ((UINTN)&Regs->GpOdr,  (Value | BitMask));
  }
  else {
    mGpioOps->Write32 ((UINTN)&Regs->GpOdr,  (Value & (~BitMask)));
  }

  return EFI_SUCCESS;
}
