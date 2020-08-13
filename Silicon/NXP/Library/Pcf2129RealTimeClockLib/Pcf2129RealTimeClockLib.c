/** @file

  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>
  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Protocol/I2cMaster.h>

#define SLAVE_ADDRESS             (FixedPcdGet8 (PcdI2cSlaveAddress))

#define PCF2129_CTL3_BIT_BLF      BIT2 // Battery Low Flag
#define PCF2129_SECONDS_MASK      0x7f
#define PCF2129_MINUTES_MASK      0x7f
#define PCF2129_HOURS_MASK        0x3f
#define PCF2129_DAYS_MASK         0x3f
#define PCF2129_WEEKDAYS_MASK     0x07
#define PCF2129_MONTHS_MASK       0x1f

STATIC EFI_HANDLE                 mI2cMasterHandle;
STATIC VOID                       *mI2cMasterEventRegistration;
STATIC EFI_I2C_MASTER_PROTOCOL    *mI2cMaster;
STATIC EFI_EVENT                  mRtcVirtualAddrChangeEvent;

#pragma pack(1)
typedef struct {
  UINT8                           Control[3];
  UINT8                           Seconds;
  UINT8                           Minutes;
  UINT8                           Hours;
  UINT8                           Days;
  UINT8                           Weekdays;
  UINT8                           Months;
  UINT8                           Years;
  UINT8                           SecondAlarm;
  UINT8                           MinuteAlarm;
  UINT8                           HourAlarm;
  UINT8                           DayAlarm;
  UINT8                           WeekdayAlarm;
} RTC_DATETIME;
#pragma pack()

/**
  Returns the current time and date information, and the time-keeping
  capabilities of the hardware platform.

  @param  Time                  A pointer to storage to receive a snapshot of
                                the current time.
  @param  Capabilities          An optional pointer to a buffer to receive the
                                real time clock device's capabilities.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER Time is NULL.
  @retval EFI_DEVICE_ERROR      The time could not be retrieved due to hardware
                                error.

**/
EFI_STATUS
EFIAPI
LibGetTime (
  OUT EFI_TIME                *Time,
  OUT EFI_TIME_CAPABILITIES   *Capabilities
  )
{
  EFI_I2C_REQUEST_PACKET      Op;
  RTC_DATETIME                DateTime;
  EFI_STATUS                  Status;
  UINT8                       Reg;

  if (Time == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mI2cMaster == NULL) {
    return EFI_DEVICE_ERROR;
  }

  Reg = OFFSET_OF (RTC_DATETIME, Control[2]);

  Op.OperationCount = 1;

  Op.Operation[0].Flags = 0;
  Op.Operation[0].LengthInBytes = 1;
  Op.Operation[0].Buffer = &Reg;

  Status = mI2cMaster->StartRequest (mI2cMaster, SLAVE_ADDRESS,
                         (VOID *)&Op, NULL, NULL);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Op.OperationCount = 1;

  Op.Operation[0].Flags = I2C_FLAG_READ;
  Op.Operation[0].LengthInBytes = OFFSET_OF (RTC_DATETIME, SecondAlarm) -
                                  OFFSET_OF (RTC_DATETIME, Control[2]);
  Op.Operation[0].Buffer = (VOID *)&(DateTime.Control[2]);

  Status = mI2cMaster->StartRequest (mI2cMaster, SLAVE_ADDRESS,
                         (VOID *)&Op, NULL, NULL);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if ((DateTime.Control[2] & PCF2129_CTL3_BIT_BLF) != 0) {
    DEBUG ((DEBUG_INFO,
      "### Warning: RTC battery status low, check/replace RTC battery.\n"));
  }

  Time->Nanosecond = 0;
  Time->Second  = BcdToDecimal8 (DateTime.Seconds & PCF2129_SECONDS_MASK);
  Time->Minute  = BcdToDecimal8 (DateTime.Minutes & PCF2129_MINUTES_MASK);
  Time->Hour    = BcdToDecimal8 (DateTime.Hours & PCF2129_HOURS_MASK);
  Time->Day     = BcdToDecimal8 (DateTime.Days & PCF2129_DAYS_MASK);
  Time->Month   = BcdToDecimal8 (DateTime.Months & PCF2129_MONTHS_MASK);
  Time->Year    = BcdToDecimal8 (DateTime.Years);
  Time->Year    += Time->Year >= 98 ? 1900 : 2000;

  if (Capabilities != NULL) {
    Capabilities->Resolution = 1;
    Capabilities->Accuracy = 0;
    Capabilities->SetsToZero = TRUE;
  }
  return EFI_SUCCESS;
}


/**
  Sets the current local time and date information.

  @param  Time                  A pointer to the current time.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The time could not be set due due to hardware
                                error.

**/
EFI_STATUS
EFIAPI
LibSetTime (
  IN EFI_TIME                *Time
  )
{
  EFI_I2C_REQUEST_PACKET      Op;
  RTC_DATETIME                DateTime;
  UINT8                       *Reg;
  EFI_STATUS                  Status;

  if (mI2cMaster == NULL) {
    return EFI_DEVICE_ERROR;
  }

  // start register address
  Reg                          = &(DateTime.Control[2]);
  *Reg                         = OFFSET_OF (RTC_DATETIME, Seconds);

  DateTime.Seconds             = DecimalToBcd8 (Time->Second);
  DateTime.Minutes             = DecimalToBcd8 (Time->Minute);
  DateTime.Hours               = DecimalToBcd8 (Time->Hour);
  DateTime.Days                = DecimalToBcd8 (Time->Day);
  DateTime.Weekdays            = 0;
  DateTime.Months              = DecimalToBcd8 (Time->Month);
  DateTime.Years               = DecimalToBcd8 (Time->Year % 100);

  Op.OperationCount             = 1;
  Op.Operation[0].Flags         = 0;
  Op.Operation[0].LengthInBytes = OFFSET_OF (RTC_DATETIME, SecondAlarm) -
                                  OFFSET_OF (RTC_DATETIME, Control[2]);
  Op.Operation[0].Buffer        = (VOID *)Reg;

  Status = mI2cMaster->StartRequest (mI2cMaster, SLAVE_ADDRESS,
                         (VOID *)&Op, NULL, NULL);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}


/**
  Returns the current wakeup alarm clock setting.

  @param  Enabled               Indicates if the alarm is currently enabled or
                                disabled.
  @param  Pending               Indicates if the alarm signal is pending and
                                requires acknowledgement.
  @param  Time                  The current alarm setting.

  @retval EFI_SUCCESS           The alarm settings were returned.
  @retval EFI_INVALID_PARAMETER Any parameter is NULL.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a
                                hardware error.
  @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this
                                platform.

**/
EFI_STATUS
EFIAPI
LibGetWakeupTime (
  OUT BOOLEAN     *Enabled,
  OUT BOOLEAN     *Pending,
  OUT EFI_TIME    *Time
  )
{
  //
  // Currently unimplemented.
  //
  return EFI_UNSUPPORTED;
}


/**
  Sets the system wakeup alarm clock time.

  @param  Enabled               Enable or disable the wakeup alarm.
  @param  Time                  If Enable is TRUE, the time to set the wakeup
                                alarm for.

  @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was
                                enabled. If Enable is FALSE, then the wakeup
                                alarm was disabled.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a
                                hardware error.
  @retval EFI_UNSUPPORTED       A wakeup timer is not supported on this
                                platform.

**/
EFI_STATUS
EFIAPI
LibSetWakeupTime (
  IN BOOLEAN      Enabled,
  OUT EFI_TIME    *Time
  )
{
  //
  // Currently unimplemented.
  //
  return EFI_UNSUPPORTED;
}

STATIC
VOID
I2cMasterRegistrationEvent (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  EFI_HANDLE                Handle;
  UINTN                     BufferSize;
  EFI_STATUS                Status;
  EFI_I2C_MASTER_PROTOCOL   *I2cMaster;
  UINTN                     BusFrequency;

  //
  // Try to connect the newly registered driver to our handle.
  //
  do {
    BufferSize = sizeof (EFI_HANDLE);
    Status = gBS->LocateHandle (ByRegisterNotify,
                                &gEfiI2cMasterProtocolGuid,
                                mI2cMasterEventRegistration,
                                &BufferSize,
                                &Handle);
    if (EFI_ERROR (Status)) {
      if (Status != EFI_NOT_FOUND) {
        DEBUG ((DEBUG_WARN, "%a: gBS->LocateHandle () returned %r\n",
          __FUNCTION__, Status));
      }
      break;
    }

    if (Handle != mI2cMasterHandle) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "%a: found I2C master!\n", __FUNCTION__));

    gBS->CloseEvent (Event);

    Status = gBS->OpenProtocol (mI2cMasterHandle, &gEfiI2cMasterProtocolGuid,
                    (VOID **)&I2cMaster, gImageHandle, NULL,
                    EFI_OPEN_PROTOCOL_EXCLUSIVE);
    ASSERT_EFI_ERROR (Status);

    Status = I2cMaster->Reset (I2cMaster);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: I2CMaster->Reset () failed - %r\n",
        __FUNCTION__, Status));
      break;
    }

    BusFrequency = FixedPcdGet16 (PcdI2cBusFrequency);
    Status = I2cMaster->SetBusFrequency (I2cMaster, &BusFrequency);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: I2CMaster->SetBusFrequency () failed - %r\n",
        __FUNCTION__, Status));
      break;
    }

    mI2cMaster = I2cMaster;
    break;
  } while (TRUE);
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
  EfiConvertPointer (0x0, (VOID **)&mI2cMaster);
}

/**
  Library entry point

  @param  ImageHandle           Handle that identifies the loaded image.
  @param  SystemTable           System Table for this image.

  @retval EFI_SUCCESS           The operation completed successfully.

**/
EFI_STATUS
EFIAPI
LibRtcInitialize (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS          Status;
  UINTN               BufferSize;

  //
  // Find the handle that marks the controller
  // that will provide the I2C master protocol.
  //
  BufferSize = sizeof (EFI_HANDLE);
  Status = gBS->LocateHandle (ByProtocol,
                  &gPcf2129RealTimeClockLibI2cMasterProtocolGuid, NULL,
                  &BufferSize, &mI2cMasterHandle);
  ASSERT_EFI_ERROR (Status);

  //
  // Register a protocol registration notification callback on the I2C master
  // protocol. This will notify us even if the protocol instance we are looking
  // for has already been installed.
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiI2cMasterProtocolGuid,
    TPL_CALLBACK,
    I2cMasterRegistrationEvent,
    NULL,
    &mI2cMasterEventRegistration);

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
