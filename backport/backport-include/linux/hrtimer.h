#ifndef __BACKPORT_LINUX_HRTIMER_H
#define __BACKPORT_LINUX_HRTIMER_H
#include <linux/version.h>
#include_next <linux/hrtimer.h>

#if LINUX_VERSION_IS_LESS(4,16,0)

#define HRTIMER_MODE_ABS_SOFT HRTIMER_MODE_ABS
#define HRTIMER_MODE_REL_SOFT HRTIMER_MODE_REL

#endif /* < 4.16 */

#if LINUX_VERSION_IS_LESS(6,13,0)
#define hrtimer_setup(a,b,c,d) hrtimer_init(a,c,d); (a)->function = b

#define hrtimer_update_function(a,b) (a)->function = b
#endif /* < 6.13 */

#if LINUX_VERSION_IS_LESS(6,15,0)
#define hrtimer_dummy_timeout LINUX_BACKPORT(hrtimer_dummy_timeout)
static inline enum hrtimer_restart hrtimer_dummy_timeout(struct hrtimer *unused)
{
	return HRTIMER_NORESTART;
}
#endif /* < 6.15 */

#endif /* __BACKPORT_LINUX_HRTIMER_H */
