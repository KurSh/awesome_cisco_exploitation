/* $Id: exec_show_controllers_mci.h,v 3.3.20.1 1996/08/12 16:01:32 widmer Exp $
 * $Source: /release/112/cvs/Xsys/hes/exec_show_controllers_mci.h,v $
 *------------------------------------------------------------------
 * MCI show controllers commands
 *
 * May 1993, Robert Widmer
 *
 * Copyright (c) 1993-1996 by cisco Systems, Inc.
 * All rights reserved.
 * 
 *------------------------------------------------------------------
 * $Log: exec_show_controllers_mci.h,v $
 * Revision 3.3.20.1  1996/08/12  16:01:32  widmer
 * CSCdi65725:  Create Eolns macro that defaults csb->which to 0
 * Branch: California_branch
 * Add Eolns and Nvgenns macro that default csb->which to 0
 * Remove hidden keyword help strings
 *
 * Revision 3.3  1996/01/29  07:08:41  hampton
 * Remove command number arguments passed to parse routines that only
 * perform a single function.  [CSCdi47717]
 *
 * Revision 3.2  1995/11/17  09:15:56  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  11:38:31  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/07/02  10:51:55  tli
 * CSCdi36508:  Rationalize parser file placement, part I
 * Rationalize os, hes, les, if, and ibm.
 *
 * Revision 2.1  1995/06/07  22:23:15  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/******************************************************************
 * show controllers mci
 *
 */
EOLNS	(show_control_mci_eol, show_mci);
KEYWORD (show_control_mci, show_control_mci_eol, NONE,
	 "mci","MCI interface internal state", PRIV_USER);
TEST_INT(show_control_mci_test, show_control_mci, NONE, ALTERNATE,
	 nMCI);

#undef	ALTERNATE
#define	ALTERNATE	show_control_mci_test
