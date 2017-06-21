/* $Id: ipx_nlsp_chain.c,v 3.6.10.2 1996/06/03 20:23:39 john Exp $
 * $Source: /release/112/cvs/Xsys/xns/ipx_nlsp_chain.c,v $
 *------------------------------------------------------------------
 * Parse chains for IPX/NLSP
 *
 * October, 1994, Joel P. Bion
 *
 * Copyright (c) 1994-1996 by cisco Systems, Inc.
 * All rights reserved.
 *
 *------------------------------------------------------------------
 * $Log: ipx_nlsp_chain.c,v $
 * Revision 3.6.10.2  1996/06/03  20:23:39  john
 * CSCdi58748:  ipxwan and nlsp command nvgen order wrong and subsys issues
 * Branch: California_branch
 *
 * Revision 3.6.10.1  1996/05/21  10:11:20  thille
 * CSCdi51599:  multiple /interface/ literals bloat code.
 * Branch: California_branch
 * Save another 392 bytes by doing away with duplicate literals.
 *
 * Revision 3.6  1996/02/22  14:40:31  widmer
 * CSCdi49281:  make all parser_extension_request static const
 * make parser_extension_request typedef const
 * make all parser_extension_request static
 *
 * Revision 3.5  1996/02/01  06:13:24  hampton
 * Migrate files out of the parser directory.  [CSCdi47717]
 *   Step 2: Change all files which reference the moved parser files to
 *   point to their new locations.
 *
 * Revision 3.4  1996/01/29  07:31:43  hampton
 * Remove unnecessary includes of parser definition files.  [CSCdi47717]
 *
 * Revision 3.3  1995/12/17  18:42:22  thille
 * CSCdi45760: Parser needs to go on a diet.
 * Move object files that are not part of parser to os_exec or libutil.a
 * as appropriate.  Rename parser files to make it clear what is part of
 * parser and avoid other stuff accumulating.  Modify files that include
 * parser .h files to deal with new names.
 *
 * Revision 3.2  1995/11/17  19:21:50  gstovall
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:58:09  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.2  1995/08/25  15:00:06  dkatz
 * CSCdi38139:  ISIS needs better control of adjacency parameters
 *
 * Revision 2.1  1995/06/07  23:27:40  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */
#include "master.h"
#include "../ui/common_strings.h"
#include "interface_private.h"
#include "packet.h"
#include "../if/network.h"
#include "config.h"
#include "parser.h"
#include "../iprouting/route.h"
#include "../xns/novell.h"
#include "../xns/novroute.h"
#include "../ui/debug.h"
#include "xns.h"
#include "novell.h"
#include "novell_debug.h"

#include "../parser/parser_actions.h"
#include "../xns/novell_actions.h"
#include "../parser/macros.h"
#include "../xns/parser_defs_xns_nov.h"
#include "../parser/parser_defs_parser.h"
#include "../parser/parser_defs_sse.h"
#include "../iprouting/parser_defs_igrp2.h"
#include "../clns/parser_defs_isis.h"
#include "../clns/clns_route.h"
#include "../clns/isis.h"

/*
 * Parse chains for DEBUG commands
 */
#define ALTERNATE NONE
#include "exec_debug_ipx_nlsp.h"
LINK_POINT(ipx_nlsp_debug_commands, ALTERNATE);
#undef ALTERNATE


/*
 * Parse chains for SHOW commands
 */
#define ALTERNATE NONE
#include "exec_show_ipx_nlsp.h"
LINK_POINT(ipx_nlsp_show_commands, ALTERNATE);
#undef ALTERNATE


#define ALTERNATE NONE
#include "cfg_ipx_nlsp.h"
LINK_POINT(ipx_nlsp_top_commands, ALTERNATE);
#undef ALTERNATE

/*
 * Parse chains for interface commands
 */
#define ALTERNATE NONE
#include "cfg_int_ipx_nlsp.h"
LINK_POINT(ipx_nlsp_int_commands, ALTERNATE);
#undef ALTERNATE


/*
 * Parse chains for router commands
 */
#define ALTERNATE NONE
#include "cfg_nov_router_nlsp.h"
LINK_POINT(ipx_nlsp_router_commands, ALTERNATE);
#undef ALTERNATE

/*
 * Parse chains for clear commands
 */
#define ALTERNATE NONE
#include "exec_clear_ipx_nlsp.h"
LINK_POINT(ipx_nlsp_clear_commands, ALTERNATE);
#undef ALTERNATE

/*
 * Parse chain registration array for IPX/NLSP
 */
static parser_extension_request ipx_nlsp_chain_init_table[] = {
    { PARSE_ADD_CFG_TOP_IPX_CMD, &pname(ipx_nlsp_top_commands) },
    { PARSE_ADD_SHOW_IPX_CMD,&pname(ipx_nlsp_show_commands) },
    { PARSE_ADD_CLEAR_IPX_CMD, &pname(ipx_nlsp_clear_commands) },
    { PARSE_ADD_DEBUG_IPX_CMD,&pname(ipx_nlsp_debug_commands) },
    { PARSE_ADD_CFG_INT_IPX_CMD,&pname(ipx_nlsp_int_commands) },
    { PARSE_ADD_ROUTER_IPX_CMD,&pname(ipx_nlsp_router_commands) },
    { PARSE_LIST_END, NULL }
};

/*
 * ipx_nlsp_parser_init - Initialize parse chains for NOVELL
 * 
 * This routine is called at system startup to register the parse chains
 * for NOVELL.
 */

void ipx_nlsp_parser_init (void)
{
   parser_add_command_list(ipx_nlsp_chain_init_table, "ipxnlsp");
}
