/** @file
  This driver installs SMBIOS information for NXP LX2160ARDB platforms

  Copyright (c) 2015, ARM Limited. All rights reserved.
  Copyright 2019-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SmbiosPlatformDxe.h"


#pragma pack()

/**
 * BIOS information (section 7.1)
 */
STATIC ARM_TYPE0 mArmDefaultType0 = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_BIOS_INFORMATION, /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE0),      /* UINT8 Length */
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,     /* SMBIOS_TABLE_STRING       Vendor */
    2,     /* SMBIOS_TABLE_STRING       BiosVersion */
    0xE800,/* UINT16                    BiosSegment */
    3,     /* SMBIOS_TABLE_STRING       BiosReleaseDate */
    0,     /* UINT8                     BiosSize */
    {
      0,0,0,0,0,0,
      1, /* PCI supported */
      0,
      0, /* PNP supported */
      0,
      1, /* BIOS upgradable */
      0, 0, 0,
      0, /* Boot from CD not supported */
      1, /* selectable boot */
    },   /* MISC_BIOS_CHARACTERISTICS BiosCharacteristics */
    {    /* BIOSCharacteristicsExtensionBytes[2] */
      0x3,
      0xC,
    },
    0,     /* UINT8                     SystemBiosMajorRelease */
    0,     /* UINT8                     SystemBiosMinorRelease */
    0xFF,  /* UINT8                     EmbeddedControllerFirmwareMajorRelease */
    0xFF   /* UINT8                     EmbeddedControllerFirmwareMinorRelease */
  },
  /* Text strings (unformatted area) */
  TYPE0_STRINGS
};

/**
 * System information (section 7.2)
 */
STATIC CONST ARM_TYPE1 mArmDefaultType1 = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_SYSTEM_INFORMATION,
      sizeof (SMBIOS_TABLE_TYPE1),
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,     /* Manufacturer */
    2,     /* Product Name */
    3,     /* Version */
    4,     /* Serial */
    { 0x8a95d198, 0x7f46, 0x11e5, { 0xbf,0x8b,0x08,0x00,0x27,0x04,0xd4,0x8e }},    /* UUID */
    6,     /* Wakeup type */
    0,     /* SKU */
    0,     /* Family */
  },
  /* Text strings (unformatted)*/
  TYPE1_STRINGS
};

/**
 * System Enclosure or Chassis info
 */
STATIC CONST ARM_TYPE3 mArmDefaultType3 = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE, /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE3),      /* UINT8 Length */
      SMBIOS_HANDLE_CHASSIS,
    },
    1,   /* Manufacturer */
    9,   /* enclosure type (laptop) */
    2,   /* version */
    3,   /* serial */
    0,   /* asset tag */
    ChassisStateUnknown,   /* boot chassis state */
    ChassisStateSafe,      /* power supply state */
    ChassisStateSafe,      /* thermal state */
    ChassisSecurityStatusNone,   /* security state */
    {0,0,0,0,}, /* OEM defined */
    1,          /* 1U height */
    1,          /* number of power cords */
    0,          /* no contained elements */
  },
  TYPE3_STRINGS
};

/**
 * Structure defines attributes of the Processor
 */
STATIC CONST ARM_TYPE4 mArmDefaultType4_a72 = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE4),           /* UINT8 Length */
      SMBIOS_HANDLE_A72_CLUSTER,
    },
    1,                                       /* socket type */
    3,                                       /* processor type CPU */
    ProcessorFamilyIndicatorFamily2,         /* processor family, acquire from field2 */
    2,                                       /* manufactuer */
    {{0,},{0.}},                             /* processor id */
    5,                                       /* version */
    {0,0,0,0,0,1},                           /* voltage */
    100,                                     /* external clock */
    2200,                                    /* max speed */
    2000,                                    /* current speed */
    0x41,                                    /* status */
    ProcessorUpgradeOther,
    SMBIOS_HANDLE_A57_L1I,                   /* l1 cache handle */
    SMBIOS_HANDLE_A57_L2,                    /* l2 cache handle */
    0xFFFF,                                  /* l3 cache handle */
    0,                                       /* serial not set */
    0,                                       /* asset not set */
    8,                                       /* part number */
    16,                                      /* core count in socket */
    16,                                      /* enabled core count in socket */
    0,                                       /* threads per socket */
    0xEC,                                    /* processor characteristics */
    ProcessorFamilyARM,                      /* ARM core */
  },
  TYPE4_STRINGS
};


/**
 * CPU cache device in the system. One structure is specified for each such
 * device, whether the device is internal to or external to the CPU module.
 * Cache modules can be associated with a processor structure
 * in one or two ways  depending on the SMBIOS version.
 */
STATIC CONST ARM_TYPE7 mArmDefaultType7_a57_l1i = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_CACHE_INFORMATION,   /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE7),         /* UINT8 Length */
      SMBIOS_HANDLE_A57_L1I,
    },
    1,
    0x280,                                 /* L1 enabled,
                                             varies with Memory Address */
    0x0030,                                /* 48k i cache max */
    0x0030,                                /* 48k installed */
    {0,1},                                 /* SRAM type */
    {0,1},                                 /* SRAM type */
    0,                                     /* unkown speed */
    CacheErrorParity,                      /* parity checking */
    CacheTypeInstruction, /* instruction cache */
    CacheAssociativityOther, /* three way */
    0x400,                                /* 1 MB max L2  cache */
    0x400,                                /* 1 MB installed L2 Cache */
  },
  TYPE7_STRINGS
};

/**
 * Memory array
 */
STATIC CONST ARM_TYPE16 mArmDefaultType16 = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE16),          /* UINT8 Length */
      SMBIOS_HANDLE_MEMORY,
    },
    MemoryArrayLocationSystemBoard, /* on motherboard */
    MemoryArrayUseSystemMemory,     /* system RAM */
    MemoryErrorCorrectionSingleBitEcc, /* single bit  ECC correction */
    0x2000000, /* 32GB */
    0xFFFE,   /* No error information structure */
    0x1,      /* soldered memory */
  },
  TYPE16_STRINGS
};

/**
 * Memory device
 */
STATIC CONST ARM_TYPE17 mArmDefaultType17 = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_MEMORY_DEVICE, /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE17),  /* UINT8 Length */
      SMBIOS_HANDLE_DIMM,
    },
    SMBIOS_HANDLE_MEMORY, /* array to which this module belongs */
    0xFFFE,               /* no errors */
    72, /* single DIMM, no ECC is 64bits (for ecc this would be 72) */
    64, /* data width of this device (64-bits) */
    0x7FFF, /* for size 32GB -1MB or greater*/
    0x09,   /* DIMM */
    0,      /* not part of a set */
    1,      /* right side of board */
    2,      /* bank 0 */
/*  MemoryTypeLpddr3, */ /* LP DDR3, isn't defined yet */
    MemoryTypeDdr4,                  /* LP DDR4 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, /* unbuffered */
    2900,                            /* 2900Mhz DDR */
    0, /* varies between diffrent production runs */
    0, /* serial */
    0, /* asset tag */
    0, /* part number */
    0, /* rank */
    0,                               /* Extended Size */
    2900,                            /* Configured speed in MT/s */
    0,                               /* Minimum operating voltage */
    0,                               /* Maximum operating voltage  */
    0,                               /* Configured voltage */
    3,                               /* Memory Technology */
    {{                    // MemoryOperatingModeCapability
      0,  // Reserved                        :1;
      0,  // Other                           :1;
      0,  // Unknown                         :1;
      1,  // VolatileMemory                  :1;
      0,  // ByteAccessiblePersistentMemory  :1;
      0,  // BlockAccessiblePersistentMemory :1;
      0   // Reserved                        :10;
    }},
    0,                    // FirwareVersion
    0,                    // ModuleManufacturerID (unknown)
    0,                    // ModuleProductID (unknown)
    0,                    // MemorySubsystemControllerManufacturerID (unknown)
    0,                    // MemorySubsystemControllerProductID (unknown)
    0,                    // NonVolatileSize
    0xFFFFFFFFFFFFFFFFULL,// VolatileSize // initialized at runtime, refer to PhyMemArrayInfoUpdateSmbiosType16()
    0,                    // CacheSize
    0,                    // LogicalSize (since MemoryType is not MemoryTypeLogicalNonVolatileDevice)
    0,                    // ExtendedSpeed,
    0                     // ExtendedConfiguredMemorySpeed
  },
  TYPE17_STRINGS
};

/**
 * Memory array mapped address, this structure
 * is overridden by InstallMemoryStructure
 */
STATIC CONST ARM_TYPE19 mArmDefaultType19 = {
  {
    {  /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS, /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE19),                /* UINT8 Length */
      SMBIOS_HANDLE_PI_RESERVED,
    },
    0xFFFFFFFF, /* invalid, look at extended addr field */
    0xFFFFFFFF,
    SMBIOS_HANDLE_DIMM, /* handle */
    1,
    0x000000000,        /* starting addr of first 32GB */
    0x7FFFFFFFE,        /* ending addr of first 32GB */
  },
  TYPE19_STRINGS
};

/**
 * System boot info
 */
STATIC CONST ARM_TYPE32 mArmDefaultType32 = {
  {
    { /* SMBIOS_STRUCTURE Hdr */
      EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION, /* UINT8 Type */
      sizeof (SMBIOS_TABLE_TYPE32),            /* UINT8 Length */
      SMBIOS_HANDLE_PI_RESERVED,
    },
    {0,0,0,0,0,0},                             /* reserved */
    BootInformationStatusNoError,
  },
  TYPE32_STRINGS
};

STATIC CONST ARM_TYPE9 mArmDefaultType9_1 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    1,
    SlotTypePciExpressGen3X4,
    SlotDataBusWidth4X,
    SlotUsageAvailable,
    SlotLengthShort,
    0,                 // SlotId
    {1},               // unknown
    {1,0,0},           // PME and SMBUS
    0x0,               // Segment
    0x0,               // Bus
    0x0,               // DevFunc
  },
  TYPE9_STRING
};

STATIC CONST ARM_TYPE9 mArmDefaultType9_2 = {
  {
    { // SMBIOS_STRUCTURE Hdr
      EFI_SMBIOS_TYPE_SYSTEM_SLOTS, // UINT8 Type
      sizeof (SMBIOS_TABLE_TYPE9),  // UINT8 Length
      SMBIOS_HANDLE_PI_RESERVED,
    },
    2,
    SlotTypePciExpressGen3X8,
    SlotDataBusWidth8X,
    SlotUsageAvailable,
    SlotLengthShort,
    0,                // SlotId
    {1},              // unknown
    {1,0,0},          // PME and SMBUS
    0x1,              // Segment
    0x0,              // Bus
    0x0,              // DevFunc
  },
  TYPE9_STRING
};

STATIC CONST VOID *LX2Tables[] =
{
    &mArmDefaultType0,
    &mArmDefaultType1,
    &mArmDefaultType3,
    &mArmDefaultType4_a72,
    &mArmDefaultType7_a57_l1i,/* Cache layout is the same on the A72 vs A57 */
    &mArmDefaultType16,
    &mArmDefaultType17,
    &mArmDefaultType19,
    &mArmDefaultType32,
    &mArmDefaultType9_1,
    &mArmDefaultType9_2,
     NULL
};
/**
   Installs a memory descriptor (type19) for the given address range

   @param  Smbios               SMBIOS protocol

**/
EFI_STATUS
InstallMemoryStructure (
  IN EFI_SMBIOS_PROTOCOL       *Smbios,
  IN UINT64                    StartingAddress,
  IN UINT64                    RegionLength
  )
{
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  ARM_TYPE19                MemoryDescriptor;
  EFI_STATUS                Status;

  Status = EFI_SUCCESS;

  CopyMem (&MemoryDescriptor, &mArmDefaultType19, sizeof (ARM_TYPE19));

  MemoryDescriptor.Base.ExtendedStartingAddress = StartingAddress;
  MemoryDescriptor.Base.ExtendedEndingAddress = StartingAddress + RegionLength;
  SmbiosHandle = MemoryDescriptor.Base.Hdr.Handle;

  Status = Smbios->Add (
    Smbios,
    NULL,
    &SmbiosHandle,
    (EFI_SMBIOS_TABLE_HEADER*) &MemoryDescriptor
    );
  return Status;
}

/**
 * Install supported SMBIOS structructures

   @param  Smbios               SMBIOS protocol
   @parm   DefaultTables        Table containg supported struct
**/
EFI_STATUS
InstallStructures (
  IN EFI_SMBIOS_PROTOCOL       *Smbios,
  IN CONST VOID                *DefaultTables[]
   )
{

  EFI_STATUS                Status;
  EFI_SMBIOS_HANDLE         SmbiosHandle;

  INT32 TableEntry;

  Status = EFI_SUCCESS;

  for (TableEntry = 0; DefaultTables [TableEntry] != NULL; TableEntry++)  {
    SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER*)DefaultTables[TableEntry])->Handle;
    Status = Smbios->Add (
      Smbios,
      NULL,
      &SmbiosHandle,
      (EFI_SMBIOS_TABLE_HEADER*) DefaultTables[TableEntry]
      );

    if (EFI_ERROR (Status))  {
      break;
    }
  }

  return Status;
}


/**
   Install all structures from the DefaultTables structure

   @param  Smbios               SMBIOS protocol

**/
EFI_STATUS
InstallAllStructures (
   IN EFI_SMBIOS_PROTOCOL       *Smbios
   )
{
  EFI_STATUS                Status;

  Status = EFI_SUCCESS;

  /* Add all LX2160ARDB table entries */
  Status = InstallStructures (Smbios, LX2Tables);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
   Installs SMBIOS information for ARM platforms

   @param ImageHandle     Module's image handle
   @param SystemTable     Pointer of EFI_SYSTEM_TABLE

   @retval EFI_SUCCESS    Smbios data successfully installed
   @retval Other          Smbios data was not installed

**/
EFI_STATUS
EFIAPI
SmbiosTablePublishEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_SMBIOS_PROTOCOL       *Smbios;

  /* Find the SMBIOS protocol */
  Status = gBS->LocateProtocol (
    &gEfiSmbiosProtocolGuid,
    NULL,
    (VOID**)&Smbios
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallAllStructures (Smbios);

  return Status;
}
