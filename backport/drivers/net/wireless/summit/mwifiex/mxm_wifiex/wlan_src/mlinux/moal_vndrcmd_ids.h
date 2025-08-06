/** @file moal_vndrvcmds_id.h
 *
 * @brief This file contains the list of wiphy_vendor_comamnds commands.
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

#ifndef _MOAL_VNDRCMD_ID_H_
#define _MOAL_VNDRCMD_ID_H_
	//SONA command section
	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_MFG_START,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_mfg_start,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = VENDOR_CMD_RAW_DATA,
#endif
	},
	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_MFG_WRITE,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_mfg_write,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = VENDOR_CMD_RAW_DATA,
#endif
	},
	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_MFG_STOP,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_mfg_stop,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = VENDOR_CMD_RAW_DATA,
#endif
	},
	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_RADIO_ID,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_radio_id,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = radio_id_policy,
		.maxattr = ATTR_RID_MAX,
#endif
	},

	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_REG_TST,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_reg_tst,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = reg_tst_policy,
		.maxattr = ATTR_REG_MAX,
#endif
	},

	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_REG_TXC,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_reg_txc,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = reg_tst_policy,
		.maxattr = ATTR_REG_MAX,
#endif
	},

	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_REG_TX,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_reg_tx,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = reg_tst_policy,
		.maxattr = ATTR_REG_MAX,
#endif
	},

	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_REG_CW,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_reg_cw,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = reg_tst_policy,
		.maxattr = ATTR_REG_MAX,
#endif
	},

	{
		.info = {
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_REG_RX,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.doit  = woal_vndr_reg_rx,
#if KERNEL_VERSION(5, 3, 0) <= CFG80211_VERSION_CODE
		.policy = reg_tst_policy,
		.maxattr = ATTR_REG_MAX,
#endif
	},

#endif
