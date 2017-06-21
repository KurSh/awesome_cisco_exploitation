/* $Id: vip_xdi.h,v 3.1.60.2 1996/09/11 15:52:27 snyder Exp $
 * $Source: /release/112/cvs/Xsys/src-vip/vip_xdi.h,v $
 *------------------------------------------------------------------
 * vip_xdi.h - 1F MC68840 based PCI port adaptor support
 *                  (VIP specific  FDDI SMT definitions)
 *
 * August 1995, Farhad Sunavala
 *
 * Copyright (c) 1995-1996 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: vip_xdi.h,v $
 * Revision 3.1.60.2  1996/09/11  15:52:27  snyder
 * CSCdi68773:  more things const
 *              536 out of data section
 *              100 out of image
 * Branch: California_branch
 *
 * Revision 3.1.60.1  1996/03/21  23:52:41  gstovall
 * Branch: California_branch
 * The ELC_branch now knows the joy of California dreaming.
 *
 * Revision 3.1.4.3  1996/02/26  04:39:30  fsunaval
 * CSCdi49791:  Incorporate code-review comments for Fddi port adapter
 * Branch: ELC_branch
 *
 * Revision 3.1.4.2  1996/01/25  12:10:45  fsunaval
 * Branch: ELC_branch
 * 1.Predator FDDI support.
 * 2.Patch CSCdi46358 (PA management interrupt) changes into ELC_branch.
 *
 * Revision 3.1.4.1  1995/12/23  04:42:09  fsunaval
 * Branch: ELC_branch
 * 1.First pass at FDDI port adapter.
 *
 * Revision 3.1  1995/11/09  13:30:05  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/08/24  19:55:24  fsunaval
 * Placeholder files for new development.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

#ifndef __VIP_XDI_H__
#define __VIP_XDI_H__

/*
@(#) if7_smtdefs.h if7_v2.2.1:cset.0055:4:4 Mon Mar  1 16:49:14 1993 Copyright 1990-1993 XLNT Designs, Inc., ALL RIGHTS RESERVED
*/
/*********************************************************************
	SMT Header File
	
	SMT Definitions Header File

	THIS FILE IS TO BE PORTED BY THE IMPLEMENTOR.
	
	File:			if7_smtdefs.h
	Created:		12/01/89

	Version:		if7_v2.2.1		Mon Mar  1 16:49:14 1993
	Last Modified:	cset.0055		03/01/93
	
	Copyright 1990-1993 XLNT Designs, Inc., ALL RIGHTS RESERVED

	ALL RIGHTS RESERVED. THE SOURCE FILES DISTRIBUTED WITH THE
	XLNT Manager(tm) SYSTEM ARE THE SOLE PROPERTY OF XLNT Designs,
	Inc. AND CONTAIN CONFIDENTIAL AND PROPRIETARY INFORMATION.
	REPRODUCTION OR DUPLICATION BY ANY MEANS OF ANY PORTION OF
	THIS SOFTWARE WITHOUT PRIOR WRITTEN CONSENT OF XLNT Designs,
	Inc. IS EXPRESSLY FORBIDDEN.
	
	This header file is used by all SMT modules. This file contains
	definitions for all SMT system dependent values like number of
	PHYs and MACs.

	Modification History:
	*** SMT 7.2 ***
		Based on 6.2.3 multistation code.
	930217.002	LJP
		Conditionally compile Void_t and Void_p typedefs based on
		__STDC__.
	930224.005	LJP
		Added MCONST constant qualifier macro.
*********************************************************************/

/*********************************************************************
	Type Definitions
*********************************************************************/

/*
*	Integer Definitions
*	The target environment must be able to support 16- and 32-bit
*	signed and unsigned integer values.
*/
typedef short           Int16;
typedef unsigned short  uInt16;

typedef long            Int32;
typedef unsigned long   uInt32;


/*
*	Character Definitions
*	The C type "char" is assumed to be a signed 8-bit value.
*/
typedef unsigned char   uChar;


/*
*	Boolean Definitions
*	A Boolean variable may take on one of two values. The FDDI SMT
*	uses the term "flag" for Boolean variables. A flag may either be
*	set or cleared. The constant SET is equivalent to a logical
*	TRUE and the constant CLEAR is a logical FALSE.
*/
typedef int             Flag;


/*
*	Void Pointer Defintion
*	This macro defines a generic pointer type. In ANSI C and most
*	newer compilers for traditional C, the type void * is used to
*	represent a generic pointer. In older compilers, char * can
*	be used. Use of char * may cause program verifiers, like lint,
*	and some compilers to complain about possible pointer alignment
*	problems.
*/
#ifdef	__STDC__
typedef void            Void_t;
typedef void           *Void_p;
#else
typedef char            Void_t;
typedef char           *Void_p;
#endif


union Splice {
        uInt32  dword;
        struct {
            uInt16      word1;
            uInt16      word0;
        } w;
        struct {
            uChar       byte3;
            uChar       byte2;
            uChar       byte1;
            uChar       byte0;
        } b;
};


/*********************************************************************
	System Definitions
*********************************************************************/

/*
*	Line State Values
*	Each line state must correspond to an individual bit in an
*	integer value.
*/
#define	Noise_Line_State			0x01
#define	Active_Line_State			0x02
#define	Idle_Line_State				0x08
#define	Quiet_Line_State			0x10
#define	Master_Line_State			0x20
#define	Halt_Line_State				0x40
#define	Super_Idle_Line_State		0x80

#define	Transmit_PHY_Data_Request	0x200

/*
*	E, A, and C indicator bit positions.
*	The frame interface routines must provide the E, A, and C
*	indicator values with all frames received. The indicator
*	values will be passed to the frame services process as a
*	bit string. These defined values determine the bit positions
*	of each indicator.
*/
#define	E_Bit_Position	0x80
#define	A_Bit_Position	0x40
#define	C_Bit_Position	0x20


/*
*	Station Limitations
*	These values determine the system limits for various elements
*	within the station.
*/

#define	MAX_MAC_COUNT		1
#define	MAX_PATH_COUNT		2
#define	MAX_PORT_COUNT		2

/* Multistation */
#define	MAX_STATION_COUNT	4

/*
*	These values specify the number of vendor notifications in the MIB
*	for each object.
*/
#define	VENDOR_SMT_EC_COUNT		0
#define	VENDOR_MAC_EC_COUNT		0
#define	VENDOR_PATH_EC_COUNT	0
#define	VENDOR_PORT_EC_COUNT	0

/*
*       Cisco SMT Definitions
*/
#define CSC_STATION_STRING      "CS7500-R4000"
#define CON_CSC_DEFAULT         (Con_rejectM_M)
#define MAX_TREQ                0xFFFF3CB0      /* 4ms */


/*********************************************************************
*	System Configurations
*	These values describe the target environment for the SMT software.
*********************************************************************/

/*
*	The bit format used by the processor must be known.
*	Either BIG_ENDIAN or LITTLE_ENDIAN must be defined.
*	Do not define both.
*/
/* #define	LITTLE_ENDIAN */
#define	BIG_ENDIAN

/*
*	Padding sizes.
*	The padding size moves the first character of the frame buffer
*	into the last byte of a word. This first character of the frame
*	buffer is used by the FC field which is usually handled
*	separately from the rest of the frame. By forcing this type of
*	alignment, the remaining data in the frame starts on a word
*	boundary, thus making data transfers easier and more efficient.
*/
#define	FC_PAD_SIZE	3


/*********************************************************************
*	Operational Limitations
*	These value determine the limits of the SMT software.
*********************************************************************/

/*
*	CSP Signal queue limitations
*/
#define	MAX_SIG_QUEUE_SIZE	512

/*
*	FBM Action queue limitations
*/
#define	MAX_FBM_QUEUE_SIZE	32

/*
*	Timer list limitations
*/
#define	MAX_TIMER_LIST_SIZE	16
#define	MIN_USECS			1

/*
* 930224.005	LJP
* The MCONST macro allows an implementer to define a qualifier keyword or
* compiler directive for indicating constant data. The const keyword is
* used as a default if ANSI C is in use. Otherwise, nothing is defined
* and the core code will use an empty macro definition for MCONST.
*/
#ifdef __STDC__
#define MCONST const
#endif
/*
@(#) if7_cspmacro.h if7_v2.2.1:cset.0009:2:2 Mon Mar  1 16:49:14 1993 Copyright 1990-1993 XLNT Designs, Inc., ALL RIGHTS RESERVED
*/
/*********************************************************************
	Connection Services Process Header File
	
	CSP Macro Definitions

	THIS FILE IS TO BE PORTED BY THE IMPLEMENTOR.
	
	File:			if7_cspmacro.h
	Created:		12/01/89

	Version:		if7_v2.2.1		Mon Mar  1 16:49:14 1993
	Last Modified:	cset.0009		10/23/92
	
	Copyright 1990-1993 XLNT Designs, Inc., ALL RIGHTS RESERVED

	This is the header file used throughout the CSP modules. This
	file defines C macros used to make the CSP source code portable.
	The macros listed here are to be provided by implementor.

	Modification History:
	*** SMT 7.2 ***
		Based on 6.2.3 multistation code.
*********************************************************************/

/*********************************************************************
	Implementor Definitions
*********************************************************************/

#define	CMT_DISABLE		ALL_DISABLE

/*********************************************************************
	Implementor Provided Macros
*********************************************************************/

/*
*	Processor State
*	Throughout the CSP portion of the SMT system, the system interrupts
*	will need to be disabled and enabled. This variable type defines
*	a variable large enough to hold the processor state information
*	required to change the interrupt settings.
*/
typedef long             ProcState;

/*********************************************************************
macro		MDisableCSPInterrupts (pState)

Function:	Set the system or processor state to disable interrupt
			processing for CSP interrupts.
Parameters:	pState	= address of buffer to hold current interrupt
			or processor state.
Input:		None.
Output:		pState	= contains current interrupt or processor state.
Return:		No value returned.
Notes:		Since interrupt processing is completely system dependent,
			the implementor must provide a facility for disabling
			interrupt processing. When CSP uses this macro to disable
			interrupts, it stores the current state and restores the
			current state to enable interrupts (see MRestoreInterrupts()).
Modification History:
*********************************************************************/
#define	MDisableCSPInterrupts(pState)\
		*(pState) = raise_interrupt_level(CMT_DISABLE);


/*********************************************************************
macro		MRestoreCSPInterrupts (pState)

Function:	Restore interrupt processing to a previous state.
Parameters:	pState	= address of buffer containing previous interrupt state.
Input:		pState	= containes interrupt state to restore to.
Output:		None.
Return:		No value returned.
Notes:		When interrupts are disabled in CSP, the previous interrupt
			level is remember. When the critical code section completes
			execution, the CSP code restore the interrupt processing
			state.
Modification History:
*********************************************************************/
#define	MRestoreCSPInterrupts(pState)\
		reset_interrupt_level (*(pState));
/*
@(#) if7_fbmmacro.h if7_v2.2.1:cset.0009:2:2 Mon Mar  1 16:49:14 1993 Copyright 1990-1993 XLNT Designs, Inc., ALL RIGHTS RESERVED
*/
/*********************************************************************
	Frame Based Management Header File
	
	FSP Macro File

	THIS FILE IS TO BE PORTED BY THE IMPLEMENTOR.

	File:			if7_fbmmacro.h
	Created:		12/01/89

	Version:		if7_v2.2.1		Mon Mar  1 16:49:14 1993
	Last Modified:	cset.0009		10/23/92
	
	Copyright 1990-1993 XLNT Designs, Inc., ALL RIGHTS RESERVED
	
	This is the header file used throughout the FSP modules. This
	file defines C macros used to make the FSP source code portable.
	The macros listed here are to be provided by implementor.

	Modification History:
	*** SMT 7.2 ***
		Based on 6.2.3 multistation code.
*********************************************************************/

/*********************************************************************
	Implementor Provided Macros
*********************************************************************/

/*
*	Processor State
*	Throughout the CSP portion of the SMT system, the system interrupts
*	will need to be disabled and enabled. This variable type defines
*	a variable large enough to hold the processor state information
*	required to change the interrupt settings.
*/
/* typedef int             ProcState; */

/*********************************************************************
macro		MDisableInterrupts (pState)

Function:	Set the system or processor state to disable interrupt
			processing for FBM interrupts.
Parameters:	pState	= address of buffer to hold current interrupt
			or processor state.
Input:		None.
Output:		pState	= contains current interrupt or processor state.
Return:		No value returned.
Notes:		Since interrupt processing is completely system dependent,
			the implementor must provide a facility for disabling
			interrupt processing. When FBM uses this macro to disable
			interrupts, it stores the current state and restores the
			current state to enable interrupts (see MRestoreInterrupts()).
Modification History:
*********************************************************************/
#define	MDisableInterrupts(pState)\
		*(pState) = raise_interrupt_level(CMT_DISABLE);


/*********************************************************************
macro		MRestoreInterrupts (pState)

Function:	Restore interrupt processing to a previous state.
Parameters:	pState	= address of buffer containing previous interrupt state.
Input:		pState	= containes interrupt state to restore to.
Output:		None.
Return:		No value returned.
Notes:		When interrupts are disabled in FBM, the previous interrupt
			level is remember. When the critical code section completes
			execution, the FBM code restore the interrupt processing
			state.
Modification History:
*********************************************************************/
#define	MRestoreInterrupts(pState)\
		reset_interrupt_level (*(pState));

#endif __VIP_XDI_H__

