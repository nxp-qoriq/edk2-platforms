/** I2cMm.c
  I2c driver APIs for read, write, initialize, set speed and reset

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/I2cLib.h>
#include <Library/MmServicesTableLib.h>

#include <Protocol/I2cMaster.h>

#define I2C_CLOCK                  87500000

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

  I2cClock = I2C_CLOCK;

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

  I2cBase = mI2cRegs;

  Status = I2cBusXfer (I2cBase, SlaveAddress, RequestPacket);

  return Status;
}

STATIC CONST EFI_I2C_CONTROLLER_CAPABILITIES I2cControllerCapabilities = {
  0,
  0,
  0,
  0
};

STATIC EFI_I2C_MASTER_PROTOCOL mI2c = {
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

/**
  The Entry Point for I2C driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
I2cMmEntryPoint (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_MM_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS                Status;
  UINTN                     BusFrequency;

  mI2cRegs = (EFI_PHYSICAL_ADDRESS)FixedPcdGet64 (PcdI2c5BaseAddr);

  Status = mI2c.Reset (&mI2c);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: I2CMaster Reset () failed - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  BusFrequency = FixedPcdGet32 (PcdI2cSpeed);
  Status = mI2c.SetBusFrequency (&mI2c, &BusFrequency);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: I2CMaster SetBusFrequency () failed - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  //
  // Install I2c Master protocol on this controller
  //
  Status = gMmst->MmInstallProtocolInterface (
                    &ImageHandle,
                    &gEfiI2cMasterProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    (VOID**)&mI2c
                    );

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
I2cMmUnload (
  IN EFI_HANDLE                  ImageHandle
  )
{
  EFI_STATUS                     Status;

  //
  // Uninstall protocols installed by the driver in its entrypoint
  //
  Status = gMmst->MmUninstallProtocolInterface (
                    ImageHandle,
                    &gEfiI2cMasterProtocolGuid,
                    &mI2c
                    );

  return Status;
}
