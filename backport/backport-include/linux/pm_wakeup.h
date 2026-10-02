#ifndef __BACKPORT_LINUX_PM_WAKEUP_H
#define __BACKPORT_LINUX_PM_WAKEUP_H
#include_next <linux/pm_wakeup.h>
#include <linux/version.h>
#include <linux/device.h>

#if LINUX_VERSION_IS_LESS(4,12,0)
#define pm_wakeup_hard_event LINUX_BACKPORT(pm_wakeup_hard_event)
static inline void pm_wakeup_hard_event(struct device *dev)
{
	return pm_wakeup_event(dev, 0);
}
#endif

#if LINUX_VERSION_IS_LESS(6,14,0)
static void device_disable_wakeup(void *dev)
{
	device_init_wakeup(dev, false);
}

#if LINUX_VERSION_IS_LESS(4,6,0)
static inline int devm_add_action_or_reset(struct device *dev,
					   void (*action)(void *), void *data);
#endif

static inline int devm_device_init_wakeup(struct device *dev)
{
	device_init_wakeup(dev, true);
	return devm_add_action_or_reset(dev, device_disable_wakeup, dev);
}
#endif

#endif /* __BACKPORT_LINUX_PM_WAKEUP_H */
