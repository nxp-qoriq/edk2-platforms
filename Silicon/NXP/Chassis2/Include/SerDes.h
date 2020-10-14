/** SerDes.h
  Header file for SoC specific SerDes routines

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef SERDES_H
#define SERDES_H

#define SRDS_MAX_LANES     4

VOID
GetSerDesProtocolMap (
  OUT UINT64               *SerDesProtocolMap
  );


typedef enum {
  SRDS_1  = 0,
  SRDS_2,
  SRDS_MAX_NUM
} SERDES_NUMBER;

typedef struct {
  UINT16 Protocol;
  UINT8  SrdsLane[SRDS_MAX_LANES];
} SERDES_CONFIG;

typedef VOID
(*SERDES_PROBE_LANES_CALLBACK) (
  IN UINT32 LaneProtocol,
  IN VOID *Arg
  );

VOID
SerDesProbeLanes (
  IN SERDES_PROBE_LANES_CALLBACK SerDesLaneProbeCallback,
  IN VOID                        *Arg
  );
#endif
