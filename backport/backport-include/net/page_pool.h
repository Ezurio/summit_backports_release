#ifndef _BACKPORT_NET_PAGE_POOL_H
#define _BACKPORT_NET_PAGE_POOL_H

#include_next <net/page_pool.h>

#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(5,19,0)

static inline int page_pool_ethtool_stats_get_count(void)
{
	return 0;
}

static inline u8 *page_pool_ethtool_stats_get_strings(u8 *data)
{
	return data;
}

static inline u64 *page_pool_ethtool_stats_get(u64 *data, void *stats)
{
	return data;
}

#endif /* LINUX_VERSION_IS_LESS(5,19,0) */

#if LINUX_VERSION_IS_GEQ(5,15,0)
#ifndef CONFIG_PAGE_POOL
#define page_pool_create(a) ERR_PTR(-ENOTSUPP)
#define page_pool_dev_alloc_frag(a, b, c) NULL
#endif /* CONFIG_PAGE_POOL */
#endif /* LINUX_VERSION_IS_GEQ(5,15,0) */

#endif /* _BACKPORT_NET_PAGE_POOL_H */
