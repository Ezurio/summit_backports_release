#ifndef __BACKPORT_LINUX_PM_WAKEUP_H
#define __BACKPORT_LINUX_PM_WAKEUP_H
#include_next <linux/pm_wakeup.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(4,12,0)
#define pm_wakeup_hard_event LINUX_BACKPORT(pm_wakeup_hard_event)
static inline void pm_wakeup_hard_event(struct device *dev)
{
	return pm_wakeup_event(dev, 0);
}
#endif

#endif /* __BACKPORT_LINUX_PM_WAKEUP_H */
