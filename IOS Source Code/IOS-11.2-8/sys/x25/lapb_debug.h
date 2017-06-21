/* $Id: lapb_debug.h,v 3.2 1995/11/17 18:06:26 hampton Exp $
 * $Source: /swtal/cherf.111/ios/sys/x25/lapb_debug.h,v $
 *------------------------------------------------------------------
 * lapb_debug.h -- Debugging declarations for LAPB
 *
 * May 1993, John Bashinski
 *
 * Copyright (c) 1993-1994 by Cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: lapb_debug.h,v $
 * Revision 3.2  1995/11/17  18:06:26  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:52:37  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/06/07  23:21:04  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/* Function to fire up LAPB debugging */
extern void lapb_debug_init(void);

/*
 * The actual debugging flags are defined in lapb_debug_flags.h.
 * We include that file twice, once to define the flags themselves
 * and once to define the indices that the parser uses.
 */
#include "../x25/lapb_debug_flags.h"
#define __DECLARE_DEBUG_NUMS__
#include "../x25/lapb_debug_flags.h"
