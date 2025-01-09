#ifndef _COMPAT_LINUX_PM_QOS_H
#define _COMPAT_LINUX_PM_QOS_H 1

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(3,2,0)
#include_next <linux/pm_qos.h>
#else
#include <linux/pm_qos_params.h>
struct pm_qos_request {
	struct plist_node node;
	int pm_qos_class;
};
#endif /* LINUX_VERSION_IS_GEQ(3,2,0) */

#ifndef PM_QOS_DEFAULT_VALUE
#define PM_QOS_DEFAULT_VALUE -1
#endif

#if LINUX_VERSION_IS_LESS(5,7,0)
static inline void cpu_latency_qos_add_request(struct pm_qos_request *req,
                                              s32 value)
{
#if LINUX_VERSION_IS_GEQ(3,2,0)
	pm_qos_add_request(req, PM_QOS_CPU_DMA_LATENCY, value);
#else
	pm_qos_add_request((struct pm_qos_request_list *)req, PM_QOS_CPU_DMA_LATENCY, value);
#endif
}

static inline void cpu_latency_qos_update_request(struct pm_qos_request *req,
                                                 s32 new_value)
{
#if LINUX_VERSION_IS_GEQ(3,2,0)
	pm_qos_update_request(req, new_value);
#else
	pm_qos_update_request((struct pm_qos_request_list *)req, new_value);
#endif
}

static inline void cpu_latency_qos_remove_request(struct pm_qos_request *req)
{
#if LINUX_VERSION_IS_GEQ(3,2,0)
	pm_qos_remove_request(req);
#else
	pm_qos_remove_request((struct pm_qos_request_list *)req);
#endif
}

static inline bool cpu_latency_qos_request_active(struct pm_qos_request *req)
{
#if LINUX_VERSION_IS_GEQ(3,2,0)
	return pm_qos_request_active(req);
#else
	return pm_qos_request_active((struct pm_qos_request_list *)req);
#endif
}

static inline s32 cpu_latency_qos_limit(void)
{
	return pm_qos_request(PM_QOS_CPU_DMA_LATENCY);
}
#endif /* < 5.7 */

#endif	/* _COMPAT_LINUX_PM_QOS_H */
