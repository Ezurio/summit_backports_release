/*
 * Copyright (C) 2018-2024 Ezurio
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

#ifndef _LRD_VENDOR_CMD_H_
#define _LRD_VENDOR_CMD_H_

#include <linux/netlink.h>
#include <net/genetlink.h>

#define LRD_OUI  0xC0EE40
#define SONA_OUI 0xE8CBF5

enum lrd_vendor_commands {
	LRD_VENDOR_CMD_MFG_START = 1,
	LRD_VENDOR_CMD_MFG_WRITE,
	LRD_VENDOR_CMD_MFG_STOP,
	LRD_VENDOR_CMD_LRU_START,
	LRD_VENDOR_CMD_LRU_WRITE,
	LRD_VENDOR_CMD_LRU_STOP,
	LRD_VENDOR_CMD_LRD_WRITE,
	LRD_VENDOR_CMD_GET_CHANNEL,
	LRD_VENDOR_CMD_MAX,
};

enum lrd_vendor_events {
	LRD_VENDOR_EVENT_RESTART = 0x8000,
};

enum lrd_nlattrs {
	LRD_ATTR_CMD_RSP = 1,    //BZ13280 Some Android NL libraries discard attributes that are 0
	LRD_ATTR_DATA,
	LRD_ATTR_RESTART_REASON,
	LRD_ATTR_MAX
};

enum lrd_attr_restart_reason {
	LRD_REASON_RESET = 1,
	LRD_REASON_RESUME
};

enum sona_vendor_commands {
	SONA_CMD_RADIO_ID  = 1,
};

enum attr_radio_id {
	ATTR_RID_FAMILY = 1,
	ATTR_RID_TYPE,
	ATTR_RID_FORM,
	ATTR_RID_WIFI_BUS,
	ATTR_RID_LAST,
	ATTR_RID_MAX = ATTR_RID_LAST -1,
};

#define RID_ATTR_FAMILY_NXP   3
#define RID_ATTR_TYPE_60      100

enum attr_rid_form {
    RID_ATTR_FRM_SIP = 1, 
    RID_ATTR_FRM_SMT,
    RID_ATTR_FRM_SMT_CA,
    RID_ATTR_FRM_1216 = 100,
    RID_ATTR_FRM_2230,
    RID_ATTR_FRM_1218,
};

enum attr_rid_bus {
	RID_ATTR_BUS_SDIO = 1,
	RID_ATTR_BUS_PCIE,
	RID_ATTR_BUS_USB,
};


void lrd_set_vendor_commands(struct wiphy *wiphy);
void lrd_set_vendor_events  (struct wiphy *wiphy);
#endif
