#ifndef __BACKPORT_LINUX_NET_INET_ECN_H
#define __BACKPORT_LINUX_NET_INET_ECN_H

#include_next <net/inet_ecn.h>

#include <linux/if_vlan.h>

#if LINUX_VERSION_IS_LESS(5,16,0)
#define skb_get_dsfield LINUX_BACKPORT(skb_get_dsfield)
static inline int skb_get_dsfield(struct sk_buff *skb)
{
	switch (skb_protocol(skb, true)) {
	case cpu_to_be16(ETH_P_IP):
		if (!pskb_network_may_pull(skb, sizeof(struct iphdr)))
			break;
		return ipv4_get_dsfield(ip_hdr(skb));

	case cpu_to_be16(ETH_P_IPV6):
		if (!pskb_network_may_pull(skb, sizeof(struct ipv6hdr)))
			break;
		return ipv6_get_dsfield(ipv6_hdr(skb));
	}

	return -1;
}
#endif /* LINUX_VERSION_IS_LESS(5,16,0) */

#endif /* __BACKPORT_LINUX_INET_ECN_H */