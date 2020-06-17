/** @PCF2129RtcLib.c
  Implement EFI RealTimeClock with runtime services via RTC Lib for PCF2129 RTC.

  Based on RTC implementation available in
  EmbeddedPkg/Library/TemplateRealTimeClockLib/RealTimeClockLib.c

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
  Copyright 2017, 2019-2020 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Protocol/Pcf2129Mm.h>
#include <Protocol/SmmCommunication.h>

#define BOOTTIME_DEBUG(x)       do { if (!EfiAtRuntime()) DEBUG (x); } while (0)

STATIC EFI_SMM_COMMUNICATION_PROTOCOL  *mSmmCommunication        = NULL;
STATIC UINT8                           *mPcf2129Header           = NULL;
STATIC EFI_EVENT                  mRtcVirtualAddrChangeEvent;

/**
  returns Day of the week [0-6] 0=Sunday
  Don't try to provide a Year that's before 1998, please !
 **/
UINTN
EfiTimeToWday (
  IN  EFI_TIME  *Time
  )
{
  UINTN MonthDiff;
  UINTN Year;
  UINTN Month;
  UINTN JulianDate;  // Absolute Julian Date representation of the supplied Time
  UINTN EpochDays;   // Number of days elapsed since EPOCH_JULIAN_DAY

  MonthDiff = (14 - Time->Month) / 12 ;
  Year = Time->Year + 4800 - MonthDiff;
  Month = Time->Month + (12*MonthDiff) - 3;

  JulianDate = Time->Day + ((153*Month + 2)/5) + (365*Year) + (Year/4) - (Year/100) + (Year/400) - 32045;

  ASSERT (JulianDate >= EPOCH_JULIAN_DATE);
  EpochDays = JulianDate - EPOCH_JULIAN_DATE;

   // 4=1/1/1998 was a Thursday

  return (EpochDays + 4) % 7;
}

/**
  Returns the current time and date information, and the time-keeping capabilities
  of the hardware platform.

  @param  Time                  A pointer to storage to receive a snapshot of the current time.
  @param  Capabilities          An optional pointer to a buffer to receive the real time clock
                                device's capabilities.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER Time is NULL.
  @retval EFI_DEVICE_ERROR      The time could not be retrieved due to hardware error.

**/
EFI_STATUS
EFIAPI
LibGetTime (
  OUT EFI_TIME                *Time,
  OUT  EFI_TIME_CAPABILITIES  *Capabilities
  )
{
  EFI_STATUS                          Status;
  UINTN                               CommSize;
  EFI_SMM_COMMUNICATE_HEADER          *SmmCommunicateHeader;
  SMM_PCF2129_COMMUNICATE_HEADER      *SmmPcf2129Header;

  Status = EFI_SUCCESS;

  if (mPcf2129Header == NULL || mSmmCommunication == NULL) {
    return EFI_DEVICE_ERROR;
  }

  CommSize = sizeof (SMM_PCF2129_COMMUNICATE_HEADER) + sizeof (EFI_MM_COMMUNICATE_HEADER);

  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)mPcf2129Header;
  SmmPcf2129Header = (SMM_PCF2129_COMMUNICATE_HEADER *)SmmCommunicateHeader->Data;
  SmmPcf2129Header->Function = FUNCTION_GET_TIME;

  Status = mSmmCommunication->Communicate(mSmmCommunication, mPcf2129Header, &CommSize);
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (SmmPcf2129Header->ReturnStatus)) {
    BOOTTIME_DEBUG ((DEBUG_ERROR, "RTC read error %r\n", SmmPcf2129Header->ReturnStatus));
    return SmmPcf2129Header->ReturnStatus;
  }

  // hours, minutes and seconds
  Time->Nanosecond = 0;
  Time->Second  = BcdToDecimal8 (SmmPcf2129Header->Regs.Seconds & 0x7F);
  Time->Minute  = BcdToDecimal8 (SmmPcf2129Header->Regs.Minutes & 0x7F);
  Time->Hour = BcdToDecimal8 (SmmPcf2129Header->Regs.Hours & 0x3F);
  Time->Day = BcdToDecimal8 (SmmPcf2129Header->Regs.Days & 0x3F);
  Time->Month  = BcdToDecimal8 (SmmPcf2129Header->Regs.Months & 0x1F);
  Time->Year = BcdToDecimal8 (SmmPcf2129Header->Regs.Years) + \
               ( BcdToDecimal8 (SmmPcf2129Header->Regs.Years) >= 98 ? 1900 : 2000);

  return SmmPcf2129Header->ReturnStatus;
}

/**
  Sets the current local time and date information.

  @param  Time                  A pointer to the current time.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The time could not be set due due to hardware error.

**/

EFI_STATUS
EFIAPI
LibSetTime (
  IN EFI_TIME                *Time
  )
{
  EFI_STATUS                          Status;
  UINTN                               CommSize;
  EFI_SMM_COMMUNICATE_HEADER          *SmmCommunicateHeader;
  SMM_PCF2129_COMMUNICATE_HEADER      *SmmPcf2129Header;

  Status = EFI_SUCCESS;

  if (mPcf2129Header == NULL || mSmmCommunication == NULL) {
    return EFI_DEVICE_ERROR;
  }

  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)mPcf2129Header;
  SmmPcf2129Header = (SMM_PCF2129_COMMUNICATE_HEADER *)SmmCommunicateHeader->Data;
  SmmPcf2129Header->Function = FUNCTION_SET_TIME;

  // hours, minutes and seconds
  SmmPcf2129Header->Regs.Seconds = DecimalToBcd8 (Time->Second);
  SmmPcf2129Header->Regs.Minutes = DecimalToBcd8 (Time->Minute);
  SmmPcf2129Header->Regs.Hours = DecimalToBcd8 (Time->Hour);
  SmmPcf2129Header->Regs.Days = DecimalToBcd8 (Time->Day);
  SmmPcf2129Header->Regs.Weekdays = EfiTimeToWday (Time) & 0x07;
  SmmPcf2129Header->Regs.Months = DecimalToBcd8 (Time->Month);
  SmmPcf2129Header->Regs.Years = DecimalToBcd8 (Time->Year % 100);

  CommSize = sizeof (SMM_PCF2129_COMMUNICATE_HEADER) + sizeof (EFI_MM_COMMUNICATE_HEADER);

  Status = mSmmCommunication->Communicate(mSmmCommunication, mPcf2129Header, &CommSize);
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (SmmPcf2129Header->ReturnStatus)) {
    BOOTTIME_DEBUG ((DEBUG_ERROR, "RTC write error %r\n", SmmPcf2129Header->ReturnStatus));
    return SmmPcf2129Header->ReturnStatus;
  }

  return SmmPcf2129Header->ReturnStatus;
}


/**
  Returns the status of current wakeup alarm clock setting.

  @param  Enabled               Indicates if the alarm is currently enabled or disabled.
  @param  Pending               Indicates if the alarm signal is pending and requires acknowledgement.
  @param  Time                  The current alarm setting.

  @retval EFI_SUCCESS           The alarm settings were returned.
  @retval EFI_INVALID_PARAMETER Any parameter is NULL.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a hardware error.
**/
EFI_STATUS
EFIAPI
LibGetWakeupTime (
  OUT BOOLEAN     *Enabled,
  OUT BOOLEAN     *Pending,
  OUT EFI_TIME    *Time
  )
{
  // Not a required feature
  return EFI_UNSUPPORTED;
}


/**
  Sets the system wakeup alarm clock time.

  @param  Enabled               Enable or disable the wakeup alarm.
  @param  Time                  If Enable is TRUE, the time to set the wakeup alarm for.

  @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was enabled. If
                                Enable is FALSE, then the wakeup alarm was disabled.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a hardware error.
**/
EFI_STATUS
EFIAPI
LibSetWakeupTime (
  IN BOOLEAN      Enabled,
  OUT EFI_TIME    *Time
  )
{
  // Not a required feature
  return EFI_UNSUPPORTED;
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in
  lib to virtual mode.

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
VOID
EFIAPI
LibRtcVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EfiConvertPointer (0x0, (VOID **)&mSmmCommunication);
  EfiConvertPointer (0x0, (VOID **)&mPcf2129Header);
}

/**
  This is the declaration of an EFI image entry point. This can be the entry point to an application
  written to this specification, an EFI boot service driver, or an EFI runtime driver.

  @param  ImageHandle           Handle that identifies the loaded image.
  @param  SystemTable           System Table for this image.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_DEVICE_ERROR      The operation could not be started.

**/
EFI_STATUS
EFIAPI
LibRtcInitialize (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{

  EFI_STATUS                    Status;
  UINTN                         Pcf2129HeaderSize;
  EFI_SMM_COMMUNICATE_HEADER    *SmmCommunicateHeader;

  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &mSmmCommunication);
  ASSERT_EFI_ERROR (Status);
  //
  // Allocate memory for Pcf2129 communicate buffer.
  //
  ///
  /// To avoid confusion in interpreting frames, the communication buffer should always
  /// begin with EFI_MM_COMMUNICATE_HEADER
  ///
  Pcf2129HeaderSize = sizeof (SMM_PCF2129_COMMUNICATE_HEADER) + OFFSET_OF (EFI_MM_COMMUNICATE_HEADER, Data);

  mPcf2129Header = AllocateRuntimeZeroPool (Pcf2129HeaderSize);

  ASSERT (mPcf2129Header != NULL);

  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)mPcf2129Header;
  CopyGuid (&SmmCommunicateHeader->HeaderGuid, &gEfiSmmPcf2129ProtocolGuid);
  SmmCommunicateHeader->MessageLength = sizeof (SMM_PCF2129_COMMUNICATE_HEADER);

  //
  // Register for the virtual address change event
  //
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_NOTIFY,
                  LibRtcVirtualNotifyEvent, NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mRtcVirtualAddrChangeEvent);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
