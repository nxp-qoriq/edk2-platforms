/** @MmcHost.h

  Copyright 2018 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef MMC_HOST_DXE_H__
#define MMC_HOST_DXE_H__

#include <Protocol/MmcHost.h>

#define MMC_DEVICE_SIGNATURE                 SIGNATURE_32('s','d','x','c')

#define MMC_DEVICE_INSTANCE_FROM_HOST(a)    CR(a, MMC_DEVICE_INSTANCE, MmcHostProtocol, MMC_DEVICE_SIGNATURE)

typedef enum {
  UNKNOWN_CARD,
  MMC_CARD,              //MMC card
  MMC_CARD_HIGH,         //MMC Card with High capacity
  EMMC_CARD,             //eMMC card
  SD_CARD,               //SD card
} CARD_TYPE;

typedef struct _MMC_DEVICE_INSTANCE    MMC_DEVICE_INSTANCE;

struct _MMC_DEVICE_INSTANCE {
  UINT32                             Signature;
  CARD_TYPE                          CardType;
  UINTN                              DeviceBaseAddress;
  MMC_CMD_INFO                       Cmd;
  EFI_MMC_HOST_PROTOCOL              MmcHostProtocol;
};

#endif /* MMC_HOST_DXE_H__ */
