/** SerDes.c
  Provides SoC specific SerDes interface

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Bitops.h>
#include <Library/DebugLib.h>
#include <Chassis2/Include/SerDes.h>
#include <Include/SocSerDes.h>
#include <Soc.h>
#include <Uefi.h>

SERDES_CONFIG SerDes1ConfigTbl[] = {
        /* SerDes 1 */
        {0x3333, {SGMII_FM1_DTSEC9, SGMII_FM1_DTSEC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
        {0x1133, {XFI_FM1_MAC9, XFI_FM1_MAC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
        {0x1333, {XFI_FM1_MAC9, SGMII_FM1_DTSEC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
        {0x2333, {SGMII_2500_FM1_DTSEC9, SGMII_FM1_DTSEC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
        {0x2233, {SGMII_2500_FM1_DTSEC9, SGMII_2500_FM1_DTSEC10, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
        {0x1040, {XFI_FM1_MAC9, NONE, QSGMII_FM1_A, NONE } },
        {0x2040, {SGMII_2500_FM1_DTSEC9, NONE, QSGMII_FM1_A, NONE } },
        {0x1163, {XFI_FM1_MAC9, XFI_FM1_MAC10, PCIE1, SGMII_FM1_DTSEC6 } },
        {0x2263, {SGMII_2500_FM1_DTSEC9, SGMII_2500_FM1_DTSEC10, PCIE1, SGMII_FM1_DTSEC6 } },
        {0x3363, {SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6, PCIE1, SGMII_FM1_DTSEC6 } },
        {0x2223, {SGMII_2500_FM1_DTSEC9, SGMII_2500_FM1_DTSEC10, SGMII_2500_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
        {0x3040, {SGMII_FM1_DTSEC9, NONE, QSGMII_FM1_A, NONE } },
        {}
};

SERDES_CONFIG SerDes2ConfigTbl[] = {
        /* SerDes 2 */
        {0x8888, {PCIE1, PCIE1, PCIE1, PCIE1 } },
        {0x5559, {PCIE1, PCIE2, PCIE3, SATA } },
        {0x5577, {PCIE1, PCIE2, PCIE3, PCIE3 } },
        {0x5506, {PCIE1, PCIE2, NONE, PCIE3 } },
        {0x0506, {NONE, PCIE2, NONE, PCIE3 } },
        {0x0559, {NONE, PCIE2, PCIE3, SATA } },
        {0x5A59, {PCIE1, SGMII_FM1_DTSEC2, PCIE3, SATA } },
        {0x5A06, {PCIE1, SGMII_FM1_DTSEC2, NONE, PCIE3 } },
        {}
};

SERDES_CONFIG *SerDesConfigTbl[] = {
        SerDes1ConfigTbl,
        SerDes2ConfigTbl
};

/**
  Function to get serdes Lane protocol corresponding to
  serdes protocol.

  @param  SerDes    Serdes number.
  @param  Cfg       Serdes Protocol.
  @param  Lane      Serdes Lane number.

  @return           Serdes Lane protocol.

**/
STATIC
SERDES_PROTOCOL
GetSerDesPrtcl (
  IN  INTN          SerDes,
  IN  INTN          Cfg,
  IN  INTN          Lane
  )
{
  SERDES_CONFIG     *Config;

  if (SerDes >= ARRAY_SIZE (SerDesConfigTbl)) {
    return 0;
  }

  Config = SerDesConfigTbl[SerDes];
  while (Config->Protocol) {
    if (Config->Protocol == Cfg) {
      return Config->SrdsLane[Lane];
    }
    Config++;
  }

  return EFI_SUCCESS;
}

/**
  Function to check if inputted protocol is a valid serdes protocol.

  @param  SerDes                   Serdes number.
  @param  Prtcl                    Serdes Protocol to be verified.

  @return EFI_INVALID_PARAMETER    Input parameter in invalid.
  @return EFI_NOT_FOUND            Serdes Protocol not a valid protocol.
  @return EFI_SUCCESS              Serdes Protocol is a valid protocol.

**/
STATIC
EFI_STATUS
CheckSerDesPrtclValid (
  IN  INTN      SerDes,
  IN  UINT32    Prtcl
  )
{
  SERDES_CONFIG *Config;
  INTN          Cnt;

  if (SerDes >= ARRAY_SIZE (SerDesConfigTbl)) {
    return EFI_INVALID_PARAMETER;
  }

  Config = SerDesConfigTbl[SerDes];
  while (Config->Protocol) {
    if (Config->Protocol == Prtcl) {
      DEBUG ((DEBUG_INFO, "Protocol: %x Matched with the one in Table\n", Prtcl));
      break;
    }
    Config++;
  }

  if (!Config->Protocol) {
    return EFI_NOT_FOUND;
  }

  for (Cnt = 0; Cnt < SRDS_MAX_LANES; Cnt++) {
    if (Config->SrdsLane[Cnt] != NONE) {
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Function to fill serdes map information.

  @param  Srds                  Serdes number.
  @param  SerdesProtocolMask    Serdes Protocol Mask.
  @param  SerdesProtocolShift   Serdes Protocol shift value.
  @param  SerDesPrtclMap        Pointer to Serdes Protocol map.

**/
STATIC
VOID
LSSerDesMap (
  IN  UINT32                    Srds,
  IN  UINT32                    SerdesProtocolMask,
  IN  UINT32                    SerdesProtocolShift,
  OUT UINT64                    *SerDesPrtclMap
  )
{
  CCSR_GUR                      *Gur;
  UINT32                        SrdsProt;
  INTN                          Lane;
  UINT32                        Flag;

  Gur = (VOID *)PcdGet64 (PcdGutsBaseAddr);
  Flag = 0;

  SrdsProt = GurRead ((UINTN)&Gur->RcwSr[RCWSR_INDEX]) & SerdesProtocolMask;
  SrdsProt >>= SerdesProtocolShift;

  DEBUG ((DEBUG_INFO, "Using SERDES%d Protocol: %d (0x%x)\n",
                                   Srds + 1, SrdsProt, SrdsProt));

  if (EFI_SUCCESS != CheckSerDesPrtclValid (Srds, SrdsProt)) {
    DEBUG ((DEBUG_ERROR, "SERDES%d[PRTCL] = 0x%x is not valid\n",
                                   Srds + 1, SrdsProt));
    Flag++;
  }

  for (Lane = 0; Lane < SRDS_MAX_LANES; Lane++) {
    SERDES_PROTOCOL LanePrtcl = GetSerDesPrtcl (Srds, SrdsProt, Lane);
    if (LanePrtcl >= SERDES_PRTCL_COUNT) {
      DEBUG ((DEBUG_ERROR, "Unknown SerDes lane protocol %d\n", LanePrtcl));
      Flag++;
    } else {
      *SerDesPrtclMap |= BIT (LanePrtcl);
    }
  }

  if (Flag) {
    DEBUG ((DEBUG_ERROR, "Could not configure SerDes module!!\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Successfully configured SerDes module!!\n"));
  }
}

/**
  Get lane protocol on provided serdes lane and execute callback function.

  @param  Srds                    Serdes number.
  @param  SerdesProtocolMask      Mask to get Serdes Protocol for Srds
  @param  SerdesProtocolShift     Shift value to get Serdes Protocol for Srds.
  @param  SerDesLaneProbeCallback Pointer Callback function to be called for Lane protocol
  @param  Arg                     Pointer to Arguments to be passed to callback function.

**/
STATIC
VOID
SerDesInstanceProbeLanes (
  IN  UINT32                      Srds,
  IN  UINT32                      SerdesProtocolMask,
  IN  UINT32                      SerdesProtocolShift,
  IN  SERDES_PROBE_LANES_CALLBACK SerDesLaneProbeCallback,
  IN  VOID                        *Arg
  )
{

  CCSR_GUR                        *Gur;
  UINT32                          SrdsProt;
  INTN                            Lane;

  Gur = (VOID *)PcdGet64 (PcdGutsBaseAddr);;

  SrdsProt = GurRead ((UINTN)&Gur->RcwSr[RCWSR_INDEX]) & SerdesProtocolMask;
  SrdsProt >>= SerdesProtocolShift;

  /*
   * Invoke callback for all lanes in the SerDes instance:
   */
  for (Lane = 0; Lane < SRDS_MAX_LANES; Lane++) {
    SERDES_PROTOCOL LanePrtcl = GetSerDesPrtcl (Srds, SrdsProt, Lane);
    if (LanePrtcl >= SERDES_PRTCL_COUNT || LanePrtcl < NONE) {
      DEBUG ((DEBUG_ERROR, "Unknown SerDes lane protocol %d\n", LanePrtcl));
    }
    else if (LanePrtcl != NONE) {
      SerDesLaneProbeCallback (LanePrtcl, Arg);
    }
  }
}

/**
  Probe all serdes lanes for lane protocol and execute provided callback function.

  @param  SerDesLaneProbeCallback Pointer Callback function to be called for Lane protocol
  @param  Arg                     Pointer to Arguments to be passed to callback function.

**/
VOID
SerDesProbeLanes (
  IN SERDES_PROBE_LANES_CALLBACK SerDesLaneProbeCallback,
  IN VOID                        *Arg
  )
{
  SerDesInstanceProbeLanes (SRDS_1,
                            RCWSR_SRDS1_PRTCL_MASK,
                            RCWSR_SRDS1_PRTCL_SHIFT,
                            SerDesLaneProbeCallback,
                            Arg);

  if (PcdGetBool (PcdSerdes2Enabled)) {
   SerDesInstanceProbeLanes (SRDS_2,
                             RCWSR_SRDS2_PRTCL_MASK,
                             RCWSR_SRDS2_PRTCL_SHIFT,
                             SerDesLaneProbeCallback,
                             Arg);
  }
#if RCWSR_SRDS3_PRTCL_MASK
   SerDesInstanceProbeLanes(SRDS_3,
                            RCWSR_SRDS3_PRTCL_MASK,
                            RCWSR_SRDS3_PRTCL_SHIFT,
                            SerDesLaneProbeCallback,
                            Arg);
#endif
}


/**
  Function to return Serdes protocol map for all serdes available on board.

  @param  SerDesPrtclMap   Pointer to Serdes protocl map.

**/
VOID
GetSerDesProtocolMap (
  OUT UINT64               *SerDesPrtclMap
  )
{
  *SerDesPrtclMap = 0x0;

  LSSerDesMap (SRDS_1,
               RCWSR_SRDS1_PRTCL_MASK,
               RCWSR_SRDS1_PRTCL_SHIFT,
               SerDesPrtclMap);

  if (PcdGetBool (PcdSerdes2Enabled)) {
    LSSerDesMap (SRDS_2,
                 RCWSR_SRDS2_PRTCL_MASK,
                 RCWSR_SRDS2_PRTCL_SHIFT,
                 SerDesPrtclMap);
  }
#ifdef RCWSR_SRDS3_PRTCL_MASK
    LSSerDesMap(SRDS_3,
                RCWSR_SRDS3_PRTCL_MASK,
                RCWSR_SRDS3_PRTCL_SHIFT,
                SerDesPrtclMap);
#endif
}

BOOLEAN
IsSerDesLaneProtocolConfigured (
  IN UINT64          SerDesPrtclMap,
  IN SERDES_PROTOCOL Device

  )
{
  if (Device >= SERDES_PRTCL_COUNT || Device < NONE) {
    ASSERT (Device > NONE && Device < SERDES_PRTCL_COUNT);
    DEBUG ((DEBUG_ERROR, "Unknown SerDes lane protocol Device %d\n", Device));
  }

  return (SerDesPrtclMap & BIT (Device)) != 0 ;
}
