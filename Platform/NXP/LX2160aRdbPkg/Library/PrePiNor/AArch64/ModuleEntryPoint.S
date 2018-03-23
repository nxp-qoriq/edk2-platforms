//  @ModuleEntryPoint.S
//
//  Copyright 2018 NXP
//
//  This program and the accompanying materials
//  are licensed and made available under the terms and conditions of the BSD License
//  which accompanies this distribution.  The full text of the license may be found at
//  http://opensource.org/licenses/bsd-license.php
//
//  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
//  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
//

#include <AutoGen.h>
#include <AsmMacroIoLibV8.h>
#include <Library/PcdLib.h>
#include <Library/PlatformLib.h>

#define FSL_LSCH3_SVR				(FixedPcdGet64 (PcdGutsBaseAddr) + 0xA4)

.text
.align 3

GCC_ASM_EXPORT(_ModuleEntryPoint)

StartupAddr:        .8byte ASM_PFX(CEntryPoint)

SvrDevId:
	ldr	x1, =FSL_LSCH3_SVR
	ldr	w0, [x1]
	lsr	w0, w0, #16
	ret

ASM_PFX(_ModuleEntryPoint):

/** NXP_TZPC_BP147 configuration */
	/* Set Non Secure access for all devices protected via NXP_TZPC */
	ldr	x1, =NXP_TZPCDECPROT_0_SET_BASE /* Decode Protection-0 Set Reg */
	orr	w0, w0, #1 << 3 /* DCFG_RESET is accessible from NS world */
	str	w0, [x1]

	isb
	dsb	sy
/* NXP_TZPC_BP147 configuration ends here*/

/** NXP_TZASC_400 configuration */
	/* Set NXP_TZASC so that:
	 * a. We use only Region0 whose global secure write/read is EN
	 * b. We use only Region0 whose NSAID write/read is EN
	 *
	 * NOTE: As per the CCSR map doc, NXP_TZASC 3 and NXP_TZASC 4 are just
	 * 	 placeholders.
	 */

	bl	SvrDevId		/* get high 16 bits of SVR */
	/*
	 * Skip NXP_TZASC related operations for non LX2160 and its personlaities
	 */
	ldr	x1, =SVR_DEV_LX2160
	cmp	x0, x1
	bne	1f

/** NXP_TZASC_1 configuration */
	ldr	x1, =NXP_TZASC_GATE_KEEPER(0)
	ldr	w0, [x1]		/* Filter 0 Gate Keeper Register */
	orr	w0, w0, #1 << 0		/* Set open_request for Filter 0 */
	str	w0, [x1]

	ldr	x1, =NXP_TZASC_REGION_ATTRIBUTES_0(0)
	ldr	w0, [x1]		/* Region-0 Attributes Register */
	orr	w0, w0, #1 << 31	/* Set Sec global write en, Bit[31] */
	orr	w0, w0, #1 << 30	/* Set Sec global read en, Bit[30] */
	str	w0, [x1]

	ldr	x1, =NXP_TZASC_REGION_ID_ACCESS_0(0)
	ldr	w0, [x1]		/* Region-0 Access Register */
	mov	w0, #0xFFFFFFFF		/* Set nsaid_wr_en and nsaid_rd_en */
	str	w0, [x1]
/* NXP_TZASC_1 configuration ends here */

/** NXP_TZASC_2 configuration */
	ldr	x1, =NXP_TZASC_GATE_KEEPER(1)
	ldr	w0, [x1]		/* Filter 0 Gate Keeper Register */
	orr	w0, w0, #1 << 0		/* Set open_request for Filter 0 */
	str	w0, [x1]

	ldr	x1, =NXP_TZASC_REGION_ATTRIBUTES_0(1)
	ldr	w0, [x1]		/* Region-1 Attributes Register */
	orr	w0, w0, #1 << 31	/* Set Sec global write en, Bit[31] */
	orr	w0, w0, #1 << 30	/* Set Sec global read en, Bit[30] */
	str	w0, [x1]

	ldr	x1, =NXP_TZASC_REGION_ID_ACCESS_0(1)
	ldr	w0, [x1]		/* Region-1 Attributes Register */
	mov	w0, #0xFFFFFFFF		/* Set nsaid_wr_en and nsaid_rd_en */
	str	w0, [x1]
/* NXP_TZASC_2 configuration ends here */

/** NXP_TZASC_3 configuration */
	ldr	x1, =NXP_TZASC_GATE_KEEPER(2)
	ldr	w0, [x1]		/* Filter 0 Gate Keeper Register */
	orr	w0, w0, #1 << 0		/* Set open_request for Filter 0 */
	str	w0, [x1]

	ldr	x1, =NXP_TZASC_REGION_ATTRIBUTES_0(2)
	ldr	w0, [x1]		/* Region-1 Attributes Register */
	orr	w0, w0, #1 << 31	/* Set Sec global write en, Bit[31] */
	orr	w0, w0, #1 << 30	/* Set Sec global read en, Bit[30] */
	str	w0, [x1]

	ldr	x1, =NXP_TZASC_REGION_ID_ACCESS_0(2)
	ldr	w0, [x1]		/* Region-1 Attributes Register */
	mov	w0, #0xFFFFFFFF		/* Set nsaid_wr_en and nsaid_rd_en */
	str	w0, [x1]
/* NXP_TZASC_3 configuration ends here */

/** NXP_TZASC_4 configuration */
	ldr	x1, =NXP_TZASC_GATE_KEEPER(3)
	ldr	w0, [x1]		/* Filter 0 Gate Keeper Register */
	orr	w0, w0, #1 << 0		/* Set open_request for Filter 0 */
	str	w0, [x1]

	ldr	x1, =NXP_TZASC_REGION_ATTRIBUTES_0(3)
	ldr	w0, [x1]		/* Region-1 Attributes Register */
	orr	w0, w0, #1 << 31	/* Set Sec global write en, Bit[31] */
	orr	w0, w0, #1 << 30	/* Set Sec global read en, Bit[30] */
	str	w0, [x1]

	ldr	x1, =NXP_TZASC_REGION_ID_ACCESS_0(3)
	ldr	w0, [x1]		/* Region-1 Attributes Register */
	mov	w0, #0xFFFFFFFF		/* Set nsaid_wr_en and nsaid_rd_en */
	str	w0, [x1]
/* NXP_TZASC_3 configuration ends here */
	isb
	dsb	sy
/* NXP_TZASC_400 configuration ends here*/

1:
  MOV32 (x0, FixedPcdGet32(PcdOcramStackBase))
  mov sp, x0
  MOV64 (x0, FixedPcdGet64(PcdFdBaseAddress))
  MOV32 (x1, FixedPcdGet32(PcdFdNorBaseAddress))
  MOV32 (x5, FixedPcdGet32(PcdPiFdSize))
  add x1, x1, x5
  MOV32 (x2, FixedPcdGet32(PcdFdSize))
  ldr x4, StartupAddr
  blr x4
