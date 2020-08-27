/** @IortLib.h
  IORT Lib specific Macros and structure

  Copyright 2020 NXP
  Copyright 2020 Puresoftware

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef __LS1046AFRWY_IORT_LIB_H__
#define __LS1046AFRWY_IORT_LIB_H__

enum IORT_STATUS {
  IORT_SUCCESS,
  IORT_NODE_NOT_FOUND
};

extern UINT32
SetItsIdMapping (
    VOID    *CurrentTable,
    UINT32  InputId,
    UINT32  OutputId
    );

extern UINT32
SetIommuIdMapping (
    VOID    *CurrentTable,
    UINTN   SegmentNumber,
    UINT32  InputId,
    UINT32  OutputId
    );

#endif
