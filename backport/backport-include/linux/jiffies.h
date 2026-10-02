#ifndef __BACKPORT_LNIUX_JIFFIES_H
#define __BACKPORT_LNIUX_JIFFIES_H
#include_next <linux/jiffies.h>

#ifndef time_is_before_jiffies
#define time_is_before_jiffies(a) time_after(jiffies, a)
#endif

#ifndef time_is_after_jiffies
#define time_is_after_jiffies(a) time_before(jiffies, a)
#endif

#ifndef time_is_before_eq_jiffies
#define time_is_before_eq_jiffies(a) time_after_eq(jiffies, a)
#endif

#ifndef time_is_after_eq_jiffies
#define time_is_after_eq_jiffies(a) time_before_eq(jiffies, a)
#endif

/*
 * This function is available, but not exported in kernel < 3.17, add
 * an own version.
 */
#if LINUX_VERSION_IS_LESS(3,17,0)
#define nsecs_to_jiffies LINUX_BACKPORT(nsecs_to_jiffies)
extern unsigned long nsecs_to_jiffies(u64 n);
#endif /* 3.17 */

#ifndef secs_to_jiffies
#define secs_to_jiffies(_secs) (unsigned long)((_secs) * HZ)
#endif

#if LINUX_VERSION_IS_LESS(4,19,0)
#define jiffies_delta_to_msecs LINUX_BACKPORT(jiffies_delta_to_msecs)
static inline unsigned int jiffies_delta_to_msecs(long delta)
{
	return jiffies_to_msecs(max(0L, delta));
}
#endif /* 4.19 */

#endif /* __BACKPORT_LNIUX_JIFFIES_H */
