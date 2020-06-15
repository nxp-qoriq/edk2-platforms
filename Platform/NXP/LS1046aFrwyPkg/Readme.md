Capsule Update Steps:

Note: Correctly Update BaseAddress and Length in SystemFirmwareUpdateConfig.ini
      as incorrect value results in system being unresponsive. for e.g.
      BaseAddress   = 0x4010A6B9    # Base address offset on flash
      Length        = 0x00200000    # Length

      BaseAddress = Flash Address + fip Offset + Non-Trusted Firmware BL33 offset.
                  = 0x40000000    + 0x100000   + 0xA6B9
                  = 0x4010A6B9

OpenSSL Patch for CryptoPkg
$ cd edk2
$ git submodule init
$ git submodule update

Build and Boot Firmware Image
• Update target with new firmware image
• Boot target to UEFI Shell

Verify Capsule Structures:
• Copy CapsuleApp.efi(edk2/Build/LS1046aFrwyPkg/RELEASE_GCC5/AARCH64/CapsuleApp.efi) to a USB/SD drive on Board.
• Run CapsuleApp.efi with no parameters to see the help information
• Run CapsuleApp.efi -P to view the Firmware Management Protocol details.
  The details should match the SystemFirmwareDescriptorTable.aslc
• Run CapsuleApp.efi -E to view the ESRT details. The FwType should be 0x1 (SystemFirmware),
  and the FwVersion should be the CURRENT_FIRMWARE_VERSION value from the SystemFirmwareDescriptorTable.aslc file.

Build System Firmware Update Capsule
• Update SystemFirmwareDescriptorTable.aslc file to a higher version by updating the CURRENT_FIRMWARE_VERSION.
  eg. (FixedPcdGet32 (PcdFirmwareRevision)+1)
• Build firmware image again.

Verify System Firmware Update Capsule
• Copy System Firmware Update Capsule Image with higher version to a USB/SD drive.
  (edk2/Build/LS1046aFrwyPkg/RELEASE_GCC5/FV/LS1046AFRWYFIRMWAREUPDATECAPSULEFMPPKCS7.Cap)
• Run CapsuleApp.efi -D <CapsuleImage> to dump capsule image header information.
  The UpdateImageTypeId value is the same as the ESRT FwClass value,
  and is also the same as the Firmware Management Protocol ImageTypeId GUID value.
• Run CapsuleApp.efi <CapsuleImage> to load and process the system firmware update capsule.
  (This step will take time to write the firmware on flash)
• Run reset command.
• Boot target to UEFI shell.
• Run CapsuleApp.efi -P to view the Firmware Management Protocol details.
  The details should match the updated version information in the SystemFirmwareDescriptorTable.aslc file.
• Run CapsuleApp.efi -E to view the ESRT details.
  The details should match the updated version information in the SystemFirmwareDescriptorTable.aslc file.
