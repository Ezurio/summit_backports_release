#ifndef __BACKPORT_LINUX_MMC_CORE_H
#define __BACKPORT_LINUX_MMC_CORE_H
#include_next <linux/mmc/core.h>

#if LINUX_VERSION_IS_LESS(3,2,0)
static inline int mmc_hw_reset(struct mmc_card *card)
{
	return -EOPNOTSUPP;
}
#elif LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,2)
/* Do nothing, backported by RHEL */
#elif LINUX_VERSION_IS_LESS(5,18,0)
#define mmc_hw_reset(card) mmc_hw_reset(card->host)
#endif

#endif /* __BACKPORT_LINUX_MMC_CORE_H */
