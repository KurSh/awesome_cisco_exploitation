/* $Id: strcspn.c,v 1.1.4.1 1996/05/09 14:56:03 rbadri Exp $
 * $Source: /release/112/cvs/Xwbu/cat5k/alc_libc/strcspn.c,v $
 *------------------------------------------------------------------
 * strcspn.c
 *
 * ?
 *
 * Copyright (c) 1993-1996 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: strcspn.c,v $
 * Revision 1.1.4.1  1996/05/09  14:56:03  rbadri
 * Branch: California_branch
 * Synergy features to the mainline
 *
 * Revision 1.1.40.1  1996/04/27  06:15:34  cakyol
 * non sync sync from V112_0_2 to ....
 * Branch: LE_Cal_ATM_FR_California_Postsync_960425_branch
 *
 * Revision 1.1.28.1  1996/04/08  01:40:37  bbenson
 * Branch: LE_Cal_V112_0_2_branch
 * Sync of LE_Cal to new V112_0_2 sync point (post Port Ready).
 *
 * Revision 1.1.16.1  1996/03/22  09:11:42  rlowe
 * Non-sync Synalc3_LE_Cal_V111_1_0_3_merge_branch to V111_1_3
 * yielding LE_Syn_Cal_V111_1_3_branch.
 * Branch: LE_Syn_Cal_V111_1_3_branch
 *
 * Revision 1.1.2.1  1996/03/13  00:30:46  rlowe
 * Add file content from Synergy repository.
 * Branch: Synalc3_LE_Cal_V111_1_0_3_merge_branch
 *
 * Revision 1.1  1996/03/11  07:53:55  rlowe
 * Placeholders for California.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

#ifndef lint
static char sccsid[]="@(#)strcspn.c	1.1 6/24/93 ";
#endif
/*		
						strcspn.c
 	Return the number of characters in the maximum leading segment
  	of string which consists solely of characters NOT from charset.
*/
int
strcspn(string, charset)
char    *string;
register char   *charset;
{
    register char *p, *q;

    for(q=string; *q != '\0'; ++q) {
        for(p=charset; *p != '\0' && *p != *q; ++p)
            ;
        if(*p != '\0')
            break;
    }
    return(q-string);
}
