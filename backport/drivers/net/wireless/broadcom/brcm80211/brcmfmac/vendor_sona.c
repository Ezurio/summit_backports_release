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
#include "cfg80211.h"
#include "bus.h"
#include "brcm_hw_ids.h"

const struct nla_policy radio_id_policy[ATTR_RID_MAX + 1] = {
    [ATTR_RID_FAMILY]   = {.type = NLA_U32},
    [ATTR_RID_TYPE]     = {.type = NLA_U32},
    [ATTR_RID_FORM]     = {.type = NLA_U32},
    [ATTR_RID_WIFI_BUS] = {.type = NLA_U8},
};

int 
sona_vendor_cmd_radio_id(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	struct brcmf_cfg80211_vif *vif;
	struct brcmf_if           *ifp;
	struct brcmf_pub          *drvr;

	struct sk_buff            *skb = NULL;
	int                        ret = 0;


	vif = container_of(wdev, struct brcmf_cfg80211_vif, wdev);
	ifp = vif->ifp;
	drvr = ifp->drvr;

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, (sizeof(struct nlattr) + sizeof(u32)) * 4 );

	if (unlikely(!skb)) {
		ret = -ENOMEM;
		wiphy_err(wiphy,"skb alloc failed\n");
		goto done;
	}

	nla_put_u32(skb, ATTR_RID_FAMILY, RID_ATTR_FML_IFX);

	switch (drvr->bus_if->chip) {
	case CY_CC_55572_CHIP_ID:
		nla_put_u32(skb, ATTR_RID_TYPE, RID_ATTR_TYPE_IF573);
		break;
	case CY_CC_55500_CHIP_ID:
		nla_put_u32(skb, ATTR_RID_TYPE, RID_ATTR_TYPE_IF513);
		break;
	case CY_CC_4373_CHIP_ID:
		nla_put_u32(skb, ATTR_RID_TYPE, RID_ATTR_TYPE_LWB5P);
		break;
	case CY_CC_43439_CHIP_ID:
		nla_put_u32(skb, ATTR_RID_TYPE, RID_ATTR_TYPE_LWBP);
		break;
	case BRCM_CC_43430_CHIP_ID:
		nla_put_u32(skb, ATTR_RID_TYPE, RID_ATTR_TYPE_LWB);
		break;
	default:
		break;
	}

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		wiphy_err(wiphy, "Vendor Command reply failed ret:%d\n", ret);

done:
	return ret;
}
