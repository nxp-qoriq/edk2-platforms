/** ArmPlatformLib.c
*
*  Contains board initialization functions.
*
*  Based on BeagleBoardPkg/Library/BeagleBoardLib/BeagleBoard.c
*
*  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
*  Copyright (c) 2016, Freescale Semiconductor, Inc. All rights reserved.
*  Copyright 2018-2020 NXP
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

#include <Library/ArmPlatformLib.h>
#include <Ppi/ArmMpCoreInfo.h>
#include <Library/PL011UartLib.h>
#include <Library/PL011UartClockLib.h>

#define AQUANTIA107_IRQ_MASK 0xC

extern VOID SocInit (UINT32 ExternITMask);
/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

/**
  LX2160ARDB has four PL011 serial ports,Out of which UART1 is used as
  serial port console (described by ACPI SPCR) and UART2 for standard
  debug port (described by ACPI DBG2)

  Function to initialize debug UART2 port and not serial port console

 **/
VOID
SerialDebugPortInitialize (
  VOID
  )
{
  UINT64              BaudRate;
  UINT32              ReceiveFifoDepth;
  EFI_PARITY_TYPE     Parity;
  UINT8               DataBits;
  EFI_STOP_BITS_TYPE  StopBits;

  //
  // Initialize the Debug UART port
  //
  ReceiveFifoDepth = 0; // Use the default value for FIFO depth
  Parity = (EFI_PARITY_TYPE)FixedPcdGet8 (PcdUartDefaultParity);
  DataBits = FixedPcdGet8 (PcdUartDefaultDataBits);
  StopBits = (EFI_STOP_BITS_TYPE)FixedPcdGet8 (PcdUartDefaultStopBits);

  BaudRate = (UINTN)FixedPcdGet64 (PcdUartDefaultBaudRate);
  PL011UartInitializePort (
    (UINTN)FixedPcdGet64 (PcdSerialDbgRegisterBase),
    PL011UartClockGetFreq(),
    &BaudRate,
    &ReceiveFifoDepth,
    &Parity,
    &DataBits,
    &StopBits
    );
}

/**
  Placeholder for Platform Initialization

**/
EFI_STATUS
ArmPlatformInitialize (
  IN  UINTN   MpId
  )
{
  SocInit (AQUANTIA107_IRQ_MASK);

  if (FixedPcdGet64 (PcdSerialDbgRegisterBase) != 0) {
    SerialDebugPortInitialize ();
  }

  return EFI_SUCCESS;
}

ARM_CORE_INFO LX2160aMpCoreInfoCTA72x8[] = {
  {
    // Cluster 0, Core 0
    0x0, 0x0,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
};

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  *CoreCount    = sizeof (LX2160aMpCoreInfoCTA72x8) / sizeof (ARM_CORE_INFO);
  *ArmCoreTable = LX2160aMpCoreInfoCTA72x8;

  return EFI_SUCCESS;
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };

EFI_PEI_PPI_DESCRIPTOR      gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  *PpiListSize = sizeof (gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}


UINTN
ArmPlatformGetCorePosition (
  IN UINTN MpId
  )
{
  return 1;
}
