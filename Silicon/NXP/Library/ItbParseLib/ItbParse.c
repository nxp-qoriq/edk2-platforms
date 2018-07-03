/** ItbParse.c

  Copyright 2017 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ItbParse.h>
#include <Library/PrintLib.h>
#include <libfdt.h>


EFI_STATUS
FitCheckHeader (
  EFI_PHYSICAL_ADDRESS  FitImage
 )
{
  if (fdt_check_header((VOID*) FitImage)) {
    DEBUG ((DEBUG_ERROR, "bad FIT header\n"));
    return EFI_UNSUPPORTED;
  }

  // mandatory node 'description' property
  if (fdt_getprop ((VOID*)FitImage, 0, FIT_DESC_PROP, NULL) == NULL) {
    DEBUG ((DEBUG_ERROR, "Wrong FIT format: no description\n"));
    return EFI_UNSUPPORTED;
  }

  // mandatory subimages parent 'images' node
  if (fdt_path_offset ((VOID*)FitImage, FIT_IMAGES_PATH) < 0) {
    DEBUG ((DEBUG_ERROR, "Wrong FIT format: no images parent node\n"));
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
FitGetConfNode (
  EFI_PHYSICAL_ADDRESS  FitImage,
  VOID*  ConfigPtr,
  INT32*  NodeOffset
 )
{
  INT32 Noffset;
  INT32 ConfsOffset;
  INT32 Length;
  char* ConfigName;

  ConfsOffset = fdt_path_offset ((VOID*)FitImage, FIT_CONFS_PATH);
  if (ConfsOffset < 0) {
    DEBUG ((DEBUG_ERROR, "Can't find configurations parent node '%s' (%s)\n",
          FIT_CONFS_PATH, fdt_strerror (ConfsOffset)));
    return EFI_UNSUPPORTED;
  }

  ConfigName = (char*)ConfigPtr;

  if (ConfigName && *ConfigName == '\0')
    ConfigName = NULL;
  if (ConfigName == NULL) {
      /* get configuration unit name from the default property
       * */
    DEBUG ((DEBUG_ERROR, "No configuration specified, trying default...\n"));
    ConfigName = (CHAR8 *)fdt_getprop ((VOID*)FitImage, ConfsOffset,
        FIT_DEFAULT_PROP, &Length);
  }
  Noffset = fdt_subnode_offset ((VOID*)FitImage, ConfsOffset, ConfigName);
  if (Noffset < 0) {
     DEBUG ((DEBUG_ERROR,
     "Can't get node offset for configuration unit name: '%s' (%s)\n",
     ConfigName, fdt_strerror (Noffset)));
     return EFI_UNSUPPORTED;
  }

  *NodeOffset = Noffset;
  return EFI_SUCCESS;
}

EFI_STATUS
FitGetNodeFromConf (
  EFI_PHYSICAL_ADDRESS  FitImage,
  INT32 CfgNodeOffset,
  CHAR8* ConfPropName,
  INT32* NodeOffset
 )
{
  INT32 Noffset;
  INT32 Length;
  INT32 ImgNoffset;
  CHAR8* PropName;

  PropName = (char *)fdt_getprop ((VOID*)FitImage, CfgNodeOffset,
                                         ConfPropName, &Length);

  if (PropName == NULL) {
    return EFI_UNSUPPORTED;
  }

  ImgNoffset = fdt_path_offset ((VOID*)FitImage, FIT_IMAGES_PATH);
  if (ImgNoffset < 0) {
    return EFI_UNSUPPORTED;
  }
  Noffset = fdt_subnode_offset ((VOID*)FitImage, ImgNoffset, PropName);
  if (Noffset < 0) {
    return EFI_NOT_FOUND;
  }

  *NodeOffset = Noffset;
  return EFI_SUCCESS;
}

EFI_STATUS
FitGetNodeData (
  EFI_PHYSICAL_ADDRESS  FitImage,
  INT32                  NodeOffset,
  EFI_PHYSICAL_ADDRESS* Addr,
  INT32*                Size
 )
{
  VOID *Data;

  Data = (VOID*)fdt_getprop ((VOID*)FitImage, NodeOffset, FIT_IMAGE_DATA, Size);
  if (Data == NULL) {
    return EFI_UNSUPPORTED;
  }
  *Addr = (EFI_PHYSICAL_ADDRESS)Data;
  return EFI_SUCCESS;
}

/*
 * Retrieves the node offset of a given image within a FIT image
 *
 * @param[in] FitImageAddr  Address of the FIT image
 * @param[in] ImageUnitName Target image name
 * @param[out] NodeOffset   Pointer to area where the node offset is
 *                          to be returned
 *
 * @retval EFI_SUCCESS, on success
 * @retval error code, on failure
 */


EFI_STATUS
FitImageGetNode (
    EFI_PHYSICAL_ADDRESS FitImageAddr,
    CHAR8 *ImageUnitName,
    INT32 *NodeOffset
    )
{
  INT32 SubNodeOffset;
  INT32 ImagesNodeOffset;
  VOID *FitImage;

  FitImage = (VOID *)FitImageAddr;

  ImagesNodeOffset = fdt_path_offset (FitImage, FIT_IMAGES_PATH);
  if (ImagesNodeOffset < 0) {
    DEBUG ((DEBUG_ERROR, "Can't find images parent node '%a' (%a)\n",
        FIT_IMAGES_PATH, fdt_strerror (ImagesNodeOffset)));
    return EFI_UNSUPPORTED;
  }

  SubNodeOffset = fdt_subnode_offset (FitImage, ImagesNodeOffset, ImageUnitName);
  if (SubNodeOffset < 0) {
    DEBUG ((DEBUG_ERROR,"Can't get node offset for image unit name: '%a' (%a)\n",
        ImageUnitName, fdt_strerror (SubNodeOffset)));
  }

  *NodeOffset = SubNodeOffset;
  return EFI_SUCCESS;
}


