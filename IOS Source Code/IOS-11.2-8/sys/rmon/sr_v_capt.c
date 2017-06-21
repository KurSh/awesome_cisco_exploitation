/************************************************************************
 *                                                                      *
 *                              NOTE WELL                               *
 * This is vendor-supplied and vendor-supported code.  Do not make any  *
 * modifications to this code without the knowledge and consent of the  *
 * SNMP agent group.                                                    *
 *                                                                      *
 ************************************************************************/
/* $Id: sr_v_capt.c,v 3.2 1995/11/17 18:39:58 hampton Exp $
 * $Source: /swtal/cherf.111/ios/sys/rmon/sr_v_capt.c,v $
 *------------------------------------------------------------------
 * Remote Monitoring MIB Support
 *
 * August 1995, SNMP Research
 *
 * Copyright (c) 1995 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: sr_v_capt.c,v $
 * Revision 3.2  1995/11/17  18:39:58  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:03:40  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 *
 * Copyright (C) 1994 by SNMP Research, Incorporated.
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

/*
 *                PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 *
 */


#include "master.h"
#include "sr_rmonmib.h"

#include "../snmp/sr_diag.h"


#define MAXBUFSIZE 32

/*---------------------------------------------------------------------
 * Retrieve data from the bufferControlEntry family. This is
 * performed in 3 steps:
 *
 *   1) Test the validity of the object instance.
 *   2) Retrieve the data.
 *   3) Build the variable binding (VarBind) that will be returned.
 *---------------------------------------------------------------------*/
VarBind        *
bufferControlEntry_get(incoming, object, searchType, contextInfo, serialNum)
    OID            *incoming;
    ObjectInfo     *object;
    int             searchType;
    ContextInfo    *contextInfo;
    int             serialNum;
{
    int             instLength = incoming->length - object->oid.length;
    int             arg = object->nominator;
    void           *dp;
    int             carry;
    long            bufferControlIndex;

#ifdef bufferControlEntry_DISABLE_CACHE
    bufferControlEntry_t *data;
    unsigned long   buffer[1];
    OID             inst;
#else				/* bufferControlEntry_DISABLE_CACHE */
    static bufferControlEntry_t *data;
    static unsigned long buffer[1];
    static OID      inst;
    static int      last_serialNum;
    static OID     *last_incoming;

    /* if the cache is not valid */
    if ((serialNum != last_serialNum) || (serialNum == -1) || (data == NULL) ||
	(CmpOIDInst(incoming, last_incoming, object->oid.length) != 0) ||
	(!NEXT_ROW(data->valid)) ||
	(!VALID(object->nominator, data->valid))) {
#endif				/* bufferControlEntry_DISABLE_CACHE */

	/*
	 * Check the object instance.
	 * 
	 * An EXACT search requires that the instance be of length 1
	 * 
	 * A NEXT search requires that the requested object does not
	 * lexicographically precede the current object type.
	 */

	if (searchType == EXACT) {
	    if (instLength != 1) {
		return ((VarBind *) NULL);
	    }
	    carry = 0;
	}
	else {
	    carry = 1;
	}

	if ((InstToInt(incoming, 0 + object->oid.length, &bufferControlIndex, searchType, &carry)) < 0) {
	    arg = -1;
	}
	if (searchType == NEXT) {
	    bufferControlIndex = MAX(1, bufferControlIndex);
	}

	if (carry) {
	    arg = -1;
	}

	/*
	 * Retrieve the data from the kernel-specific routine.
	 */
	if ((arg == -1) || (data = k_bufferControlEntry_get(serialNum, contextInfo, arg, searchType, bufferControlIndex)) == NULL) {
	    arg = -1;
	}

	else {
	    /*
	     * Build instance information
	     */
	    inst.oid_ptr = buffer;
	    inst.length = 1;
	    inst.oid_ptr[0] = data->bufferControlIndex;
	}

#ifndef bufferControlEntry_DISABLE_CACHE
	if (arg == -1) {
	    data = NULL;
	}
	else {
	    last_serialNum = serialNum;
	    if (last_incoming != NULL) {
		FreeOID(last_incoming);
	    }
	    last_incoming = CloneOID(incoming);
	}
    }
#endif				/* bufferControlEntry_DISABLE_CACHE */

    /*
     * Build the variable binding for the variable that will be returned.
     */

    switch (arg) {

#ifdef I_bufferControlIndex
      case I_bufferControlIndex:
	dp = &data->bufferControlIndex;
	break;
#endif				/* I_bufferControlIndex */

#ifdef I_bufferControlChannelIndex
      case I_bufferControlChannelIndex:
	dp = &data->bufferControlChannelIndex;
	break;
#endif				/* I_bufferControlChannelIndex */

#ifdef I_bufferControlFullStatus
      case I_bufferControlFullStatus:
	dp = &data->bufferControlFullStatus;
	break;
#endif				/* I_bufferControlFullStatus */

#ifdef I_bufferControlFullAction
      case I_bufferControlFullAction:
	dp = &data->bufferControlFullAction;
	break;
#endif				/* I_bufferControlFullAction */

#ifdef I_bufferControlCaptureSliceSize
      case I_bufferControlCaptureSliceSize:
	dp = &data->bufferControlCaptureSliceSize;
	break;
#endif				/* I_bufferControlCaptureSliceSize */

#ifdef I_bufferControlDownloadSliceSize
      case I_bufferControlDownloadSliceSize:
	dp = &data->bufferControlDownloadSliceSize;
	break;
#endif				/* I_bufferControlDownloadSliceSize */

#ifdef I_bufferControlDownloadOffset
      case I_bufferControlDownloadOffset:
	dp = &data->bufferControlDownloadOffset;
	break;
#endif				/* I_bufferControlDownloadOffset */

#ifdef I_bufferControlMaxOctetsRequested
      case I_bufferControlMaxOctetsRequested:
	dp = &data->bufferControlMaxOctetsRequested;
	break;
#endif				/* I_bufferControlMaxOctetsRequested */

#ifdef I_bufferControlMaxOctetsGranted
      case I_bufferControlMaxOctetsGranted:
	dp = &data->bufferControlMaxOctetsGranted;
	break;
#endif				/* I_bufferControlMaxOctetsGranted */

#ifdef I_bufferControlCapturedPackets
      case I_bufferControlCapturedPackets:
	dp = &data->bufferControlCapturedPackets;
	break;
#endif				/* I_bufferControlCapturedPackets */

#ifdef I_bufferControlTurnOnTime
      case I_bufferControlTurnOnTime:
	dp = &data->bufferControlTurnOnTime;
	break;
#endif				/* I_bufferControlTurnOnTime */

#ifdef I_bufferControlOwner
      case I_bufferControlOwner:
	dp = MakeOctetString(data->bufferControlOwner->octet_ptr, data->bufferControlOwner->length);
	break;
#endif				/* I_bufferControlOwner */

#ifdef I_bufferControlStatus
      case I_bufferControlStatus:
	dp = &data->bufferControlStatus;
	break;
#endif				/* I_bufferControlStatus */

      default:
	return ((VarBind *) NULL);

    }				/* switch */

    return (MakeVarBind(object, &inst, dp));

}

#ifdef SETS
/*----------------------------------------------------------------------
 * Free the bufferControlEntry data object.
 *---------------------------------------------------------------------*/
void
bufferControlEntry_free(data)
    bufferControlEntry_t *data;
{
    if (data != NULL) {
	k_bufferControlEntryFreeUserpartData(data);
	FreeOctetString(data->bufferControlOwner);

	free((char *) data);
    }
}

/*----------------------------------------------------------------------
 * cleanup after bufferControlEntry set/undo
 *---------------------------------------------------------------------*/
static int
bufferControlEntry_cleanup(
    doList_t       *trash)
{
    bufferControlEntry_free(trash->data);

#ifdef SR_SNMPv2
    bufferControlEntry_free(trash->undodata);
#endif				/* SR_SNMPv2 */

    return NO_ERROR;
}

/*---------------------------------------------------------------------
 * Determine if this SET request is valid. If so, add it to the do-list.
 * This operation is performed in 4 steps:
 *
 *   1) Validate the object instance.
 *   2) Locate a "related" do-list element, or create a new do-list
 *      element.
 *   3) Add the SET request to the selected do-list element.
 *   4) Finally, determine if the do-list element (all "related" set
 *      requests) is completely valid.
 *---------------------------------------------------------------------*/
int
bufferControlEntry_test(incoming, object, value, doHead, doCur, contextInfo)
    OID            *incoming;
    ObjectInfo     *object;
    ObjectSyntax   *value;
    doList_t       *doHead;
    doList_t       *doCur;
    ContextInfo    *contextInfo;
{
    int             instLength = incoming->length - object->oid.length;
    doList_t       *dp;
    int             found;
    int             error_status;
    int             carry = 0;
    bufferControlEntry_t *bufferControlEntry;
    long            bufferControlIndex;

    /*
     * Validate the object instance
     * 
     */
    if (instLength != 1) {
	return NO_CREATION_ERROR;
    }

    if ((InstToInt(incoming, 0 + object->oid.length, &bufferControlIndex, EXACT, &carry)) < 0) {
	return (NO_CREATION_ERROR);
    }

    bufferControlEntry = k_bufferControlEntry_get(-1, contextInfo, object->nominator, EXACT, bufferControlIndex);

    /*
     * Determine the element of the do-list which should contain this SET
     * request. This is decided by looking for a previous do-list element for
     * the same row.
     */
    found = 0;

    for (dp = doHead; dp != NULL; dp = dp->next) {
	if ((dp->setMethod == bufferControlEntry_set) &&
	    (((bufferControlEntry_t *) (dp->data)) != NULL) &&
	    (((bufferControlEntry_t *) (dp->data))->bufferControlIndex == bufferControlIndex)) {

	    found = 1;
	    break;
	}
    }

    if (!found) {
	dp = doCur;
	if ((dp->data = (void *) malloc(sizeof(bufferControlEntry_t))) == NULL) {
	    return RESOURCE_UNAVAILABLE_ERROR;
	}
	memset(dp->data, 0, sizeof(bufferControlEntry_t));

	dp->setMethod = bufferControlEntry_set;
	dp->cleanupMethod = bufferControlEntry_cleanup;

#ifdef SR_SNMPv2
	dp->undoMethod = bufferControlEntry_undo;
#endif				/* SR_SNMPv2 */

	dp->state = UNKNOWN;

	if (bufferControlEntry != NULL) {

#ifdef SR_SNMPv2
	    if ((dp->undodata = (void *) malloc(sizeof(bufferControlEntry_t))) == NULL) {
		return RESOURCE_UNAVAILABLE_ERROR;
	    }
#endif				/* SR_SNMPv2 */

	    /* fill in existing values */
	    memcpy((char *) (dp->data), (char *) (bufferControlEntry), sizeof(bufferControlEntry_t));

#ifdef SR_SNMPv2
	    memcpy((char *) (dp->undodata), (char *) (bufferControlEntry), sizeof(bufferControlEntry_t));
#endif				/* SR_SNMPv2 */

	    SET_ALL_VALID(((bufferControlEntry_t *) (dp->data))->valid);

	    ((bufferControlEntry_t *) (dp->data))->bufferControlOwner =
		CloneOctetString(bufferControlEntry->bufferControlOwner);

#ifdef SR_SNMPv2
	    ((bufferControlEntry_t *) (dp->undodata))->bufferControlOwner =
		CloneOctetString(bufferControlEntry->bufferControlOwner);
#endif				/* SR_SNMPv2 */

	}
	else {

#ifdef bufferControlEntry_READ_CREATE
	    /* Try to fill in reasonable default values for this new entry */
	    ((bufferControlEntry_t *) (dp->data))->bufferControlIndex = bufferControlIndex;
	    SET_VALID(I_bufferControlIndex, ((bufferControlEntry_t *) (dp->data))->valid);

	    error_status = k_bufferControlEntry_set_defaults(dp);
	    if (error_status != NO_ERROR) {
		return error_status;
	    }
#else				/* bufferControlEntry_READ_CREATE */
	    return NO_ACCESS_ERROR;
#endif				/* bufferControlEntry_READ_CREATE */
	}
    }

    switch (object->nominator) {

#ifdef I_bufferControlChannelIndex
      case I_bufferControlChannelIndex:

	if (((value->sl_value < 1) || (value->sl_value > 65535))) {
	    return WRONG_VALUE_ERROR;
	}

	((bufferControlEntry_t *) (dp->data))->bufferControlChannelIndex = value->sl_value;
	break;
#endif				/* I_bufferControlChannelIndex */

#ifdef I_bufferControlFullAction
      case I_bufferControlFullAction:

	if ((value->sl_value < 1) || (value->sl_value > 2)) {
	    return WRONG_VALUE_ERROR;
	}

	((bufferControlEntry_t *) (dp->data))->bufferControlFullAction = value->sl_value;
	break;
#endif				/* I_bufferControlFullAction */

#ifdef I_bufferControlCaptureSliceSize
      case I_bufferControlCaptureSliceSize:

	((bufferControlEntry_t *) (dp->data))->bufferControlCaptureSliceSize = value->sl_value;
	break;
#endif				/* I_bufferControlCaptureSliceSize */

#ifdef I_bufferControlDownloadSliceSize
      case I_bufferControlDownloadSliceSize:

	((bufferControlEntry_t *) (dp->data))->bufferControlDownloadSliceSize = value->sl_value;
	break;
#endif				/* I_bufferControlDownloadSliceSize */

#ifdef I_bufferControlDownloadOffset
      case I_bufferControlDownloadOffset:

	((bufferControlEntry_t *) (dp->data))->bufferControlDownloadOffset = value->sl_value;
	break;
#endif				/* I_bufferControlDownloadOffset */

#ifdef I_bufferControlMaxOctetsRequested
      case I_bufferControlMaxOctetsRequested:

	((bufferControlEntry_t *) (dp->data))->bufferControlMaxOctetsRequested = value->sl_value;
	break;
#endif				/* I_bufferControlMaxOctetsRequested */

#ifdef I_bufferControlOwner
      case I_bufferControlOwner:

	if (((bufferControlEntry_t *) (dp->data))->bufferControlOwner != NULL) {
	    FreeOctetString(((bufferControlEntry_t *) (dp->data))->bufferControlOwner);
	}

	((bufferControlEntry_t *) (dp->data))->bufferControlOwner =
	    MakeOctetString(value->os_value->octet_ptr, value->os_value->length);

	break;
#endif				/* I_bufferControlOwner */

#ifdef I_bufferControlStatus
      case I_bufferControlStatus:

	if ((value->sl_value < 1) || (value->sl_value > 4)) {
	    return WRONG_VALUE_ERROR;
	}

	error_status = CheckEntryStatus(value->sl_value,
					bufferControlEntry == NULL ? 0L : bufferControlEntry->bufferControlStatus);
	if (error_status != 0)
	    return (error_status);

	((bufferControlEntry_t *) (dp->data))->bufferControlStatus = value->sl_value;
	break;
#endif				/* I_bufferControlStatus */

      default:
	DPRINTF((APALWAYS, "snmpd: Internal error (invalid nominator in bufferControlEntry_test)\n"));
	return GEN_ERROR;

    }				/* switch */

    /* Do system dependent testing in k_bufferControlEntry_test */
    error_status = k_bufferControlEntry_test(object, value, dp, contextInfo);

    if (error_status == NO_ERROR) {
	SET_VALID(object->nominator, ((bufferControlEntry_t *) (dp->data))->valid);
	error_status = k_bufferControlEntry_ready(object, value, doHead, dp);
    }

    return (error_status);
}

/*---------------------------------------------------------------------
 * Perform the kernel-specific set function for this group of
 * related objects.
 *---------------------------------------------------------------------*/
int
bufferControlEntry_set(doHead, doCur, contextInfo)
    doList_t       *doHead;
    doList_t       *doCur;
    ContextInfo    *contextInfo;
{
    return (k_bufferControlEntry_set((bufferControlEntry_t *) (doCur->data),
				     contextInfo, doCur->state));
}

#endif				/* SETS */


/*---------------------------------------------------------------------
 * Retrieve data from the captureBufferEntry family. This is
 * performed in 3 steps:
 *
 *   1) Test the validity of the object instance.
 *   2) Retrieve the data.
 *   3) Build the variable binding (VarBind) that will be returned.
 *---------------------------------------------------------------------*/
VarBind        *
captureBufferEntry_get(incoming, object, searchType, contextInfo, serialNum)
    OID            *incoming;
    ObjectInfo     *object;
    int             searchType;
    ContextInfo    *contextInfo;
    int             serialNum;
{
    int             instLength = incoming->length - object->oid.length;
    int             arg = object->nominator;
    void           *dp;
    int             carry;
    long            captureBufferControlIndex;
    long            captureBufferIndex;

#ifdef captureBufferEntry_DISABLE_CACHE
    captureBufferEntry_t *data;
    unsigned long   buffer[2];
    OID             inst;
#else				/* captureBufferEntry_DISABLE_CACHE */
    static captureBufferEntry_t *data;
    static unsigned long buffer[2];
    static OID      inst;
    static int      last_serialNum;
    static OID     *last_incoming;

    /* if the cache is not valid */
    if ((serialNum != last_serialNum) || (serialNum == -1) || (data == NULL) ||
	(CmpOIDInst(incoming, last_incoming, object->oid.length) != 0) ||
	(!NEXT_ROW(data->valid)) ||
	(!VALID(object->nominator, data->valid))) {
#endif				/* captureBufferEntry_DISABLE_CACHE */

	/*
	 * Check the object instance.
	 * 
	 * An EXACT search requires that the instance be of length 2
	 * 
	 * A NEXT search requires that the requested object does not
	 * lexicographically precede the current object type.
	 */

	if (searchType == EXACT) {
	    if (instLength != 2) {
		return ((VarBind *) NULL);
	    }
	    carry = 0;
	}
	else {
	    carry = 1;
	}

	if ((InstToInt(incoming, 1 + object->oid.length, &captureBufferIndex, searchType, &carry)) < 0) {
	    arg = -1;
	}

	if ((InstToInt(incoming, 0 + object->oid.length, &captureBufferControlIndex, searchType, &carry)) < 0) {
	    arg = -1;
	}
	if (searchType == NEXT) {
	    captureBufferControlIndex = MAX(1, captureBufferControlIndex);
	}

	if (carry) {
	    arg = -1;
	}

	/*
	 * Retrieve the data from the kernel-specific routine.
	 */
	if ((arg == -1) || (data = k_captureBufferEntry_get(serialNum, contextInfo, arg, searchType, captureBufferControlIndex, captureBufferIndex)) == NULL) {
	    arg = -1;
	}

	else {
	    /*
	     * Build instance information
	     */
	    inst.oid_ptr = buffer;
	    inst.length = 2;
	    inst.oid_ptr[0] = data->captureBufferControlIndex;
	    inst.oid_ptr[1] = data->captureBufferIndex;
	}

#ifndef captureBufferEntry_DISABLE_CACHE
	if (arg == -1) {
	    data = NULL;
	}
	else {
	    last_serialNum = serialNum;
	    if (last_incoming != NULL) {
		FreeOID(last_incoming);
	    }
	    last_incoming = CloneOID(incoming);
	}
    }
#endif				/* captureBufferEntry_DISABLE_CACHE */

    /*
     * Build the variable binding for the variable that will be returned.
     */

    switch (arg) {

#ifdef I_captureBufferControlIndex
      case I_captureBufferControlIndex:
	dp = &data->captureBufferControlIndex;
	break;
#endif				/* I_captureBufferControlIndex */

#ifdef I_captureBufferIndex
      case I_captureBufferIndex:
	dp = &data->captureBufferIndex;
	break;
#endif				/* I_captureBufferIndex */

#ifdef I_captureBufferPacketID
      case I_captureBufferPacketID:
	dp = &data->captureBufferPacketID;
	break;
#endif				/* I_captureBufferPacketID */

#ifdef I_captureBufferPacketData
      case I_captureBufferPacketData:
	dp = MakeOctetString(data->captureBufferPacketData->octet_ptr, data->captureBufferPacketData->length);
	break;
#endif				/* I_captureBufferPacketData */

#ifdef I_captureBufferPacketLength
      case I_captureBufferPacketLength:
	dp = &data->captureBufferPacketLength;
	break;
#endif				/* I_captureBufferPacketLength */

#ifdef I_captureBufferPacketTime
      case I_captureBufferPacketTime:
	dp = &data->captureBufferPacketTime;
	break;
#endif				/* I_captureBufferPacketTime */

#ifdef I_captureBufferPacketStatus
      case I_captureBufferPacketStatus:
	dp = &data->captureBufferPacketStatus;
	break;
#endif				/* I_captureBufferPacketStatus */

      default:
	return ((VarBind *) NULL);

    }				/* switch */

    return (MakeVarBind(object, &inst, dp));

}
