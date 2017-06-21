/* $Id: strncpy.c,v 1.1.4.1 1996/05/09 14:56:06 rbadri Exp $
 * $Source: /release/112/cvs/Xwbu/cat5k/alc_libc/strncpy.c,v $
 *------------------------------------------------------------------
 * strncpy.c
 *
 * ?
 *
 * Copyright (c) 1993-1996 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: strncpy.c,v $
 * Revision 1.1.4.1  1996/05/09  14:56:06  rbadri
 * Branch: California_branch
 * Synergy features to the mainline
 *
 * Revision 1.1.40.1  1996/04/27  06:15:36  cakyol
 * non sync sync from V112_0_2 to ....
 * Branch: LE_Cal_ATM_FR_California_Postsync_960425_branch
 *
 * Revision 1.1.28.1  1996/04/08  01:40:40  bbenson
 * Branch: LE_Cal_V112_0_2_branch
 * Sync of LE_Cal to new V112_0_2 sync point (post Port Ready).
 *
 * Revision 1.1.16.1  1996/03/22  09:11:45  rlowe
 * Non-sync Synalc3_LE_Cal_V111_1_0_3_merge_branch to V111_1_3
 * yielding LE_Syn_Cal_V111_1_3_branch.
 * Branch: LE_Syn_Cal_V111_1_3_branch
 *
 * Revision 1.1.2.1  1996/03/13  00:30:49  rlowe
 * Add file content from Synergy repository.
 * Branch: Synalc3_LE_Cal_V111_1_0_3_merge_branch
 *
 * Revision 1.1  1996/03/11  07:53:58  rlowe
 * Placeholders for California.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

#ifndef lint
static char sccsid[]="@(#)strncpy.c	1.1 6/24/93 ";
#endif
/*
					strncpy.c

	Copy n bytes from one string to another, stop on end of string.
*/
#include "types.h"
uChar *
strncpy(to, from, n)
register uChar   *to;
register uChar   *from;
register long    n;
{
    register uChar   *start = to;

    while( n ){
        n--;
        *to++ = *from;
        if( *from++ == '\0' )
            return(start);
    }
    *to = '\0';
    return(start);
} /* strncpy */

