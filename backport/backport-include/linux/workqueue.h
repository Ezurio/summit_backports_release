#ifndef __BACKPORT_LINUX_WORKQUEUE_H
#define __BACKPORT_LINUX_WORKQUEUE_H
#include_next <linux/workqueue.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(3,7,0)
#define mod_delayed_work LINUX_BACKPORT(mod_delayed_work)
bool mod_delayed_work(struct workqueue_struct *wq, struct delayed_work *dwork,
		      unsigned long delay);
#endif

#ifndef create_freezable_workqueue
/* note freez_a_ble -> freez_ea_able */
#define create_freezable_workqueue create_freezeable_workqueue
#endif

#if LINUX_VERSION_IS_LESS(3,3,0)
#define __WQ_ORDERED	0
/*
 * commit b196be89cdc14a88cc637cdad845a75c5886c82d
 * Author: Tejun Heo <tj@kernel.org>
 * Date:   Tue Jan 10 15:11:35 2012 -0800
 *
 *     workqueue: make alloc_workqueue() take printf fmt and args for name
 */
struct workqueue_struct *
backport_alloc_workqueue(const char *fmt, unsigned int flags,
			 int max_active, struct lock_class_key *key,
			 const char *lock_name, ...);
#undef alloc_workqueue
#ifdef CONFIG_LOCKDEP
#define alloc_workqueue(fmt, flags, max_active, args...)		\
({									\
	static struct lock_class_key __key;				\
	const char *__lock_name;					\
									\
	if (__builtin_constant_p(fmt))					\
		__lock_name = (fmt);					\
	else								\
		__lock_name = #fmt;					\
									\
	backport_alloc_workqueue((fmt), (flags), (max_active),		\
				 &__key, __lock_name, ##args);		\
})
#else
#define alloc_workqueue(fmt, flags, max_active, args...)		\
	backport_alloc_workqueue((fmt), (flags), (max_active),		\
				 NULL, NULL, ##args)
#endif
#undef alloc_ordered_workqueue
#define alloc_ordered_workqueue(fmt, flags, args...) \
	alloc_workqueue(fmt, WQ_UNBOUND | __WQ_ORDERED | (flags), 1, ##args)
#define destroy_workqueue backport_destroy_workqueue
void backport_destroy_workqueue(struct workqueue_struct *wq);
#endif

#if LINUX_VERSION_IS_LESS(3,11,0)
/* power efficient workqueues were added in commit 0668106ca386. */
#define system_power_efficient_wq system_wq
#define system_freezable_power_efficient_wq system_freezable_wq
#endif

#if LINUX_VERSION_IS_LESS(3,1,0)
#define drain_workqueue(wq) flush_workqueue(wq)
#endif

#if LINUX_VERSION_IS_LESS(4,5,0)
#define __WQ_LEGACY		(1 << 18)
#endif

#if LINUX_VERSION_IS_LESS(4,15,10) && \
	!LINUX_VERSION_IN_RANGE(4,14,27, 4,16,0) && \
	!LINUX_VERSION_IN_RANGE(4,9,88, 4,10,0) && \
	!LINUX_VERSION_IN_RANGE(4,4,122, 4,5,0) && \
	!LINUX_VERSION_IN_RANGE(4,1,51, 4,2,0) && \
	!LINUX_VERSION_IN_RANGE(3,16,57, 3,17,0)
#define current_work LINUX_BACKPORT(current_work)
struct work_struct *current_work(void);
#endif

#if LINUX_VERSION_IS_LESS(6,0,0) && \
    !LINUX_VERSION_IN_RANGE(5,15,142, 5,16,0) && \
    !LINUX_VERSION_IN_RANGE(5,10,203, 5,11,0)
#define cancel_work cancel_work_sync
#endif

#if LINUX_VERSION_IS_LESS(6,10,0)
#define disable_delayed_work(dw) cancel_delayed_work(dw)
#define disable_delayed_work_sync(dw) cancel_delayed_work_sync(dw)
#define disable_work(w) cancel_work(w)
#define disable_work_sync(w) cancel_work_sync(w)
#endif

#endif /* __BACKPORT_LINUX_WORKQUEUE_H */
