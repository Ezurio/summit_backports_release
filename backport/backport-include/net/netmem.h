#ifndef __BACKPORT_NET_NETMEM_H
#define __BACKPORT_NET_NETMEM_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,9,0)
#include_next <net/netmem.h>
#endif

#ifndef pp_page_to_nmdesc
#define pp_page_to_nmdesc(p) p
#endif

#endif /* __BACKPORT_NET_NETMEM_H */
