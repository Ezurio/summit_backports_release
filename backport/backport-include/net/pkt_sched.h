#ifndef __BACKPORT_NET_PKT_SCHED_H
#define __BACKPORT_NET_PKT_SCHED_H
#include_next <net/pkt_sched.h>

#ifndef DEFAULT_TX_QUEUE_LEN
#define DEFAULT_TX_QUEUE_LEN	1000
#endif

#endif /* __BACKPORT_NET_PKT_SCHED_H */

