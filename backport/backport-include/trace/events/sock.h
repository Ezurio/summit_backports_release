#ifndef __BACKPORT_TRACE_EVENTS_SOCK_H
#define __BACKPORT_TRACE_EVENTS_SOCK_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(3,1,0)
#include_next <trace/events/sock.h>
#endif

#if LINUX_VERSION_IS_LESS(6,3,0)
#define trace_sk_data_ready(sk)
#endif

#endif
