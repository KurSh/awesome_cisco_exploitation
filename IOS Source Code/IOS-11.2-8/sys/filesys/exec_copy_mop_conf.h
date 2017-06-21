/* $Id: exec_copy_mop_conf.h,v 3.1.40.1 1996/05/07 00:38:49 ssangiah Exp $
 * $Source: /release/112/cvs/Xsys/filesys/exec_copy_mop_conf.h,v $
 *------------------------------------------------------------------
 * Add file-id option to original copy mop command.
 *
 * February 1995, Yan Ke
 *
 * Copyright (c) 1994-1996 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: exec_copy_mop_conf.h,v $
 * Revision 3.1.40.1  1996/05/07  00:38:49  ssangiah
 * CSCdi56357:  Misleading erase command behavior in the RSP file system.
 *   o Remove erase command from the RSP file system parse chain.
 *   o Clean up the RSP file system parse chain.
 *   o Fix problem with return of error codes by the flash MIB agent.
 * Branch: California_branch
 *
 * Revision 3.1  1996/02/01  04:57:41  hampton
 * Migrate files out of the parser directory.  [CSCdi47717]
 *   Step 1: Create copies of the files to be moved in their new location.
 *
 * Revision 3.2  1995/11/17  18:45:50  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  12:52:08  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.2  1995/06/21  18:17:36  widmer
 * CSCdi36227:  Fix parse chain alternates
 *
 * Revision 2.1  1995/06/07  22:17:05  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 *	copy file mop fild_id
 */	
EOLS (exec_copy_mop_conf_id_eol, rsp_flash_copy_command, COPY_MOP_FILE_ID);

STRING (exec_copy_mop_conf_id, exec_copy_mop_conf_id_eol, no_alt,
	OBJ(string, 1),
	"Destination device or file name");

TEST_MULTIPLE_FUNCS(exec_copy_mop_conf, exec_copy_mop_conf_id,
		    NONE, ALTERNATE);

#undef	ALTERNATE
#define	ALTERNATE	exec_copy_mop_conf
