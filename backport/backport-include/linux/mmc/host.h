#ifndef __BACKPORT_LINUX_MMC_HOST_H
#define __BACKPORT_LINUX_MMC_HOST_H

#include_next <linux/mmc/host.h>
#include <linux/version.h>
#include <linux/mmc/card.h>

#if LINUX_VERSION_IS_LESS(3,16,0)
#define mmc_card_hs LINUX_BACKPORT(mmc_card_hs)
static inline int mmc_card_hs(struct mmc_card *card)
{
	return card->host->ios.timing == MMC_TIMING_SD_HS ||
		card->host->ios.timing == MMC_TIMING_MMC_HS;
}
#endif /* < 3.16.0 */

#if LINUX_VERSION_IS_LESS(3,3,0) && !defined(mmc_card_uhs)
#ifdef mmc_sd_card_uhs
#define mmc_card_uhs(c) mmc_sd_card_uhs(c)
#else
#define mmc_card_uhs(c) false
#endif
#endif /* < 3.3.0 */

#endif /* __BACKPORT_LINUX_MMC_HOST_H */
