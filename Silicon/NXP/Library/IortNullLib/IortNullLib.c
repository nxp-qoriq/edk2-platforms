/** @IortNullLib.c
  IortNullLib for platfors which does not have IORT support enabled.

  Copyright 2020 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/IortLib.h>

/**
  ID mappings represent the formula by which an ID from a source is converted to
  an ID in a destination.

  This function doesn't check the table for memory footprint. i.e. it's assumed
  that the memory for ID mapping is avaliable in IORT table.

  @param[in] CurrentTable  IORT table to fixup
  @param[in] InputId       Input Id to ITS block
  @param[in] OutputId      Output Id from ITS block

  @retval    IORT_SUCCESS  IORT table fixed up successfully
 **/
UINT32
SetItsIdMapping (
  VOID    *CurrentTable,
  UINT32  InputId,
  UINT32  OutputId
  )
{
  return IORT_SUCCESS;
}

/**
  ID mappings represent the formula by which an ID from a source is converted to
  an ID in a destination.

  This function doesn't check the table for memory footprint. i.e. it's assumed
  that the memory for ID mapping is avaliable in IORT table.

  @param[in] CurrentTable  IORT table to fixup
  @param[in] SegmentNumber SegmentNumber corresponding to PCIE controller. Used
  to find PCIE controller structure in Iort->PciRcNode
  array.
  @param[in] InputId       The device as identified by BusDeviceFunc Triplet
  @param[in] OutputId      StreamId assigned to the Pcie device.

  @retval IORT_SUCCESS     IORT table fixed up successfully
  @retval IORT_NOT_FOUND   Controller node not found in IORT table
**/
UINT32
SetIommuIdMapping (
  VOID    *CurrentTable,
  UINTN   SegmentNumber,
  UINT32  InputId,
  UINT32  OutputId
  )
{
  return IORT_SUCCESS;
}
