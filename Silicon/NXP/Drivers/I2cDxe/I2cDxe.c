/** I2cDxe.c
  I2c driver APIs for read, write, initialize, set speed and reset

  Copyright 2017, 2020 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/I2cLib.h>
#include <Library/IoLib.h>
#include <Library/SocClockLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Protocol/I2cMaster.h>

#include "I2cDxe.h"

STATIC EFI_EVENT VirtualAddressChangeEvent;

STATIC EFI_PHYSICAL_ADDRESS mI2cRegs;

/**
  Function to set i2c bus frequency

  @param   This            Pointer to I2c master protocol
  @param   BusClockHertz   value to be set

  @retval EFI_SUCCESS      Operation successfull
**/

EFI_STATUS
EFIAPI
SetBusFrequency (
  IN CONST EFI_I2C_MASTER_PROTOCOL   *This,
  IN OUT UINTN                       *BusClockHertz
 )
{
  UINTN                    I2cBase;
  UINT64                   I2cClock;

  I2cBase = mI2cRegs;

  I2cClock = SocGetClock (IP_I2C, PcdGet32 (PcdI2cBus));

  I2cInitialize (I2cBase, I2cClock, *BusClockHertz);

  return EFI_SUCCESS;
}

/**
  Function to reset I2c Controller

  @param  This             Pointer to I2c master protocol

  @return EFI_SUCCESS      Operation successfull
**/
EFI_STATUS
EFIAPI
Reset (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This
  )
{
  UINTN                    I2cBase;

  I2cBase = mI2cRegs;

  return I2cReset (I2cBase);
}

EFI_STATUS
EFIAPI
StartRequest (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This,
  IN UINTN                         SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET        *RequestPacket,
  IN EFI_EVENT                     Event            OPTIONAL,
  OUT EFI_STATUS                   *I2cStatus       OPTIONAL
  )
{
  UINTN                    I2cBase;
  EFI_STATUS               Status;
  EFI_TPL                  Tpl;
  BOOLEAN                  AtRuntime;

  AtRuntime = EfiAtRuntime ();
  if (!AtRuntime) {
    Tpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  }

  I2cBase = mI2cRegs;

  Status = I2cBusXfer (I2cBase, SlaveAddress, RequestPacket);

  if (!AtRuntime) {
    gBS->RestoreTPL (Tpl);
  }

  return Status;
}

CONST EFI_I2C_CONTROLLER_CAPABILITIES I2cControllerCapabilities = {
  0,
  0,
  0,
  0
};

STATIC EFI_I2C_MASTER_PROTOCOL gI2c = {
  ///
  /// Set the clock frequency for the I2C bus.
  ///
  SetBusFrequency,
  ///
  /// Reset the I2C host controller.
  ///
  Reset,
  ///
  /// Start an I2C transaction in master mode on the host controller.
  ///
  StartRequest,
  ///
  /// Pointer to an EFI_I2C_CONTROLLER_CAPABILITIES data structure containing
  /// the capabilities of the I2C host controller.
  ///
  &I2cControllerCapabilities
};

STATIC I2C_DEVICE_PATH gDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH, HW_VENDOR_DP,
      {
        sizeof (VENDOR_DEVICE_PATH), 0
      }
    },
    EFI_CALLER_ID_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL), 0
    }
  }
};

/**
  Fixup controller regs data so that EFI can be call in virtual mode

  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
STATIC
VOID
EFIAPI
I2cVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{

  EfiConvertPointer (0x0, (VOID **)&mI2cRegs);
}

/**
  The Entry Point for I2C driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
I2cDxeEntryPoint (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                Status;


  mI2cRegs = ( EFI_PHYSICAL_ADDRESS)(FixedPcdGet64 (PcdI2c0BaseAddr) +
                         (PcdGet32 (PcdI2cBus) * FixedPcdGet32 (PcdI2cSize)));
  //
  // Install I2c Master protocol on this controller
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                &ImageHandle,
                &gEfiI2cMasterProtocolGuid,
                (VOID**)&gI2c,
                &gEfiDevicePathProtocolGuid,
                &gDevicePath,
                NULL
                );

  // Declare the controller as EFI_MEMORY_RUNTIME
  Status = gDS->AddMemorySpace (
                  EfiGcdMemoryTypeMemoryMappedIo,
                  (EFI_PHYSICAL_ADDRESS)mI2cRegs,
                  (SIZE_64KB),
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gDS->SetMemorySpaceAttributes (
                  (EFI_PHYSICAL_ADDRESS)mI2cRegs,
                   (SIZE_64KB),
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                );

  ASSERT_EFI_ERROR (Status);

    //
    // Register for the virtual address change event
    //
    Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_NOTIFY,
                    I2cVirtualNotifyEvent, NULL,
                    &gEfiEventVirtualAddressChangeGuid,
                    &VirtualAddressChangeEvent);

  return Status;
}

/**
  Unload function for the I2c Driver.

  @param  ImageHandle[in]        The allocated handle for the EFI image

  @retval EFI_SUCCESS            The driver was unloaded successfully
  @retval EFI_INVALID_PARAMETER  ImageHandle is not a valid image handle.

**/
EFI_STATUS
EFIAPI
I2cDxeUnload (
  IN EFI_HANDLE                  ImageHandle
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     *HandleBuffer;
  UINTN                          HandleCount;
  UINTN                          Index;

  //
  // Retrieve all I2c handles in the handle database
  //
  Status = gBS->LocateHandleBuffer (ByProtocol,
                                    &gEfiI2cMasterProtocolGuid,
                                    NULL,
                                    &HandleCount,
                                    &HandleBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Disconnect the driver from the handles in the handle database
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->DisconnectController (HandleBuffer[Index],
                                        gImageHandle,
                                        NULL);
  }

  //
  // Free the handle array
  //
  gBS->FreePool (HandleBuffer);

  //
  // Uninstall protocols installed by the driver in its entrypoint
  //
  Status = gBS->UninstallMultipleProtocolInterfaces (ImageHandle,
                  &gEfiI2cMasterProtocolGuid, &gI2c,
                  &gEfiDevicePathProtocolGuid, &gDevicePath,
                  NULL);

  return Status;
}
