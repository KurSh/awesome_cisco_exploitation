/* $Id: dnld.h,v 3.2 1995/11/17 18:41:38 hampton Exp $
 * $Source: /swtal/cherf.111/ios/sys/rommon/src/dnld.h,v $
 *------------------------------------------------------------------
 * dnld.h
 *
 * June 1993, Rob Clevenger
 *
 * Copyright (c) 1994 by cisco Systems, Inc.
 * All rights reserved.
 *
 * Defines for the binary serial downloader.
 *------------------------------------------------------------------
 * $Log: dnld.h,v $
 * Revision 3.2  1995/11/17  18:41:38  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:05:16  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/06/07  22:37:08  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */


#ifndef __DNLD_H__
#define __DNLD_H__

/*
** Defines for the download mechanism
*/

/*
** The following are the target handshake characters.
** The target sends the appropriate char to the host to confirm
** reciept of a packet or to inform of other status.
*/
#define DNLD_HNDSHK_START    '~'  /* target start character */
#define DNLD_HNDSHK_ACK      '+'  /* packet recieved ok */
#define DNLD_HNDSHK_RESEND   '-'  /* resend a packet */
#define DNLD_HNDSHK_ABORT    '!'  /* abort the transmission */

/* general defines */
#define DNLD_MAXRETRIES      5    /* packet retry count */
#define DNLD_MAXPACKET       256  /* maximum packet size */

/* flag for datacount field to indicate 32 bit address field */
#define DNLD_32BITFLG        0x8000

/* host sends to target in address field of termination record to abort */
#define DNLD_ABORT_ADDR      -1

/*
** defines for the host side state machine
*/
typedef enum {
    DNLD_IDLE,         /* not doing a download */
    DNLD_STARTWAIT,    /* waiting for a start from the target */
    DNLD_ACKWAIT,      /* waiting for an ack from the target */
    DNLD_SENDPKT,      /* send the packet in the filebuf */
    DNLD_SENDNEXTPKT,  /* read and send the next packet */
    DNLD_SENDLASTPKT,  /* send this packet and quit */
    DNLD_LASTACKWAIT,  /* wait for last ack */
} DNLD_STATE;

#ifndef UNIX
/* dnld.c */
extern int dnld(int argc, char *argv[]);
extern int download(char *progname, int verbose);
#endif

#endif   /* __DNLD_H__ */
