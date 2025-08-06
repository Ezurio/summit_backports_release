#include <linux/export.h>
#include <linux/device.h>
#include <linux/property.h>

#include <linux/of.h>
#include <linux/of_device.h>

const void *device_get_match_data(struct device *dev)
{
#if defined(CONFIG_OF)
#if LINUX_VERSION_IS_GEQ(4,2,0)
	return of_device_get_match_data(dev);
#else
	const struct of_device_id *match;

	match = of_match_device(dev->driver->of_match_table, dev);
	if (!match)
		return NULL;

	return match->data;
#endif
#else
	return NULL;
#endif
}
EXPORT_SYMBOL_GPL(device_get_match_data);

#if LINUX_VERSION_IS_LESS(4,15,10) && \
	!LINUX_VERSION_IN_RANGE(4,14,27, 4,16,0) && \
	!LINUX_VERSION_IN_RANGE(4,9,88, 4,10,0) && \
	!LINUX_VERSION_IN_RANGE(4,4,122, 4,5,0) && \
	!LINUX_VERSION_IN_RANGE(4,1,51, 4,2,0) && \
	!LINUX_VERSION_IN_RANGE(3,16,57, 3,17,0)

#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/hardirq.h>

#if LINUX_VERSION_IS_GEQ(3,7,0)
struct kthread {
	unsigned long flags;
	unsigned int cpu;
	void *data;
	struct completion parked;
	struct completion exited;
};
#else
struct kthread {
	int should_stop;
	void *data;
	struct completion exited;
};
#endif

#if LINUX_VERSION_IS_GEQ(4,10,0)
static inline struct kthread *to_kthread(struct task_struct *k)
{
	return (__force void *)k->set_child_tid;
}
#else
#define to_kthread(tsk)	\
	container_of((tsk)->vfork_done, struct kthread, exited)
#endif

void *kthread_data(struct task_struct *task)
{
	return to_kthread(task)->data;
}

#define in_task()		(!(preempt_count() & \
				   (NMI_MASK | HARDIRQ_MASK | SOFTIRQ_OFFSET)))

static inline struct worker *current_wq_worker(void)
{
	if (in_task() && (current->flags & PF_WQ_WORKER))
		return kthread_data(current);
	return NULL;
}

struct worker {
	/* on idle list while idle, on busy hash table while busy */
	union {
		struct list_head	entry;	/* L: while idle */
		struct hlist_node	hentry;	/* L: while busy */
	};

	struct work_struct	*current_work;	/* L: work being processed */
};

struct work_struct *current_work(void)
{
	struct worker *worker = current_wq_worker();

	return worker ? worker->current_work : NULL;
}
EXPORT_SYMBOL(current_work);
#endif
