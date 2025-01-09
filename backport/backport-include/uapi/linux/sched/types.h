#ifndef __BACKPORT_UAPI_LINUX_SCHED_TYPES_H
#define __BACKPORT_UAPI_LINUX_SCHED_TYPES_H


#if LINUX_VERSION_IS_GEQ(4,11,0)
#include_next <uapi/linux/sched/types.h>
#include_next <linux/sched/types.h>
#elif LINUX_VERSION_IS_GEQ(3,14,0)
#include_next <linux/sched.h>
#endif

#endif /* __BACKPORT_UAPI_LINUX_SCHED_TYPES_H */
