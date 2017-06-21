/* $Id: x25_debug_flags.h,v 3.2 1995/11/17 18:07:33 hampton Exp $
 * $Source: /swtal/cherf.111/ios/sys/x25/x25_debug_flags.h,v $
 *------------------------------------------------------------------
 * x25_debug_flags.h -- Debugging flag declarations for X.25
 *
 * May 1993, John Bashinski
 *
 * Copyright (c) 1993-1994 by Cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: x25_debug_flags.h,v $
 * Revision 3.2  1995/11/17  18:07:33  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:53:38  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/06/07  23:22:18  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 * We get included (multiple times) from X.25 code to define
 * debugging flags. For this, we use some macros that get defined
 * by debug_macros.h. What definitions the macros get depends on
 * which magic flags have been set by the file that includes us.
 */
#include "../ui/debug_macros.h"

/* Now define the actual flags and the array that points to them */
DEBUG_ARRDECL(x25_debug_arr)

DEBUG_FLAG(x25_debug,DEBUG_X25,"X25 packet")
DEBUG_FLAG(x25event_debug,DEBUG_X25EV,"X25 special event")

DEBUG_ARRDONE
