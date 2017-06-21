/* $Id: x25_sw_chain.c,v 3.1.2.2 1996/05/21 10:10:44 thille Exp $
 * $Source: /release/112/cvs/Xsys/x25/x25_sw_chain.c,v $
 *------------------------------------------------------------------
 * X.25 switch client parser chains
 *
 * April 1996, Gilbert Glick
 *
 * Copyright (c) 1996 by cisco Systems, Inc.
 * All rights reserved.
 *
 *------------------------------------------------------------------
 * $Log: x25_sw_chain.c,v $
 * Revision 3.1.2.2  1996/05/21  10:10:44  thille
 * CSCdi51599:  multiple /interface/ literals bloat code.
 * Branch: California_branch
 * Save another 392 bytes by doing away with duplicate literals.
 *
 * Revision 3.1.2.1  1996/04/11  23:47:43  gglick
 * CSCdi54514:  X.25 parser modularity files
 * Branch: California_branch
 *
 * Revision 3.1  1996/04/11  23:40:04  gglick
 * Initial commit of X.25 parser modularization files.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

#include "master.h"
#include "../ui/common_strings.h"
#include "interface_private.h"
#include "config.h"
#include "parser.h"
#include "registry.h"
#include "media_registry.h"
#include "../parser/parser_actions.h"
#include "../parser/macros.h"
#include "../parser/parser_defs_parser.h"
#include "parser_defs_x25.h"
#include "ll_ul.h"
#include "x25_library.h"
#include "x25_packet.h"
#include "x25_registry.h"


/*
 * Parse chain additions to X.25 PVC configuration commands
 */
#define	ALTERNATE	NONE
#include "cfg_int_x25_sw.h"
LINK_POINT(x25sw_pvc_commands, ALTERNATE);
#undef	ALTERNATE


/*
 * Parse chain registration array for X25
 */
static parser_extension_request x25sw_chain_init_table[] = {
    { PARSE_ADD_CFG_INT_X25_MAPPVC_CMD,
	  (dynamic_transition *) &pname(x25sw_pvc_commands) },
    { PARSE_ADD_CFG_INT_X25_MAPPVC_EXIT,
	  (dynamic_transition *) &pname(ci_x25sw_pvc_exit) },
    { PARSE_LIST_END, NULL }
};


/*
 * x25sw_parser_init
 * Install the X.25 switch client parse chains
 */
void x25sw_parser_init (void)
{
    /*
     *  add the X.25 switch client chains to the parser
     */
    parser_add_command_list(x25sw_chain_init_table, "x25_switch");
}
