/** Pcf2129Rtc.h
*
*  Copyright 2017, 2019-2020 NXP
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

#ifndef __PCF2129RTC_H__
#define __PCF2129RTC_H__

#define BOOTTIME_DEBUG(x)       do { if (!EfiAtRuntime()) DEBUG (x); } while (0)

#define PCF2129_CTRL3_BIT_BLF       BIT2    /* Battery Low Flag*/

// Define EPOCH (1998-JANUARY-01) in the Julian Date representation
#define EPOCH_JULIAN_DATE           2450815

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

#endif // __PCF2129RTC_H__
