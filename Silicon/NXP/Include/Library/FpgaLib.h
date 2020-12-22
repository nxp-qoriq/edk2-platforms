/** FpgaLib.h
*  Header defining the Fpga specific functions
*
*  Copyright 2018-2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
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

#ifndef __FPGA_LIB_H__
#define __FPGA_LIB_H__

/**
   Function to get system clock frequency.
**/
UINTN
GetBoardSysClk (
  VOID
  );

/**
   Function to print board personality.
**/
VOID
PrintBoardPersonality (
  VOID
  );

#endif
