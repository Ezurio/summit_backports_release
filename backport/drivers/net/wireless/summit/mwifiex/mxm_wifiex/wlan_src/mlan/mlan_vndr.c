/** @file mlan_vndr.c
 *
 * @brief This file contains functions for vendor commands.
 *
 * Copyright (C) 2025 Ezurio
 *
 * This software file (the "File") is distributed by Ezurio
 * under the terms of the GNU General Public License Version 2, June 1991
 * (the "License").  You may use, redistribute and/or modify this File in
 * accordance with the terms and conditions of the License, a copy of which
 * is available by writing to the Free Software Foundation, Inc.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
 * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
 * this warranty disclaimer.
  */

/********************************************************
Change log:
    4/7/2025: initial version
********************************************************/
#include "mlan.h"
#include "mlan_util.h"
#include "mlan_decl.h"
#include "mlan_fw.h"
#include "mlan_join.h"
#include "mlan_ioctl.h"
#include "mlan_vndr.h"

typedef enum _vndr_mfg_oids {
	MFG_OID_START = 0,
	MFG_OID_WRITE,
	MFG_OID_STOP,
} vndr_mfg_oids;

static mlan_status wlan_vndr_ioctl_mfg(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req);

/**
 *  @brief This function handles the command response of Vendor Manufacturing commands.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND structure
 *  @param pioctl_buf   A pointer to mlan_ioctl_req buf
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_vndr_mfg_cmd(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
	pmlan_adapter pmadapter = pmpriv->adapter;
	HostCmd_VNDR_MFG_CMD *mfg_cmd = &resp->params.host_vndr_mfg;
	mlan_vndr_cmd *vndr_cmd = (mlan_vndr_cmd *)pioctl_buf->pbuf;
	
	ENTER();

	switch (mfg_cmd->oid) {
		case MFG_OID_START:
			memcpy_ext(pmadapter, vndr_cmd->param.mfg_start.data, (t_u8*) mfg_cmd->data, 
				       sizeof(t_u32), sizeof(vndr_cmd->param.mfg_start.data));
		break;
		case MFG_OID_WRITE:
		break;	
		case MFG_OID_STOP:
		break;
	}

	LEAVE();
	return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares manufacturing commands for fw consumption
 *
 *  @param pmpriv      A pointer to mlan_private structure
 *  @param cmd         A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action  Action: GET or SET
 *  @param pdata_buf   A pointer to data buffer
 *
 *  @return            MLAN_STATUS_SUCCESS, or MLAN_STATUS_FAILURE
 */
mlan_status wlan_cmd_vndr_mfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd,
	t_u16 cmd_action, t_u32 cmd_oid, t_void *pdata_buf)
{
	pmlan_adapter pmadapter = pmpriv->adapter;
	HostCmd_VNDR_MFG_CMD *cmd_mfg = &cmd->params.host_vndr_mfg;

	ENTER();

	cmd->command = wlan_cpu_to_le16(HostCmd_CMD_VNDR_MFG_CMD);
	switch (cmd_oid) {
	case MFG_OID_START:
		cmd_mfg->oid = wlan_cpu_to_le32(MFG_OID_START);
		cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_VNDR_MFG_CMD) + S_DS_GEN);
		break;
	case MFG_OID_STOP:
		cmd_mfg->oid = wlan_cpu_to_le32(MFG_OID_STOP);
		cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_VNDR_MFG_CMD) + S_DS_GEN);
		break;
	case MFG_OID_WRITE: {
		mlan_vndr_mfg_write *mfg_write = (mlan_vndr_mfg_write*)pdata_buf;
		cmd_mfg->oid = wlan_cpu_to_le32(MFG_OID_WRITE);
		cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_VNDR_MFG_CMD) + mfg_write->data_len + S_DS_GEN);
		memcpy_ext(pmadapter, cmd_mfg->data, mfg_write->data,  mfg_write->data_len, mfg_write->data_len);
		}
		break;
	default:
		return MLAN_STATUS_FAILURE;
		break;
	}

	LEAVE();
	return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Handles Vnedor Manufacturing Command Requests
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_vndr_ioctl_mfg(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
	mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
	mlan_status status   = MLAN_STATUS_SUCCESS;
	mlan_vndr_cmd *vndr_cmd = MNULL;

	ENTER();

	vndr_cmd = (mlan_vndr_cmd *)pioctl_req->pbuf;

	if (pioctl_req->action == MLAN_ACT_GET) {
		PRINTM(MCMND, "Ignore Mfg Command 0x%02x get request.\n", vndr_cmd->sub_command);
		LEAVE();
		return MLAN_STATUS_FAILURE;
	}

	switch(vndr_cmd->sub_command) {
	case  MLAN_OID_VNDR_MFG_START:
		status = wlan_prepare_cmd(pmpriv, HostCmd_CMD_VNDR_MFG_CMD,  HostCmd_ACT_GEN_SET, 
					       MFG_OID_START, (t_void *)pioctl_req, MNULL);
		break;
	case MLAN_OID_VNDR_MFG_WRITE:
		status = wlan_prepare_cmd(pmpriv, HostCmd_CMD_VNDR_MFG_CMD,  HostCmd_ACT_GEN_SET, 
					       MFG_OID_WRITE, (t_void *)pioctl_req, (t_void*)&vndr_cmd->param.mfg_write);
		break;
	case MLAN_OID_VNDR_MFG_STOP:
		status = wlan_prepare_cmd(pmpriv, HostCmd_CMD_VNDR_MFG_CMD,  HostCmd_ACT_GEN_SET, 
					       MFG_OID_STOP, (t_void *)pioctl_req, MNULL);
		break;
	default:
		pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
		status = MLAN_STATUS_FAILURE;
		break;
	}

	if (status == MLAN_STATUS_SUCCESS)
		status = MLAN_STATUS_PENDING;

	LEAVE();
	return status;
}

/**
 *  @brief Handles Vendor IOCTl requests
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_vndr_ioctl(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
	mlan_status status = MLAN_STATUS_SUCCESS;
	mlan_vndr_cmd *vndr_cmd = MNULL;

	ENTER();

	if (pioctl_req->buf_len < sizeof(mlan_vndr_cmd)) {
		PRINTM(MWARN,
		       "MLAN IOCTL information buffer length is too short.\n");
		pioctl_req->data_read_written = 0;
		pioctl_req->buf_len_needed = sizeof(mlan_vndr_cmd);
		pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
		LEAVE();
		return MLAN_STATUS_RESOURCE;
	}

	vndr_cmd = (mlan_vndr_cmd *)pioctl_req->pbuf;

	switch (vndr_cmd->sub_command) {
	case MLAN_OID_VNDR_MFG_START:
	case MLAN_OID_VNDR_MFG_WRITE:
	case MLAN_OID_VNDR_MFG_STOP:
		status = wlan_vndr_ioctl_mfg(pmadapter, pioctl_req);
		break;
	default:
		pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
		status = MLAN_STATUS_FAILURE;
		break;
	}

	LEAVE();
	return status;
}



