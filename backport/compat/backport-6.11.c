// SPDX-License-Identifier: GPL-2.0
#include <linux/netdevice.h>

static void init_dummy_netdev_core(struct net_device *dev)
{
	/* make sure we BUG if trying to hit standard
	 * register/unregister code path
	 */
	dev->reg_state = NETREG_DUMMY;

	/* NAPI wants this */
	INIT_LIST_HEAD(&dev->napi_list);

	/* a dummy interface is started by default */
	set_bit(__LINK_STATE_PRESENT, &dev->state);
	set_bit(__LINK_STATE_START, &dev->state);

	/* napi_busy_loop stats accounting wants this */
	dev_net_set(dev, &init_net);

	/* Note : We dont allocate pcpu_refcnt for dummy devices,
	 * because users of this 'device' dont need to change
	 * its refcount.
	 */
}

struct net_device *alloc_netdev_dummy(int sizeof_priv)
{
	return alloc_netdev(sizeof_priv, "dummy#", NET_NAME_UNKNOWN,
		init_dummy_netdev_core);
}
EXPORT_SYMBOL_GPL(alloc_netdev_dummy);
