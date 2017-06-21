/* $Id: telnet_debug_flags.h,v 3.2 1995/11/17 18:49:48 hampton Exp $
 * $Source: /swtal/cherf.111/ios/sys/tcp/telnet_debug_flags.h,v $
 *------------------------------------------------------------------
 * telnet_debug_flags.h -- Debugging flag declarations for TELNET
 *
 * May 1993, John Bashinski
 *
 * Copyright (c) 1993-1994 by Cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: telnet_debug_flags.h,v $
 * Revision 3.2  1995/11/17  18:49:48  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:36:26  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/06/07  23:06:37  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 * We get included (multiple times) from TELNET code to define
 * debugging flags. For this, we use some macros that get defined
 * by debug_macros.h. What definitions the macros get depends on
 * which magic flags have been set by the file that includes us.
 */
#include "../ui/debug_macros.h"

/* Now define the actual flags and the array that points to them */
DEBUG_ARRDECL(telnet_debug_arr)

DEBUG_FLAG(telnet_debug,DEBUG_TELNET,"Incoming Telnet")

DEBUG_ARRDONE
