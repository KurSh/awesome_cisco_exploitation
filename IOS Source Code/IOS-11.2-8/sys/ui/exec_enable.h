/* $Id: exec_enable.h,v 3.3 1995/12/15 22:28:22 widmer Exp $
 * $Source: /vws/aam/cherf.111/ios/sys/ui/exec_enable.h,v $
 *------------------------------------------------------------------
 * E X E C _ E N A B L E . H
 *
 * Copyright (c) 1992-1995 by cisco Systems, Inc.
 * All Rights Reserved
 *------------------------------------------------------------------
 * $Log: exec_enable.h,v $
 * Revision 3.3  1995/12/15  22:28:22  widmer
 * CSCdi45685:  Need priv flag for interactive commands
 * Add Priv_Interactive to flag interactive commands
 *
 * Revision 3.2  1995/11/17  17:47:55  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:41:04  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.2  1995/08/28  15:39:09  widmer
 * CSCdi39335:  enable and disable commands should allow full priv range
 *
 * Revision 2.1  1995/07/04  01:59:26  tli
 * CSCdi36508: Rationalize parser file placement, part I
 * Rationalize cls dev dlsw dspu filesys irix lat les lex llc2 parser pgen
 * pt servers smf smrp snmp src-36-c1000 src-4k-c4000 src-68-c3000
 * src-68-c4000 src-c4000 src-rsp srt tcl ts ui
 *
 * Revision 2.1  1995/06/07  22:20:45  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/****************************************************************
 * enable
 */
EOLS	(enable_endline, enable_command, CMD_ENABLE);
NUMBER	(enable_level, enable_endline, enable_endline,
	 OBJ(int,1), PRIV_MIN, PRIV_MAX, "Enable level");
KEYWORD_ID(enable, enable_level, ALTERNATE,
	   OBJ(int,1), PRIV_MAX,
	   "enable", "Turn on privileged commands",
	   PRIV_MIN | PRIV_INTERACTIVE);

#undef  ALTERNATE
#define ALTERNATE	enable
