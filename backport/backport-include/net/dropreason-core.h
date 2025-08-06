#ifndef __BACKPORT_DROPREASON_CORE_H
#define __BACKPORT_DROPREASON_CORE_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,4,0)
#include_next <net/dropreason-core.h>
#elif LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,3)
#include_next <net/dropreason-core.h>
#elif LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,2)
/* Everything defined in dropreason.h */
#else

#if LINUX_VERSION_IN_RANGE(6,0,0, 5,17,0) && \
    LINUX_VERSION_IN_RANGE(5,15,58, 5,16,0)
#include <linux/skbuff.h>
#endif


#if LINUX_VERSION_IS_LESS(5,17,0) && \
	!LINUX_VERSION_IN_RANGE(5,15,58, 5,16,0)
#define SKB_CONSUMED 0
#define SKB_NOT_DROPPED_YET 6
#else
#if LINUX_VERSION_IS_LESS(5,18,0)
#define SKB_NOT_DROPPED_YET SKB_DROP_REASON_MAX
#endif

#if LINUX_VERSION_IS_LESS(6,2,0)
#define SKB_CONSUMED SKB_DROP_REASON_NOT_SPECIFIED
#endif
#endif

#if LINUX_VERSION_IS_LESS(6,4,0)
#define SKB_DROP_REASON_SUBSYS_MASK			0xffff0000
#define SKB_DROP_REASON_SUBSYS_SHIFT			16
#endif

#endif /* RHEL < 9.3 */
#endif /* __BACKPORT_DROPREASON_CORE_H */
