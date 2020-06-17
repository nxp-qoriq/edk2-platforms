/** @file
  Pcf2129 RTC MM

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __PCF2129_MM_H__
#define __PCF2129_MM_H__

#define EFI_SMM_PCF2129_PROTOCOL_GUID \
  { \
    0x352d6757, 0x50a0, 0x402b, { 0xa6, 0xa4, 0x2b, 0x86, 0x32, 0x7f, 0x8c, 0x11 } \
  }

#define PCF2129_CTRL3_BIT_BLF       BIT2    /* Battery Low Flag*/

// Define EPOCH (1998-JANUARY-01) in the Julian Date representation
#define EPOCH_JULIAN_DATE           2450815

#define FUNCTION_GET_TIME           0
#define FUNCTION_SET_TIME           1

#pragma pack (1)
typedef struct {
  UINT8  Control[3];
  UINT8  Seconds;
  UINT8  Minutes;
  UINT8  Hours;
  UINT8  Days;
  UINT8  Weekdays;
  UINT8  Months;
  UINT8  Years;
  UINT8  SecondAlarm;
  UINT8  MinuteAlarm;
  UINT8  HourAlarm;
  UINT8  DayAlarm;
  UINT8  WeekdayAlarm;
} PCF2129_REGS;

typedef struct {
  UINTN         Function;
  EFI_STATUS    ReturnStatus;
  PCF2129_REGS  Regs;
} SMM_PCF2129_COMMUNICATE_HEADER;
#pragma pack()

extern EFI_GUID gEfiSmmPcf2129ProtocolGuid;

#endif

