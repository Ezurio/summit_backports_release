#ifndef __BACKPORT_NET_NETDEV_RX_QUEUE_H
#define __BACKPORT_NET_NETDEV_RX_QUEUE_H

#if LINUX_VERSION_IS_GEQ(6,6,0)
#include_next <net/netdev_rx_queue.h>
#else
#include <linux/netdevice.h>
#endif

#endif