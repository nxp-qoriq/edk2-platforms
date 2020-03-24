/** @file

 Copyright 2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef GPIO_H__
#define GPIO_H__

#include <Uefi.h>

/* enum for direction */
typedef enum _GPIO_BLOCK {
  GPIO1,
  GPIO2,
  GPIO3,
  GPIO4,
  GPIO_MAX
} GPIO_BLOCK;

/* enum for direction */
typedef enum _GPIO_DIRECTION {
  INPUT,
  OUTPUT
} GPIO_DIRECTION;

/* enum for direction */
typedef enum _GPIO_STATE {
  LOW,
  HIGH
} GPIO_VAL;

typedef struct GpioRegs {
  UINT32 GpDir;
  UINT32 GpOdr;
  UINT32 GpData;
  UINT32 GpIer;
  UINT32 GpImr;
  UINT32 GpIcr;
} GPIO_REGS;

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
  );

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
  );

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
  );

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
  );

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
  );

#endif
