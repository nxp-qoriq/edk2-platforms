/*
 * Intel ACPI Component Architecture
 * iASL Compiler/Disassembler version 20180508
 * Copyright (c) 2000 - 2018 Intel Corporation
 *
 * Template for [HEST] ACPI Table (static data table)
 * Format: [ByteLength]  FieldName : HexFieldValue
 */

[0004]                          Signature : "HEST"    [Hardware Error Source Table]
[0004]                       Table Length : 000000FC
[0001]                           Revision : 01
[0001]                           Checksum : 00
[0006]                             Oem ID : "NXP   "
[0008]                       Oem Table ID : "LX2160"
[0004]                       Oem Revision : 00000001
[0004]                    Asl Compiler ID : "INTL"
[0004]              Asl Compiler Revision : 20180508

[0004]                 Error Source Count : 00000004 [updated 4 based on total num of type start from 0]

[0002]                               Type : 0006 [PCI Express Root Port AER Structure]
[0002]                           SourceId : 0001
[0001]			       Reserved0 : 0
[0001]			       Reserved0 : 0
[0001]				    Flags : 1
[0001]                            Enabled : 01
[0004]	     NumberOfRecordsToPreAllocate : 00000001
[0004]		     MaxSectionsPerRecord : 00000001
[0004]				      Bus : 00000000
[0002]				   Device : 00000000
[0002]				 Function : 00000000
[0002]			    DeviceControl : 00000001
[0002]				Reserved1 : "00"
[0004]		   UncorrectableErrorMask : 00000000
[0004]	       UncorrectableErrorSeverity : 00000000
[0004]		     CorrectableErrorMask : 00000000
[0004] AdvancedErrorCapabilitiesAndControl : 00000000
[0004]			 RootErrorCommand : 00000007

[0002]                               Type : 0007 [PCI Express Device AER Structure Bus]
[0002]                           SourceId : 0001
[0001]			       Reserved0 : 0
[0001]			       Reserved0 : 0
[0001]				    Flags : 1
[0001]                            Enabled : 01
[0004]	     NumberOfRecordsToPreAllocate : 00000001
[0004]		     MaxSectionsPerRecord : 00000001
[0004]				      Bus : 00000000
[0002]				   Device : 00000000
[0002]				 Function : 00000000
[0002]			    DeviceControl : 00000001
[0002]			       Reserved1 : "00"
[0004]		   UncorrectableErrorMask : 00000000
[0004]	       UncorrectableErrorSeverity : 00000000
[0004]		     CorrectableErrorMask : 00000000
[0004] AdvancedErrorCapabilitiesAndControl : 00000000

[0002]                               Type : 0008 [PCI Express Bridge AER Structure]
[0002]                           SourceId : 0001
[0001]			       Reserved0 : 00
[0001]			       Reserved0 : 00
[0001]				    Flags : 1
[0001]                            Enabled : 01
[0004]	     NumberOfRecordsToPreAllocate : 00000001
[0004]		     MaxSectionsPerRecord : 00000001
[0004]				      Bus : 00000000
[0002]				   Device : 00000000
[0002]				 Function : 00000000
[0002]			    DeviceControl : 00000001
[0002]			       Reserved1 : "00"
[0004]		   UncorrectableErrorMask : 00000000
[0004]	       UncorrectableErrorSeverity : 00000000
[0004]               CorrectableErrorMask : 00000000
[0004] AdvancedErrorCapabilitiesAndControl : 00000000
[0004]    SecondaryUncorrectableErrorMask : 00000000
[0004] SecondaryUncorrectableErrorSeverity : 00000000
[0004] SecondaryAdvancedErrorCapabilitiesAndControl : 00000000

[0002]                               Type : 0009 [Generic Hardware Error Source]
[0002]                           SourceId : 0001
[0002]                    RelatedSourceId : 00
[0001]                              Flags : 00
[0001]                            Enabled : 01
[0004]       NumberOfRecordsToPreAllocate : 00000001
[0004]               MaxSectionsPerRecord : 00000001
[0004]                   MaxRawDataLength : 00001000
/*
@define at PG115 Table 5-25 Generic Address Structure (GAS)
0x02       PCI Configuration space
RegisterBitWidth      field must be zero.
RegisterBitOffset     field must be zero.
*/
[0001]                     AddressSpaceId : 00 [Generic Address Structure]
[0001]                   RegisterBitWidth : 10
[0001]                   RegisterBitOffset: 00
[0001]                         AccessSize : 03 [QWord Access:32]
[0008]                            Address : 0000000003600000
/*
@PG 852 Table 18-394 Hardware Error Notification Structure Field
Length: Total length of the structure in bytes (NA)
ConfigurationWriteEnable using in polling (NA)
*/
[0001]                               Type : 0A [Hardware Error Notification Structure]
[0001]                             Length : 1C

[0002]           ConfigurationWriteEnable : 0000//003E

[0004]                       PollInterval : 00000000
[0004]                             Vector : 00000097
[0004]      SwitchToPollingThresholdValue : 00000000
[0004]     SwitchToPollingThresholdWindow : 00000000
[0004]                ErrorThresholdValue : 00000000
[0004]               ErrorThresholdWindow : 00000000
[0004]		       ErrorStatusBlockLength : 00001000
