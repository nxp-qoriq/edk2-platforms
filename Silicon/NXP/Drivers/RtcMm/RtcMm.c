/** I2cMm.c
  I2c driver APIs for read, write, initialize, set speed and reset

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/MmServicesTableLib.h>

#include <Protocol/I2cMaster.h>
#include <Protocol/Pcf2129Mm.h>

STATIC EFI_I2C_MASTER_PROTOCOL    *mI2cMaster;
STATIC EFI_HANDLE                 mRtcHandle;

/**
  Write RTC register.

  @param  SlaveDeviceAddress   Slave device address offset of RTC to be read.
  @param  RtcRegAddr           Register offset of RTC to write.
  @param  Val                  Value to be written

**/
STATIC
VOID
I2CWriteMux (
  IN  UINT8                SlaveDeviceAddress,
  IN  UINT8                RtcRegAddr,
  IN  UINT8                Val
  )
{
  EFI_I2C_REQUEST_PACKET   Req;
  EFI_STATUS               Status;
  UINT8                    Buffer[2];

  Req.OperationCount = 1;
  Buffer[0] = RtcRegAddr;
  Buffer[1] = Val;

  Req.Operation[0].Flags = 0;
  Req.Operation[0].LengthInBytes = sizeof (Buffer);
  Req.Operation[0].Buffer = Buffer;

  Status = mI2cMaster->StartRequest (mI2cMaster, SlaveDeviceAddress,
                                     (VOID *)&Req,
                                     NULL,  NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC write error at Addr:0x%x\n", RtcRegAddr));
  }
}

/**
  Configure the MUX device connected to I2C.

  @param  RegValue               Value to write on mux device register address

**/
VOID
ConfigureMuxDevice (
  IN  UINT8                RegValue
  )
{
  I2CWriteMux (FixedPcdGet8 (PcdMuxDeviceAddress), FixedPcdGet8 (PcdMuxControlRegOffset), RegValue);
}

/**
  Main entry point for an MM handler dispatch or communicate-based callback.

  @param[in]     DispatchHandle  The unique handle assigned to this handler by MmiHandlerRegister().
  @param[in]     Context         Points to an optional handler context which was specified when the
                                 handler was registered.
  @param[in,out] CommBuffer      A pointer to a collection of data in memory that will
                                 be conveyed from a non-MM environment into an MM environment.
  @param[in,out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS                         The interrupt was handled and quiesced. No other handlers
                                              should still be called.
  @retval EFI_WARN_INTERRUPT_SOURCE_QUIESCED  The interrupt has been quiesced but other handlers should
                                              still be called.
  @retval EFI_WARN_INTERRUPT_SOURCE_PENDING   The interrupt is still pending and other handlers should still
                                              be called.
  @retval EFI_INTERRUPT_PENDING               The interrupt could not be quiesced.
**/
EFI_STATUS
SmmPcf2129Handler (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS                           Status;
  SMM_PCF2129_COMMUNICATE_HEADER       *SmmPcf2129Header;
  EFI_I2C_REQUEST_PACKET               Req;
  UINT8                                RtcRegAddr;

  SmmPcf2129Header = CommBuffer;

  //
  // Check if the I2C device is connected though a MUX device.
  //
  if (FixedPcdGetBool (PcdIsRtcDeviceMuxed)) {
    // Switch to the channel connected to Ds3232 RTC
    ConfigureMuxDevice (FixedPcdGet8 (PcdMuxRtcChannelValue));
  }

  switch (SmmPcf2129Header->Function) {
    case FUNCTION_GET_TIME:
      RtcRegAddr = OFFSET_OF (PCF2129_REGS, Control[2]);

      Req.OperationCount = 1;

      Req.Operation[0].Flags = 0;
      Req.Operation[0].LengthInBytes = sizeof (RtcRegAddr);
      Req.Operation[0].Buffer = &RtcRegAddr;

      Status = mI2cMaster->StartRequest (mI2cMaster, FixedPcdGet8 (PcdI2cSlaveAddress),
                                         (VOID *)&Req,
                                         NULL,  NULL);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "RTC read error at Addr:0x%x, Status = %r\n", RtcRegAddr, Status));
        SmmPcf2129Header->ReturnStatus = Status;
        break;
      }

      Req.OperationCount = 1;

      Req.Operation[0].Flags = I2C_FLAG_READ;
      Req.Operation[0].LengthInBytes = OFFSET_OF (PCF2129_REGS, SecondAlarm) - OFFSET_OF (PCF2129_REGS, Control[2]);
      Req.Operation[0].Buffer = &SmmPcf2129Header->Regs.Control[2];

      Status = mI2cMaster->StartRequest (mI2cMaster, FixedPcdGet8 (PcdI2cSlaveAddress),
                                         (VOID *)&Req,
                                         NULL,  NULL);

      if (SmmPcf2129Header->Regs.Control[2] & PCF2129_CTRL3_BIT_BLF) {
        DEBUG ((DEBUG_INFO,
          "### Warning: RTC battery status low, check/replace RTC battery.\n"));
      }

      SmmPcf2129Header->ReturnStatus = Status;
      break;

    case FUNCTION_SET_TIME:
      SmmPcf2129Header->Regs.Control[2] = OFFSET_OF (PCF2129_REGS, Seconds);

      Req.OperationCount = 1;

      Req.Operation[0].Flags = 0;
      Req.Operation[0].LengthInBytes = OFFSET_OF (PCF2129_REGS, SecondAlarm) - OFFSET_OF (PCF2129_REGS, Control[2]);
      Req.Operation[0].Buffer = &SmmPcf2129Header->Regs.Control[2];

      Status = mI2cMaster->StartRequest (mI2cMaster, FixedPcdGet8 (PcdI2cSlaveAddress),
                                         (VOID *)&Req,
                                         NULL,  NULL);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "RTC write error at Addr:0x%x, Status = %r\n", OFFSET_OF (PCF2129_REGS, Seconds), Status));
        SmmPcf2129Header->ReturnStatus = Status;
        break;
      }

      SmmPcf2129Header->ReturnStatus = Status;
      break;

    default:
      break;
  }

  if (FixedPcdGetBool (PcdIsRtcDeviceMuxed)) {
    // Switch to the default channel
    ConfigureMuxDevice (FixedPcdGet8 (PcdMuxDefaultChannelValue));
  }

  return Status;
}

/**
  The Entry Point for Rtc driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
RtcMmEntryPoint (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_MM_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS                Status;

  Status = gMmst->MmLocateProtocol (&gEfiI2cMasterProtocolGuid, NULL, (VOID **)&mI2cMaster);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Register SMM variable SMI handler
  ///
  mRtcHandle = NULL;
  Status = gMmst->MmiHandlerRegister (SmmPcf2129Handler, &gEfiSmmPcf2129ProtocolGuid, &mRtcHandle);
  ASSERT_EFI_ERROR (Status);

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
RtcMmUnload (
  IN EFI_HANDLE                  ImageHandle
  )
{
  EFI_STATUS                     Status;

  //
  // Uninstall protocols installed by the driver in its entrypoint
  //
  Status = gMmst->MmiHandlerUnRegister (mRtcHandle);

  return Status;
}
