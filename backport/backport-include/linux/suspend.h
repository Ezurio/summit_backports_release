#ifndef __BACKPORT_SUSPEND_H
#define __BACKPORT_SUSPEND_H
#include_next <linux/suspend.h>

#if LINUX_VERSION_IS_LESS(4,0,0)
#define pm_system_wakeup LINUX_BACKPORT(pm_system_wakeup)
static inline void pm_system_wakeup(void)
{
#if LINUX_VERSION_IS_GEQ(3,9,0)
	freeze_wake();
#endif
}
#endif

#endif /* __BACKPORT_SUSPEND_H */
