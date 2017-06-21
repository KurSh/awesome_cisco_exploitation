/* $Id: diag.c,v 1.1.68.1 1996/06/16 21:18:45 vnguyen Exp $
 * $Source: /release/112/cvs/Xsys/rommon/src-68-as5200/diag.c,v $
 *------------------------------------------------------------------
 * diag.c -- Diagnostic Menus for Brasil.
 *
 * October 1995, Joe Turner
 *
 * Copyright (c) 1995-1996 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: diag.c,v $
 * Revision 1.1.68.1  1996/06/16  21:18:45  vnguyen
 * Finally committing Brasil to California_branch
 *
 * Revision 1.1  1995/12/03  21:16:24  jturner
 * Placeholder for Brasil.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */
/*
** The diagnostic menu structures and diag routine.
*/
#include "monitor.h"
#include "menu.h"
#include "setjmp.h"
#include "signal_4k.h"
#include "error.h"
#include "mon_boot.h"
#include "cookie.h"

typedef int (*PFI)();  /* pointer to a function returning an integer */
extern int netflashbooted;

/* often used menu strings */
static char  dgmenustr[] = "Diagnostic Menu";
static char  adiagfstr[] = "alter diag flags";
static char  aparamstr[] = "alter test parameters";
static char doalldgstr[] = "do all diags in this menu";
static char dogrpdgstr[] = "do group of diags in this menu";
static char  regtststr[] = "register test";
static char  inttststr[] = "interrupt test";

/* misc. */
static long one   = 1;
static long two   = 2;
static long three = 3;

/* forward declarations */
void doalldiags(struct menuinfo *menup);
void dogrpdiags(struct menuinfo *menup);
void togglediagflag(int flagmask);
void showdflags(void);

static char togglestr[] = "toggle \"%s\" flag";
static char contstr[] = "Continuous";
static char stopstr[] = "Stop on error";
static char loopstr[] = "Loop on error";
static char quietstr[] = "Quiet mode";
static int contmask = D_CONTINUOUS, stopmask = D_STOPONERR,
  loopmask = D_LOOPONERR, quietmask = D_QUIETMODE;
static struct mitem diagflagitems[] = {
  togglestr, 0, (int)contstr, (PFI)togglediagflag, &contmask, 0,(int(*)())0, 0,
  togglestr, 0, (int)stopstr, (PFI)togglediagflag, &stopmask, 0,(int(*)())0, 0,
  togglestr, 0, (int)loopstr, (PFI)togglediagflag, &loopmask, 0,(int(*)())0, 0,
  togglestr, 0, (int)quietstr, (PFI)togglediagflag, &quietmask, 0,(int(*)())0, 0,
};
static struct menuinfo diagflagm = {
  "Diagnostic Flag Menu",                      /* title */
  0,                                           /* no title param */
  (PFI)showdflags,                             /* menu title function */
  "enter item to toggle > ",                   /* our special prompt */
  sizeof(diagflagitems)/sizeof(struct mitem),  /* size of menu */
  diagflagitems,
};
static struct menuinfo *diagflagmp = &diagflagm;

extern int alt_mem(), dis_mem(), fil_mem(), mov_mem(), cmp_mem(), reset();
extern int memtest(), memloop(), addrloop(), sysmemtest(), berrscan();
extern berrtest(), memdebug();
extern toss_cookie();
int iface_present();

extern int bri_ext_test();

extern int flash_test();

static struct mitem utilmenuitems[] = {
    "alter memory",                0,0,alt_mem,        (int *)&one,0,(int(*)())0, 0,
    "bus error scan",              0,0,berrscan,       (int *)&one,0,(int(*)())0, 0,
    "compare memory block",        0,0,cmp_mem,        (int *)&one,0,(int(*)())0, 0,
    "display memory",              0,0,dis_mem,        (int *)&one,0,(int(*)())0, 0,
    "move memory block",           0,0,mov_mem,        (int *)&one,0,(int(*)())0, 0,
    "fill memory",                 0,0,fil_mem,        (int *)&one,0,(int(*)())0, 0,
    "memory test",                 0,0,memtest,        (int *)&one,0,(int(*)())0, 0,
    "memory read or write loop",   0,0,memloop,        (int *)&one,0,(int(*)())0, 0,
    "memory debug loop",           0,0,memdebug,       (int *)&one,0,(int(*)())0, 0,
    "address loop",                0,0,addrloop,       (int *)&one,0,(int(*)())0, 0,
    "system reset",                0,0,reset,          (int *)&one,0,(int(*)())0, 0,
    "serial cookie utility",       0,0,toss_cookie,    (int *)&one,0,(int(*)())0, 0,
};
static struct menuinfo utilmenu = {
    "Diagnostic Utilities Menu",
    0,
    0,
    0,
    sizeof(utilmenuitems)/sizeof(struct mitem),
    utilmenuitems,
};
static struct menuinfo *utilmenup = &utilmenu;

extern struct menuinfo *maindiagp;
static struct mitem maindiagitems[] = {
    adiagfstr,                   0, 0, (PFI)menu,       (int *)&diagflagmp, 0,(int(*)())0, 0,
    "basic utilities",           0, 0, (PFI)menu,       (int *)&utilmenup, 0,(int(*)())0, 0,
    doalldgstr,                  0, 0, (PFI)doalldiags, (int *)&maindiagp, MF_CONTINUOUS,(int(*)())0, 0,
    dogrpdgstr,                  0, 0, (PFI)dogrpdiags, (int *)&maindiagp, MF_CONTINUOUS,(int(*)())0, 0,
    "bus error test",            0, 0, berrtest,   (int *)&one,       MF_CONTINUOUS | MF_DOALL,(int(*)())0, 0,
    "main memory test",          0, 0, sysmemtest, (int *)&one,       MF_CONTINUOUS | MF_DOALL | MF_NOTNET,(int(*)())0, 0,
    "flash memory test", (int(*)())0, 0, flash_test, (int *)&one,\
	MF_CONTINUOUS | MF_DOALL, (int(*)())0, 0,
};

static struct menuinfo maindiag = {
  "Main %s",                                   /* title */
  (int)dgmenustr,                              /* title param */
  (PFI)showdflags,                             /* menu title function */
  0,                                           /* generic prompt */
  sizeof(maindiagitems)/sizeof(struct mitem),  /* size of menu */
  maindiagitems,
};
static struct menuinfo *maindiagp = &maindiag;

/*
** The menu driven diagnostics are called from either the low level
** power-on routine or from the monitor.  Frommon will be non-zero if
** called from the monitor.  Note that this value is actually the
** argc parameter (a little slight of hand here), which is always
** greater than zero.
*/
void
diag_menu(int argc, char *argv[])
{
    char arg;
    unsigned char envflagsav;

    if(argc > 1) arg = *argv[1];
    else arg = 0;
    envflagsav = envflag;
    envflag = INDIAG;     /* set the environment flag */
    sys_dirty = 1;        /* diags will dirty up the system */
    menu(&maindiag, arg);
    envflag = envflagsav;
}

void
togglediagflag(int flagmask)
{
    DIAGFLAG ^= flagmask;
}

void
showdflags(void)
{
  register unsigned char flag = DIAGFLAG;
  static char onstr[] = "ON", offstr[] = "OFF";

  printf("FLAGS: %s %s  %s %s  %s %s  %s %s\n",
	 contstr, (flag & D_CONTINUOUS) ? onstr : offstr,
	 stopstr, (flag & D_STOPONERR)  ? onstr : offstr,
	 loopstr, (flag & D_LOOPONERR)  ? onstr : offstr,
	 quietstr, (flag & D_QUIETMODE)  ? onstr : offstr);
}

void
pr_err_accum(void)
{
    printf("\n%d total accumulated errors\n", err_accum);
}

void
doalldiags(struct menuinfo *menup)
{
    register char i;
    register struct mitem *miptr =  menup->miptr;
    jmp_buf *savjmp = monjmpptr;   /* save original jmpbuf */
    jmp_buf localjmp;
    int abort;

    abort = setjmp(localjmp);
    if(!abort) {
	monjmpptr = &localjmp; /* redirect breaks */
	for(i=0; i<menup->msize; i++, miptr++) {

	    /* 
	     * Check that if NetBooted, MF_NOTNET not set and
	     * the the MF_DOALL flag is set
	     */
	    if( (!netflashbooted || !(miptr->mflag & MF_NOTNET) ) &&
	        (miptr->mflag & MF_DOALL) ) {
		/* Check for existance of the menu item (i.e. interface) */
		if ( !(miptr->mixfunc) || (*miptr->mixfunc)(miptr->mixparam) ) {
		    (*miptr->mfunc)(*miptr->mfparam);
		    err_accum += errcount;
		    errcount = 0;
		}
	    }
	}
    }
    pr_err_accum();
    monjmpptr = savjmp;  /* redirect breaks back */
    if(abort && savjmp) longjmp(savjmp,1);
}

void
dogrpdiags(struct menuinfo *menup)
{
    register char i, menuitem;
    register struct mitem *miptr;
    register struct nvram *nvptr = NVRAM;
    jmp_buf *savjmp = monjmpptr;  /* save original jmpbuf */
    jmp_buf localjmp;
    char numitems;
    char buffer[sizeof(nvptr->diaglist)];
    int abort;

    /*
    ** Prompt the user for the items to include in the group test.
    ** If the tests are to be run continuously prompt only on the
    ** first pass.
    */
    if(!(DIAGFLAG & D_CONTINUOUS) || testpass == 1) {
	displaymenu(menup);
	printf("\nenter the menu items to include in test (in order) "
	       "followed by <RET>\nor just <RET> to re-execute current "
	       "list [%s]: ", nvptr->diaglist);
	getline(buffer, sizeof(buffer));
	if(*buffer) strcpy(nvptr->diaglist, buffer);  /* user entered a list */
    }
    numitems = strlen(nvptr->diaglist);
    abort = setjmp(localjmp);
    if(!abort) {
	monjmpptr = &localjmp; /* redirect breaks */
	for(i=0; i<numitems; i++) {
	    menuitem = (nvptr->diaglist[i] - 'a');
	    miptr = &menup->miptr[menuitem];

	    /*
	     * Check that item is in the list and is valid
	     * even if the image is net booted
	     */
	    if( (netflashbooted && (miptr->mflag & MF_NOTNET) ) ||
	        (menuitem < 0) ||
	        (menuitem >= menup->msize) ) {
		continue;
	    }
	    if(miptr->mflag & MF_DOALL) {  /* check to make sure we can */
		/* Check for existance of the menu item (i.e. interface) */
		if ( !(miptr->mixfunc) || (*miptr->mixfunc)(miptr->mixparam) ) {
		    (*miptr->mfunc)(*miptr->mfparam);
		    err_accum += errcount;
		    errcount = 0;
		}
	    }
	}
    }
    pr_err_accum();
    monjmpptr = savjmp;  /* redirect breaks back */
    if(abort && savjmp) longjmp(savjmp,1);
}

int
flags(int c)
{
  int mask;

  switch(c) {
  case 'C': mask = D_CONTINUOUS; break;
  case 'L': mask = D_LOOPONERR; break;
  case 'Q': mask = D_QUIETMODE; break;
  case 'S': mask = D_STOPONERR; break;
  default: puts("unknown flag\n"); return(-1);
  }
  togglediagflag(mask);
  return(0);
}

/************************************************************************
 *
 *  Function iface_present
 *
 *  This routine determines if a particular interface is present on  
 *  on the given platform. It checks the cookie bits to make this
 *  determination. It is used to determine whether a test is displayed
 *  in the main menu of tests to run.
 *  
 *
 *  Returns: FALSE: 0 if the interface does not exist.
 *           TRUE:  1 if it does exist.      
 ************************************************************************/

int iface_present (int hw_type)
{
    struct cookie *cptr;

    cptr = (struct cookie *) get_cookie();

    if ((hw_type == INTERFACE_1E1T) && (cptr->interfaces == INTERFACE_1E1T)){
	return(1);
    }
    else if ((hw_type == INTERFACE_1E1B) && (cptr->interfaces == INTERFACE_1E1B)){
	return(1);
    }
    else if ((hw_type == INTERFACE_1E1N) && (cptr->interfaces == INTERFACE_1E1N)){
	 return(1);
    }
    else {
	return(0);
    }
}
/* end of module */




