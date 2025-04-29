#ifndef __BACKPORT_CHECKS
#define __BACKPORT_CHECKS

#if defined(CONFIG_MAC80211) && defined(CPTCFG_MAC80211_MODULE)
#error "You must not have mac80211 built into your kernel if you want to enable it"
#endif

#if defined(CONFIG_CFG80211) && defined(CPTCFG_CFG80211_MODULE)
#error "You must not have cfg80211 built into your kernel if you want to enable it"
#endif

#if defined(CONFIG_BT) && defined(CPTCFG_BT_MODULE)
#error "You must not have Bluetooth built into your kernel if you want to enable it"
#endif

#if defined(CONFIG_BRCMFMAC) && defined(CPTCFG_BRCMFMAC_MODULE)
#error "You must not have brcmfmac built into your kernel if you want to enable it"
#endif

#if defined(CONFIG_ATH6KL) && defined(CPTCFG_ATH6KL_MODULE)
#error "You must not have ath6kl built into your kernel if you want to enable it"
#endif

#endif /* __BACKPORT_CHECKS */
