/* $Id: alias.c,v 3.2.58.1 1996/03/21 23:26:27 gstovall Exp $
 * $Source: /release/111/cvs/Xsys/rommon/src/alias.c,v $
 *------------------------------------------------------------------
 * alias.c
 *
 * November 1992, Rob Clevenger
 *
 * Copyright (c) 1994-1996 by cisco Systems, Inc.
 * All rights reserved.
 *
 * Support routines for monitor alias and environment variables.
 *------------------------------------------------------------------
 * $Log: alias.c,v $
 * Revision 3.2.58.1  1996/03/21  23:26:27  gstovall
 * Branch: California_branch
 * The ELC_branch now knows the joy of California dreaming.
 *
 * Revision 3.2.2.1  1995/12/01  04:19:35  mbeesley
 * Branch: ELC_branch
 * Reorganize Rom monitor header files to make it easier to port
 * to new platforms.
 *
 * Revision 3.2  1995/11/17  18:40:49  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:04:31  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/06/07  22:36:34  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
** String table routines.
** Some of the monitor parameters are stored as strings in tables (i.e.
** monitor environment variables and aliases).  The following routines
** provide table access.
*/

#include "monitor.h"
#include "strings.h"
static char *endtbl(), *eliminate();

/*
** Search the variable table for the name pointed to by vname.  Return a
** pointer to the value string for that name or zero if vname not found.
*/
char *
getvar(char *tblptr, char *vname)
{
  register found = 0;

  while(1) {
    if(!(*tblptr)) break;  /* NULL name means end of our table */
    if(strcmp(vname,tblptr) == 0) found = 1;  /* name matches */
#ifdef DEBUG
    printf("getvar: comparing '%s' with '%s'\n",vname,tblptr);
#endif
    tblptr = strchr(tblptr, '\0') + 1;  /* bump pointer past name */
#ifdef DEBUG
    printf("getvar: var value is '%s'\n",tblptr);
#endif
    if(found) return(tblptr);
    tblptr = strchr(tblptr, '\0') + 1;  /* bump pointer past value */
  }
  return((char *)0);
}

/*
** Set a variable to a value.  The variable name may be new or may already
** exist in the table in which case the old value will be clobbered.
** Strptr must point to a string of the form "VARNAME=varval".
*/
setvar(char *tblptr, int tblsiz, char *strptr)
{
  register char *tptr;  /* our temporary pointer */
  register char *vptr;  /* our value pointer */

  if(!(vptr = strchr(strptr,'='))) return(-1);  /* there must be a '=' */
  /* null out the '=' and point to the second string */
  *vptr++ = '\0';
  /* if this item is already in our table remove it (eliminate()) */
  if(tptr = getvar(tblptr,strptr)) tptr = eliminate(tblptr,tptr);
  else tptr = endtbl(tblptr);  /* tptr now points to the end of the table */
  /* add the item to the end of the table if there is room */
  if((tblsiz - (tptr - tblptr)) < (strlen(strptr) + strlen(vptr) + 3)) {
    printf("out of room in table, cannot set \"%s\"\n",strptr);
    return(-2);
  }
  strcpy(tptr,strptr);  /* copy the variable name */
  tptr = strchr(tptr,'\0') + 1;  /* move pointer past variable name */
  strcpy(tptr,vptr);  /* copy the variable value */
  tptr = strchr(tptr,'\0') + 1;  /* move pointer past variable value */
  *tptr = '\0';  /* terminate the table */
  return(0);
}

/*
** Unset a variable.  This is different from setting it to a null string.
** This deletes the variable name completely.
*/
int
unsetvar(char *tblptr, char *var)
{
  char *vptr;

  if(vptr = getvar(tblptr,var)) {
    eliminate(tblptr,vptr);
    return(0);
  }
  return(-1);
}

/*
** Print out for the user the entire table.
*/
int
printtbl(char *tblptr)
{
    char buffer[256];

    while(*tblptr) {  /* NULL name means end of our table */
	printf("%s=",tblptr);
	tblptr = strchr(tblptr, '\0') + 1;  /* bump pointer past name */
	printf("%s\n",tblptr);
	tblptr = strchr(tblptr, '\0') + 1;  /* bump pointer past value */
    }
    return(0);
}

/*
** Return a pointer to the terminating NULL variable name.
*/
static char *
endtbl(char *tblptr)
{
  while(1) {
    if(!(*tblptr)) return(tblptr);  /* NULL variable name is terminator */
    tblptr = strchr(tblptr,'\0') + 1;  /* move pointer past variable name */
    tblptr = strchr(tblptr,'\0') + 1;  /* move pointer past variable value */
  }
}

/*
** Eliminate the evar, that is, move everything past the evar at ptr
** up to clobber that item.  Ptr must point to the beginning of the value
** portion (as returned from getvar).
*/
static char *
eliminate(char *tblptr, char *vptr)
{
  register char *nextptr;  /* pointer to next entry */

  /* vptr must be pointing at the value portion of an var */
  nextptr = strchr(vptr,'\0') + 1;  /* point nextptr past value portion */

  vptr -= 2;   /* back up over name terminator */
  while(vptr > tblptr && *vptr) vptr--;  /* back up over entire name */
  if(!*vptr) vptr++;  /* ensure we are pointing to first char of name */

  movbyte((unsigned char *)nextptr,(unsigned char *)vptr,
	  endtbl(tblptr)-nextptr+1);  /* copy terminator too */
  return(endtbl(tblptr));
}


/*
** Monitor variable routine for convenience.
** Setmonvar:
**   Set a variable to a value and update the NVRAM copy.
**   The variable name may be new or may already exist in the table
**   in which case the old value will be clobbered.
**   Strptr must point to a string of the form "VARNAME=varval".
**   VARNAME cannot contain a leading dollar sign.
**   VARNAMEs are traditionally all caps.
**   It is a good idea to stick to letters, numbers and underbars
**   when specifying VARNAMEs, although any character (except NULL)
**   will be accepted.
**   Returns a positive integer indicating the space left in the table
**   if the operation is successful.
**   Returns -1 if there is a problem with the string format.
**   Returns -2 if there is not enough room in the Monitor Variable
**   table.
**   To get the number of bytes left in the table without actually
**   setting a variable make str a NULL ptr.
*/
int
setmonvar(char *str)
{
    int retval;

#ifdef DEBUG
printf("Entering setmonvar with string: %s\n",str);
#endif
    if(str) {
	retval = setvar((NVRAM)->evartbl, EVTSIZ, str);
#ifdef DEBUG
printf("setmonvar: retval from setvar is %d\n", retval);
#endif
	if(retval < 0) return(retval);
    }
    savenv();
    return((int)(&(NVRAM)->evartbl[EVTSIZ] - endtbl((NVRAM)->evartbl)));
}

/*
** Monitor variable routine for convenience.
** Getmonvar:
**   Specify the variable name (exact spelling and case), a pointer
**   to a buffer to place the resulting string and the actual size
**   of the buffer.
**   No dollar sign is needed on "VARNAME".
**   VARNAMEs are traditionally all caps.
**   Returns the number of non-null characters (positive integer)
**   copied if the whole variable fits in the destination buffer.
**   Note that this number can be zero if the variable is set to
**   an empty string.
**   Returns -1 if variable does not exist.
**   Returns -2 if the destination buffer is not large enough.
*/
int
getmonvar(char *varname, char *destbuf, int destbufsiz)
{
    char *ptr;
    int len;

    if(!(ptr = getvar((NVRAM)->evartbl, varname))) return(-1);
    if((len = strlen(ptr)) > destbufsiz) return(-2);
    strcpy(destbuf, ptr);
    return(len);
}
