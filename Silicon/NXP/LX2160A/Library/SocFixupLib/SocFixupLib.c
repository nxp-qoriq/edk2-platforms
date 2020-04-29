/** @file

 Copyright 2018-2020 NXP

 SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <libfdt.h>
#include <Chassis.h>
#include <Soc.h>
#include <Library/DebugLib.h>
#include <Library/ItbParse.h>
#include <Library/SocFixupLib.h>

/**
  Fixup PCIe nodes in device tree based on running SoC's version.

**/
VOID
PcieFdtFixup (
  IN  VOID      *Fdt
  )
{
  CHAR8 *RegName, *OldStr, *NewStr;
  CONST CHAR8 *RegNames;
  INT32 NamesLen, OldStrLen, NewStrLen, RemainingStrLen;
  struct StrMap {
    CHAR8 *OldStr;
    CHAR8 *NewStr;
  } RegNamesMap[] = {
    { "csr_axi_slave", "regs" },
    { "config_axi_slave", "config" }
  };
  INT32 Off, i;
  UINTN CompatibleSize;

  Off = -1;

  if (((UINT32)SocGetSvr() & SVR_LX2160A_REV_MASK) == SVR_LX2160A_REV1)
      return;

  Off = fdt_node_offset_by_compatible (Fdt, -1, (VOID *)(PcdGetPtr (PcdPciFdtCompatible)));
  while (Off != -FDT_ERR_NOTFOUND) {
   fdt_setprop (Fdt, Off, "compatible", "fsl,ls2088a-pcie",
        AsciiStrLen ("fsl,ls2088a-pcie") + 1);

   RegNames = fdt_getprop (Fdt, Off, "reg-names", &NamesLen);
   if (!RegNames)
       continue;
   RegName = (CHAR8 *)RegNames;
   RemainingStrLen = NamesLen - (RegName - RegNames);
   i = 0;
   while ((i < ARRAY_SIZE(RegNamesMap)) && RemainingStrLen) {
       OldStr = RegNamesMap[i].OldStr;
       NewStr = RegNamesMap[i].NewStr;
       OldStrLen = AsciiStrLen (OldStr);
       NewStrLen = AsciiStrLen (NewStr);
       if (CompareMem (RegName, OldStr, OldStrLen) == 0) {
        /* first only leave required bytes for NewStr
         * and copy rest of the string after it
         */
        CopyMem (RegName + NewStrLen,
             RegName + OldStrLen,
             RemainingStrLen - OldStrLen);

        /* Now copy NewStr */
        CopyMem (RegName, NewStr, NewStrLen);
        NamesLen -= OldStrLen;
        NamesLen += NewStrLen;
        i++;
       }

       RegName = memchr (RegName, '\0', RemainingStrLen);
       if (!RegName)
        break;
       RegName += 1;

       RemainingStrLen = NamesLen - (RegName - RegNames);
   }
   fdt_setprop (Fdt, Off, "reg-names", RegNames, NamesLen);
   fdt_delprop (Fdt, Off, "apio-wins");
   fdt_delprop (Fdt, Off, "ppio-wins");
   Off = fdt_node_offset_by_compatible (Fdt, Off,
        (VOID *)(PcdGetPtr (PcdPciFdtCompatible)));
  }

  CompatibleSize = PcdGetSize (PcdPciFdtCompatible);
  PcdSetPtrS (PcdPciFdtCompatible, &CompatibleSize, "fsl,ls2088a-pcie");

  return;
}

/**
  Fixup the device tree based on running SOC's properties.

  @param[in]  Dtb   The device tree to fixup.

  @return EFI_SUCCESS       Successfully fix up the device tree
  @return EFI_DEVICE_ERROR  Could not fixup the device tree
  @return EFI_NOT_FOUND     Could not found the device tree overlay file
  @retval EFI_CRC_ERROR     Device tree overlay file is not correct.
**/
EFI_STATUS
FdtSocFixup (
  IN  VOID  *Dtb
  )
{
  INTN        NodeOffset;
  INTN        SubNodeOffset;
  UINT64      ChipSelect;
  EFI_STATUS  Status;
  INT32       FdtStatus;

  // Add uefi-runtime property in first Flexspi node and the Chip Select 0 (if not already present)
  // TODO: Add this to the boot source node, which can be determined from RCW
  NodeOffset = fdt_node_offset_by_compatible (Dtb, -1, (VOID *)(PcdGetPtr (PcdFlexSpiFdtCompatible)));
  if (NodeOffset == FDT_ERR_NOTFOUND) {
    DEBUG ((DEBUG_ERROR, "Error: can't find node %a in Dtb\n", (CHAR8* )PcdGetPtr (PcdFlexSpiFdtCompatible)));
    return EFI_NOT_FOUND;
  }

  FdtStatus = fdt_setprop_empty (Dtb, NodeOffset, "uefi-runtime");
  if (FdtStatus) {
    DEBUG ((DEBUG_ERROR, "Error: can't set uefi-runtime %a\n", fdt_strerror (FdtStatus)));
    return EFI_DEVICE_ERROR;
  }

  fdt_for_each_subnode (SubNodeOffset, Dtb, NodeOffset) {
    Status = FdtGetAddressSize (Dtb, SubNodeOffset, "reg", 0, &ChipSelect, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error: can't get ChipSelect (Status = %r)!\n", Status));
      return EFI_DEVICE_ERROR;
    }

    if (ChipSelect == 0) {
      FdtStatus = fdt_setprop_empty (Dtb, SubNodeOffset, "uefi-runtime");
      if (FdtStatus) {
        DEBUG ((DEBUG_ERROR, "Error: can't set uefi-runtime %a\n", fdt_strerror (FdtStatus)));
        return EFI_DEVICE_ERROR;
      }

      break;
    }
  }

  /* LX2160A-Rev2 PCIe fixup: LX2160A-Rev2 has different PCIe controller.
   * Detect the SoC version and fixup PCIe nodes in device tree accordingly.
   */
  PcieFdtFixup (Dtb);

  return EFI_SUCCESS;
}

/**
  Retrive the System Version from System Version Register (SVR)

  @return  0  could not read SVR register or invalid value in SVR register
  @return     SVR register value
**/
UINT32
SocGetSvr (
  )
{
  CCSR_GUR     *GurBase;
  UINT32        Svr;

  GurBase = (VOID *)PcdGet64 (PcdGutsBaseAddr);
  ASSERT (GurBase != NULL);

  Svr = GurRead ((UINTN)&GurBase->Svr);

  /* Store the SVR value */
  PcdSet32(PcdSocSvr, (UINT32)Svr);

  return Svr;
}
