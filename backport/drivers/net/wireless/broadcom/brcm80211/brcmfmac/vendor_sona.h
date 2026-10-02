/*
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

#ifndef _VENDOR_SONA_H_
#define _VENDOR_SONA_H_

#include <linux/netlink.h>
#include <net/genetlink.h>
#include <net/cfg80211.h>

#define SONA_OUI 0xE8CBF5

enum sona_vendor_commands {
	SONA_CMD_RADIO_ID = 1,
};

enum attr_radio_id {
	ATTR_RID_FAMILY = 1,
	ATTR_RID_TYPE,
	ATTR_RID_FORM,
	ATTR_RID_WIFI_BUS,
	ATTR_RID_LAST,
	ATTR_RID_MAX = ATTR_RID_LAST - 1,
};

enum attr_rid_family {
    RID_ATTR_FML_QCA = 1,
    RID_ATTR_FML_IFX,
    RID_ATTR_FML_NX,
    RID_ATTR_FML_TI,
};

enum attr_rid_type {
    RID_ATTR_TYPE_SU45 = 1,
    RID_ATTR_TYPE_SU50,
    RID_ATTR_TYPE_SU60 = 100,
    RID_ATTR_TYPE_SU8MP,
    RID_ATTR_TYPE_NX611,
    RID_ATTR_TYPE_LWB = 200,
    RID_ATTR_TYPE_LWB5,
    RID_ATTR_TYPE_LWBP,
    RID_ATTR_TYPE_LWB5P,
    RID_ATTR_TYPE_IF513,
    RID_ATTR_TYPE_IF573,
    RID_ATTR_TYPE_TI351 = 300,
};
	
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

extern const struct nla_policy radio_id_policy[ATTR_RID_MAX + 1];

extern int sona_vendor_cmd_radio_id(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len);

#endif
