#ifndef __BACKPORT_NET_6LOWPAN_H
#define __BACKPORT_NET_6LOWPAN_H
#include_next <net/6lowpan.h>

#if LINUX_VERSION_IS_LESS(4,12,0)
static inline void lowpan_iphc_uncompress_eui48_lladdr(struct in6_addr *ipaddr,
						       const void *lladdr)
{
	/* fe:80::XXXX:XXff:feXX:XXXX
	 *        \_________________/
	 *              hwaddr
	 */
	ipaddr->s6_addr[0] = 0xFE;
	ipaddr->s6_addr[1] = 0x80;
	memcpy(&ipaddr->s6_addr[8], lladdr, 3);
	ipaddr->s6_addr[11] = 0xFF;
	ipaddr->s6_addr[12] = 0xFE;
	memcpy(&ipaddr->s6_addr[13], lladdr + 3, 3);
}
#endif

#endif /* __BACKPORT_NET_6LOWPAN_H */
