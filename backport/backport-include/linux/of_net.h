#ifndef _BACKPORTS_LINUX_OF_NET_H
#define _BACKPORTS_LINUX_OF_NET_H

#if LINUX_VERSION_IS_GEQ(3,0,0)
#include_next <linux/of_net.h>
#endif

#include <linux/version.h>
#include <linux/etherdevice.h>

#ifndef CONFIG_OF
#if LINUX_VERSION_IS_LESS(3,10,0)
static inline const void *of_get_mac_address(struct device_node *np)
{
	return NULL;
}
#endif
#endif

/* The behavior of of_get_mac_address() changed in kernel 5.2, it now
 * returns an error code and not NULL in case of an error.
 */
#if LINUX_VERSION_IS_LESS(5,13,0)
static inline int backport_of_get_mac_address(struct device_node *np, u8 *mac_out)
{
	const void *mac = of_get_mac_address(np);

	if (!mac)
		return -ENODEV;
	if (IS_ERR(mac))
		return PTR_ERR(mac);
	ether_addr_copy(mac_out, mac);

	return 0;
}
#define of_get_mac_address LINUX_BACKPORT(of_get_mac_address)
#endif /* < 5.13 */

#endif /* _BACKPORTS_LINUX_OF_NET_H */
