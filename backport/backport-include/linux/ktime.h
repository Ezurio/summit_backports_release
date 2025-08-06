#ifndef __BACKPORT_LINUX_KTIME_H
#define __BACKPORT_LINUX_KTIME_H
#include_next <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/version.h>

#if  LINUX_VERSION_IS_LESS(3,8,0)
#define ktime_compare LINUX_BACKPORT(ktime_compare)
static inline int ktime_compare(const ktime_t cmp1, const ktime_t cmp2)
{
	if (cmp1.tv64 < cmp2.tv64)
		return -1;
	if (cmp1.tv64 > cmp2.tv64)
		return 1;
	return 0;
}
#endif /* < 3.8 */

#if  LINUX_VERSION_IS_LESS(3,17,0)
#define ktime_get_raw LINUX_BACKPORT(ktime_get_raw)
extern ktime_t ktime_get_raw(void);

/* convert a timespec64 to ktime_t format: */
#define timespec64_to_ktime LINUX_BACKPORT(timespec64_to_ktime)
static inline ktime_t timespec64_to_ktime(struct timespec64 ts)
{
	return ktime_set(ts.tv_sec, ts.tv_nsec);
}
#endif /* < 3.17 */

#if  LINUX_VERSION_IS_LESS(4,0,0)
#define ktime_ms_delta LINUX_BACKPORT(ktime_ms_delta)
static inline s64 ktime_ms_delta(const ktime_t later, const ktime_t earlier)
{
	return ktime_to_ms(ktime_sub(later, earlier));
}
#endif /* < 4.0 */

#ifndef ktime_to_timespec64
/* Map the ktime_t to timespec conversion to ns_to_timespec function */
#define ktime_to_timespec64(kt)		ns_to_timespec64((kt).tv64)
#endif

#endif /* __BACKPORT_LINUX_KTIME_H */
