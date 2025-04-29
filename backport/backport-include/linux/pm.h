#ifndef __BACKPORT_PM_H
#define __BACKPORT_PM_H
#include_next <linux/pm.h>

#ifndef PM_EVENT_AUTO
#define PM_EVENT_AUTO		0x0400
#endif

#ifndef PM_EVENT_SLEEP
#define PM_EVENT_SLEEP  (PM_EVENT_SUSPEND)
#endif

#ifndef PMSG_IS_AUTO
#define PMSG_IS_AUTO(msg)	(((msg).event & PM_EVENT_AUTO) != 0)
#endif

#ifndef pm_sleep_ptr
#define pm_sleep_ptr(_ptr) PTR_IF(IS_ENABLED(CONFIG_PM_SLEEP), (_ptr))
#endif

#ifndef DEFINE_SIMPLE_DEV_PM_OPS

#define _DEFINE_DEV_PM_OPS(name, \
			   suspend_fn, resume_fn, \
			   runtime_suspend_fn, runtime_resume_fn, idle_fn) \
const struct dev_pm_ops name = { \
	SET_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn) \
	SET_RUNTIME_PM_OPS(runtime_suspend_fn, runtime_resume_fn, idle_fn) \
}

#define DEFINE_SIMPLE_DEV_PM_OPS(name, suspend_fn, resume_fn) \
	_DEFINE_DEV_PM_OPS(name, suspend_fn, resume_fn, NULL, NULL, NULL)

#endif

#endif /* __BACKPORT_PM_H */
