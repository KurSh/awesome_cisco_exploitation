/* $Id: exec_debug_netbios.h,v 3.2 1995/11/17 09:22:10 hampton Exp $
 * $Source: /swtal/cherf.111/ios/sys/ibm/exec_debug_netbios.h,v $
 *------------------------------------------------------------------
 * E X E C _ D E B U G _ N E T B I O S . H
 *
 * Copyright (c) 1992-1995 by cisco Systems, Inc.
 * All Rights Reserved
 *------------------------------------------------------------------
 * $Log: exec_debug_netbios.h,v $
 * Revision 3.2  1995/11/17  09:22:10  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  11:44:29  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/07/02  10:53:53  tli
 * CSCdi36508:  Rationalize parser file placement, part I
 * Rationalize os, hes, les, if, and ibm.
 *
 * Revision 2.2  1995/06/19  14:48:30  widmer
 * CSCdi36074:  Clean up debug parse chain alternates
 * Add keyword_debug macro to assign debug array
 *
 * Revision 2.1  1995/06/07  22:19:31  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/******************************************************************
 * debug netbios-name-cache
 */
EOLS	(debug_netbios_eol, debug_command, DEBUG_NETBIOS_NAMEC);
KEYWORD_DEBUG(debug_netbios, debug_netbios_eol, ALTERNATE,
	      OBJ(pdb,1), netbios_debug_arr,
	      "netbios-name-cache", "NetBIOS name cache tracing", PRIV_OPR);

#undef	ALTERNATE
#define	ALTERNATE	debug_netbios
