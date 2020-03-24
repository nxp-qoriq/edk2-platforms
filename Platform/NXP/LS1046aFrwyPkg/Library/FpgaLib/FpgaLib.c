/** @file

 Copyright 2019-2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/GpioLib.h>
#include <Library/SerialPortLib.h>
#include <Library/BaseLib.h>

/**
   Function to get board system clock frequency.

**/
UINTN
GetBoardSysClk (
  VOID
  )
{
  return 100000000;
}

/**
   Function to get board system clock frequency.

**/
UINTN
GetBoardVersion (
  VOID
  )
{

  UINTN GpioVal = 0;
  SetDir (GPIO2,13,INPUT);
  SetDir (GPIO2,14,INPUT);

  GpioVal = (GetData (GPIO2,13) | GetData (GPIO2,14) << 1);

  return GpioVal;

}

/**
   Function to print board personality.

**/
VOID
PrintBoardPersonality (
  VOID
  )
{

  UINT8 Rev;
  CHAR8    Buf[32];

  Rev = GetBoardVersion();

  switch (Rev)
  {
    case 0x00:
      AsciiStrCpy (Buf, "Board: LS1046AFRWY, Rev: A ");
      SerialPortWrite ((UINT8 *) Buf, AsciiStrLen(Buf));

      break;

    case 0x01:
      AsciiStrCpy (Buf, "Board: LS1046AFRWY, Rev: B ");
      SerialPortWrite ((UINT8 *) Buf, AsciiStrLen(Buf));
      break;

    default:
      AsciiStrCpy (Buf, "Board: LS1046AFRWY, Rev: Unknown ");
      SerialPortWrite ((UINT8 *) Buf, AsciiStrLen(Buf));
      break;
  }
}
