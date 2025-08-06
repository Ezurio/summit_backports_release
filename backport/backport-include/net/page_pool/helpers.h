#ifndef _BACKPORT_NET_PAGE_POOL_HELPERS_H
#define _BACKPORT_NET_PAGE_POOL_HELPERS_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,6,0)
#include_next <net/page_pool/helpers.h>
#else
#include <net/page_pool.h>
#endif

#endif /* _BACKPORT_NET_PAGE_POOL_HELPERS_H */
