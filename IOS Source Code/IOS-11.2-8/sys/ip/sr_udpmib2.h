/* $Id: sr_udpmib2.h,v 3.2 1995/11/17 09:36:41 hampton Exp $
 * $Source: /swtal/cherf.111/ios/sys/ip/sr_udpmib2.h,v $
 *------------------------------------------------------------------
 * SNMP/SNMPv2 bi-lingual agent code.
 *
 * February 1994, Jeffrey T. Johnson (via the SNMP Research MIB Compiler)
 *
 * Copyright (c) 1994-1995 by cisco Systems, Inc.
 * All rights reserved.
 *
 *------------------------------------------------------------------
 * $Log: sr_udpmib2.h,v $
 * Revision 3.2  1995/11/17  09:36:41  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  11:58:34  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.2  1995/06/28  09:27:01  smackie
 * Repair widespread subsystem header braindamage. (CSCdi23568)
 *
 *   o Fix subsystem entry points to be foo(subsystype *) not foo(void)
 *   o Repair nonsensical and redundant subsystem sequences
 *   o Use NULL where no property is required
 *
 * Revision 2.1  1995/06/07  21:01:15  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 *
 * Copyright (C) 1993 by SNMP Research, Incorporated.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by SNMP Research, Incorporated.
 *
 * Restricted Rights Legend:
 *  Use, duplication, or disclosure by the Government is subject to
 *  restrictions as set forth in subparagraph (c)(1)(ii) of the Rights
 *  in Technical Data and Computer Software clause at DFARS 52.227-7013
 *  and in similar clauses in the FAR and NASA FAR Supplement.
 *
 */

#ifndef _udpmib2_h_
#define _udpmib2_h_

#include "../snmp/snmp_api.h"
#include "sr_udpmib2defs.h"
#include "sr_udpmib2supp.h"
#include "sr_udpmib2part.h"
#include "sr_udpmib2type.h"

extern const ObjectInfo udpmib2_OidList[];
extern const uint       udpmib2_OidListNum;

extern void init_udpmib2(subsystype *);

#endif  /* _udpmib2_h_ */
      
