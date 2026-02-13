/** @file moal_vndr.h
 *
 * @brief This file contains vendor specific defines.
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

#ifndef _MOAL_VENDOR_CMD_H_
#define _MOAL_VENDOR_CMD_H_

#include "moal_main.h"

#define SONA_OUI 0xE8CBF5
#define PREFIX_1218 "1218"

#define VNDR_DISABLE_PHY_2G 1
#define VNDR_DISABLE_PHY_5G 2
#define VNDR_DISABLE_PHY_ALL (VNDR_DISABLE_PHY_2G | VNDR_DISABLE_PHY_5G)

enum sona_vendor_commands {
	SONA_CMD_RADIO_ID  = 1,
	SONA_CMD_MFG_START = 100,
	SONA_CMD_MFG_WRITE,
	SONA_CMD_MFG_STOP,
	SONA_CMD_REG_TST = 110,
	SONA_CMD_REG_TXC,
	SONA_CMD_REG_TX,
	SONA_CMD_REG_CW,
	SONA_CMD_REG_RX,
};

enum sona_attr_radio_id {
	ATTR_RID_FAMILY = 1,
	ATTR_RID_TYPE,
	ATTR_RID_FORM,
	ATTR_RID_WIFI_BUS,
	ATTR_RID_LAST,
	ATTR_RID_MAX = ATTR_RID_LAST-1,
};

#define RID_ATTR_FAMILY_NXP   3
#define RID_ATTR_TYPE_611     102
#define RID_ATTR_BUS_SDIO     1

typedef enum _attr_rid_form {
    RID_ATTR_FRM_SIP = 1, 
    RID_ATTR_FRM_SMT,
    RID_ATTR_FRM_SMT_CA,
    RID_ATTR_FRM_1216 = 100,
    RID_ATTR_FRM_2230,
    RID_ATTR_FRM_1218
} attr_rid_form;

static const struct nla_policy radio_id_policy[ATTR_RID_MAX + 1] = {
    [ATTR_RID_FAMILY]   = {.type = NLA_U32},
    [ATTR_RID_TYPE]     = {.type = NLA_U32},
    [ATTR_RID_FORM]     = {.type = NLA_U32},
    [ATTR_RID_WIFI_BUS] = {.type = NLA_U8},
};

enum sona_attr_reg {
	ATTR_REG_ENABLE = 1,
	ATTR_REG_CHANNEL,
	ATTR_REG_BW,
	ATTR_REG_RATE,
	ATTR_REG_PWR,
	ATTR_REG_IDX,
	ATTR_REG_LAST,
	ATTR_REG_MAX = ATTR_REG_LAST-1,
};

typedef enum _attr_reg_enable{
	REG_ATTR_EN_OFF = 0,
	REG_ATTR_EN_ON  = 1,
} attr_reg_en;

typedef enum _attr_reg_bw{
	REG_ATTR_BW_20 = 0,
	REG_ATTR_BW_40,
	REG_ATTR_BW_80,
} attr_reg_bw;

static const struct nla_policy reg_tst_policy[ATTR_REG_MAX + 1] = {
	[ATTR_REG_ENABLE]  = {.type = NLA_U32},
	[ATTR_REG_CHANNEL] = {.type = NLA_U32},
	[ATTR_REG_BW]      = {.type = NLA_U32},
	[ATTR_REG_RATE]    = {.type = NLA_U32},
	[ATTR_REG_PWR]     = {.type = NLA_S32},
	[ATTR_REG_IDX]     = {.type = NLA_U32},
};

int woal_vndr_radio_id (struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);
int woal_vndr_mfg_start(struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);
int woal_vndr_mfg_write(struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);
int woal_vndr_mfg_stop (struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);

int woal_vndr_reg_tst(struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);
int woal_vndr_reg_txc(struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);
int woal_vndr_reg_tx (struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);
int woal_vndr_reg_cw (struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);
int woal_vndr_reg_rx (struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int data_len);

int woal_vndr_get_radio_form(moal_handle *handle, attr_rid_form *form);

int woal_vndr_add_card_dpc(moal_handle *handle);
#endif
