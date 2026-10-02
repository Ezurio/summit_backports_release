#ifndef __BACKPORT_LINUX_RTC_H
#define __BACKPORT_LINUX_RTC_H

#include_next <linux/rtc.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(3,19,0)
#define rtc_time64_to_tm(time, tm) rtc_time_to_tm((unsigned long)time, tm)
#endif /* LINUX_VERSION_IS_LESS(3,19,0) */

#if LINUX_VERSION_IS_LESS(2,6,38)
struct rtc_time rtc_ktime_to_tm(ktime_t kt);
#endif /* LINUX_VERSION_IS_LESS(2,6,38) */

#endif /* __BACKPORT_LINUX_RTC_H */
