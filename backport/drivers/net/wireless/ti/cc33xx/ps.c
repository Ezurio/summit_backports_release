// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of cc33xx
 *
 * Copyright (C) 2008-2009 Nokia Corporation
 *
 * Contact: Luciano Coelho <luciano.coelho@nokia.com>
 */

#include "ps.h"
#include "tx.h"
#include "debug.h"


int cc33xx_ps_set_mode(struct cc33xx *wl, struct cc33xx_vif *wlvif,
		       enum cc33xx_cmd_ps_mode_e mode)
{
	int ret;
	u16 timeout = wl->conf.host_conf.conn.dynamic_ps_timeout;

	switch (mode) {
	case STATION_AUTO_PS_MODE:
	case STATION_POWER_SAVE_MODE:
		cc33xx_debug(DEBUG_PSM, "entering psm (mode=%d,timeout=%u)",
			     mode, timeout);

		ret = cc33xx_cmd_ps_mode(wl, wlvif, mode, timeout);
		if (ret < 0)
			return ret;

		set_bit(WLVIF_FLAG_IN_PS, &wlvif->flags);

		break;

	case STATION_ACTIVE_MODE:
		cc33xx_debug(DEBUG_PSM, "leaving psm");

		ret = cc33xx_cmd_ps_mode(wl, wlvif, mode, 0);
		if (ret < 0)
			return ret;

		clear_bit(WLVIF_FLAG_IN_PS, &wlvif->flags);
		break;

	default:
		cc33xx_warning("trying to set ps to unsupported mode %d", mode);
		ret = -EINVAL;
	}

	return ret;
}

static void cc33xx_ps_filter_frames(struct cc33xx *wl, u8 hlid)
{
	int i;
	struct sk_buff *skb;
	struct ieee80211_tx_info *info;
	unsigned long flags;
	int filtered[NUM_TX_QUEUES];
	struct cc33xx_link *lnk = &wl->links[hlid];

	/* filter all frames currently in the low level queues for this hlid */
	for (i = 0; i < NUM_TX_QUEUES; i++) {
		filtered[i] = 0;
		while ((skb = skb_dequeue(&lnk->tx_queue[i]))) {
			filtered[i]++;

			if (WARN_ON(cc33xx_is_dummy_packet(wl, skb)))
				continue;

			info = IEEE80211_SKB_CB(skb);
			info->flags |= IEEE80211_TX_STAT_TX_FILTERED;
			info->status.rates[0].idx = -1;
			ieee80211_tx_status_ni(wl->hw, skb);
		}
	}

	spin_lock_irqsave(&wl->wl_lock, flags);
	for (i = 0; i < NUM_TX_QUEUES; i++) {
		wl->tx_queue_count[i] -= filtered[i];
		if (lnk->wlvif)
			lnk->wlvif->tx_queue_count[i] -= filtered[i];
	}

	spin_unlock_irqrestore(&wl->wl_lock, flags);
	cc33xx_handle_tx_low_watermark(wl);
}

void cc33xx_ps_link_start(struct cc33xx *wl, struct cc33xx_vif *wlvif,
			  u8 hlid, bool clean_queues)
{
	struct ieee80211_sta *sta;
	struct ieee80211_vif *vif = cc33xx_wlvif_to_vif(wlvif);

	if (WARN_ON_ONCE(wlvif->bss_type != BSS_TYPE_AP_BSS))
		return;

	if (!test_bit(hlid, wlvif->ap.sta_hlid_map) ||
	    test_bit(hlid, &wl->ap_ps_map))
		return;

	cc33xx_debug(DEBUG_PSM, 
		     "start mac80211 PSM on hlid %d pkts %d clean_queues %d",
		     hlid, wl->links[hlid].allocated_pkts, clean_queues);

	rcu_read_lock();
	sta = ieee80211_find_sta(vif, wl->links[hlid].addr);
	if (!sta) {
		cc33xx_error("could not find sta %pM for starting ps",
			     wl->links[hlid].addr);
		rcu_read_unlock();
		return;
	}

	ieee80211_sta_ps_transition_ni(sta, true);
	rcu_read_unlock();

	/* do we want to filter all frames from this link's queues? */
	if (clean_queues)
		cc33xx_ps_filter_frames(wl, hlid);

	__set_bit(hlid, &wl->ap_ps_map);
}
