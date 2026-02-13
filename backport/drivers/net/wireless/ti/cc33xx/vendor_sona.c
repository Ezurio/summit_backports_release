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

#include "vendor_sona.h"
#include "wlcore.h"

const struct nla_policy radio_id_policy[ATTR_RID_MAX + 1] = {
    [ATTR_RID_FAMILY]   = {.type = NLA_U32},
    [ATTR_RID_TYPE]     = {.type = NLA_U32},
    [ATTR_RID_FORM]     = {.type = NLA_U32},
    [ATTR_RID_WIFI_BUS] = {.type = NLA_U8},
};

static int 
sona_vendor_cmd_radio_id(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	struct ieee80211_hw  *hw = wiphy_to_ieee80211_hw(wiphy);
	struct cc33xx *wl = hw->priv;

	struct sk_buff            *skb = NULL;
	int                        ret = 0;

	if (!((wl->disable_5g == false) && (wl->disable_6g == true))) {
		wiphy_err(wiphy, "Unsupported chipset\n");
		ret = -EOPNOTSUPP;
		goto done;
	}

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, (sizeof(struct nlattr) + sizeof(u32)) * 4 );

	if (unlikely(!skb)) {
		ret = -ENOMEM;
		wiphy_err(wiphy,"skb alloc failed\n");
		goto done;
	}

	nla_put_u32(skb, ATTR_RID_FAMILY, RID_ATTR_FML_TI);
	nla_put_u32(skb, ATTR_RID_TYPE, RID_ATTR_TYPE_TI351);
	nla_put_u32(skb, ATTR_RID_WIFI_BUS, RID_ATTR_BUS_SDIO);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		wiphy_err(wiphy, "Vendor Command reply failed ret:%d\n", ret);

done:
	return ret;
}

static const struct wiphy_vendor_command sona_vendor_commands[] = {
	{
		{
			.vendor_id = SONA_OUI,
			.subcmd    = SONA_CMD_RADIO_ID,
		},
		.flags = WIPHY_VENDOR_CMD_NEED_WDEV |
			 WIPHY_VENDOR_CMD_NEED_NETDEV,
		.policy = radio_id_policy,
		.maxattr = ATTR_RID_MAX,
		.doit  = sona_vendor_cmd_radio_id,
	}
};


void sona_set_vendor_commands(struct wiphy *wiphy)
{
	wiphy->vendor_commands   = sona_vendor_commands;
	wiphy->n_vendor_commands = ARRAY_SIZE(sona_vendor_commands);
}

