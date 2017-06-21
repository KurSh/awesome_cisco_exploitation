/* $Id: ipalias_chain.c,v 3.4.10.1 1996/05/21 09:51:18 thille Exp $
 * $Source: /release/112/cvs/Xsys/ip/ipalias_chain.c,v $
 *------------------------------------------------------------------
 * IP alias parse chains
 *
 * May 1993, Robert Widmer
 *
 * Copyright (c) 1993-1996 by cisco Systems, Inc.
 * All rights reserved.
 *
 *------------------------------------------------------------------
 * $Log: ipalias_chain.c,v $
 * Revision 3.4.10.1  1996/05/21  09:51:18  thille
 * CSCdi51599:  multiple /interface/ literals bloat code.
 * Branch: California_branch
 * Save another 392 bytes by doing away with duplicate literals.
 *
 * Revision 3.4  1996/02/22  14:34:35  widmer
 * CSCdi49281:  make all parser_extension_request static const
 * make parser_extension_request typedef const
 * make all parser_extension_request static
 *
 * Revision 3.3  1995/12/17  18:29:41  thille
 * CSCdi45760: Parser needs to go on a diet.
 * Move object files that are not part of parser to os_exec or libutil.a
 * as appropriate.  Rename parser files to make it clear what is part of
 * parser and avoid other stuff accumulating.  Modify files that include
 * parser .h files to deal with new names.
 *
 * Revision 3.2  1995/11/17  09:34:31  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  11:56:23  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.2  1995/07/01  12:55:52  tli
 * CSCdi36508:  Rationalize parser file placement, part I
 * Rationalize ip, ipmulticast, tcp
 *
 * Revision 2.1  1995/06/07  20:58:38  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

#include "master.h"
#include "../ui/common_strings.h"
#include "config.h"
#include "parser.h"
#include "packet.h"
#include "../parser/parser_actions.h"
#include "../parser/macros.h"
#include "../ip/ip.h"
#include "../parser/parser_defs_tcpip.h"


/*
 * Parse chains for IP Alias show commands
 */
#define	ALTERNATE	NONE
#include "exec_show_ip_alias.h"
LINK_POINT(ipalias_show_commands, ALTERNATE);
#undef	ALTERNATE


/*
 * Parse chains for IP Alias config commands
 */
#define ALTERNATE       NONE
#include "cfg_ip_alias.h"
LINK_POINT(ipalias_config_commands, ALTERNATE);
#undef	ALTERNATE


/*
 * Parse chain registration array for IP Alias
 */
static parser_extension_request ipalias_chain_init_table[] = {
    { PARSE_ADD_SHOW_IP_CMD, &pname(ipalias_show_commands) },
    { PARSE_ADD_CFG_RTG_IP_CMD, &pname(ipalias_config_commands) },
    { PARSE_LIST_END, NULL }
};


/*
 * ipalias_parser_init - Initialize IP Alias parser support
 */
void ipalias_parser_init (void)
{
    parser_add_command_list(ipalias_chain_init_table, "ipalias");
}
