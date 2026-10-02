#ifndef __BACKPORT_IDR_H
#define __BACKPORT_IDR_H
/* some versions have a broken idr header */
#include <linux/spinlock.h>
#include_next <linux/idr.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(3,1,0)
#define ida_simple_get LINUX_BACKPORT(ida_simple_get)
int ida_simple_get(struct ida *ida, unsigned int start, unsigned int end,
		   gfp_t gfp_mask);

#define ida_simple_remove LINUX_BACKPORT(ida_simple_remove)
void ida_simple_remove(struct ida *ida, unsigned int id);
#endif

#if LINUX_VERSION_IS_LESS(3,9,0)
#include <linux/errno.h>
/**
 * backport of idr idr_alloc() usage
 *
 * This backports a patch series send by Tejun Heo:
 * https://lkml.org/lkml/2013/2/2/159
 */
static inline void compat_idr_destroy(struct idr *idp)
{
	idr_remove_all(idp);
	idr_destroy(idp);
}
#define idr_destroy(idp) compat_idr_destroy(idp)

static inline int idr_alloc(struct idr *idr, void *ptr, int start, int end,
			    gfp_t gfp_mask)
{
	int id, ret;

	do {
		if (!idr_pre_get(idr, gfp_mask))
			return -ENOMEM;
		ret = idr_get_new_above(idr, ptr, start, &id);
		if (!ret && id > end) {
			idr_remove(idr, id);
			ret = -ENOSPC;
		}
	} while (ret == -EAGAIN);

	return ret ? ret : id;
}

static inline void idr_preload(gfp_t gfp_mask)
{
}

static inline void idr_preload_end(void)
{
}
#endif

#ifndef idr_for_each_entry
#define idr_for_each_entry(idp, entry, id)			\
	for (id = 0; ((entry) = idr_get_next(idp, &(id))) != NULL; ++id)
#endif

#if LINUX_VERSION_IS_LESS(3,15,0)
static inline bool idr_is_empty(struct idr *idp) {
	void * x;
	int i;
	idr_for_each_entry(idp, x, i) {
		return false;
	}
	return true;
}
#endif

#if LINUX_VERSION_IS_LESS(4, 11, 0)
static inline void *backport_idr_remove(struct idr *idr, int id)
{
	void *item = idr_find(idr, id);
	idr_remove(idr, id);
	return item;
}
#define idr_remove LINUX_BACKPORT(idr_remove)
#endif

#if LINUX_VERSION_IS_LESS(4, 19, 0)
#define ida_alloc_range	LINUX_BACKPORT(ida_alloc_range)
static inline int ida_alloc_range(struct ida *ida, unsigned int min, unsigned int max, gfp_t gfp)
{
	return ida_simple_get(ida, min, max + 1, gfp);
}

#define ida_alloc LINUX_BACKPORT(ida_alloc)
static inline int ida_alloc(struct ida *ida, gfp_t gfp)
{
	return ida_alloc_range(ida, 0, ~0, gfp);
}

#define ida_alloc_min LINUX_BACKPORT(ida_alloc_min)
static inline int ida_alloc_min(struct ida *ida, unsigned int min, gfp_t gfp)
{
	return ida_alloc_range(ida, min, ~0, gfp);
}

#define ida_alloc_max LINUX_BACKPORT(ida_alloc_max)
static inline int ida_alloc_max(struct ida *ida, unsigned int max, gfp_t gfp)
{
	return ida_alloc_range(ida, 0, max, gfp);
}

#define ida_free LINUX_BACKPORT(ida_free)
static inline void ida_free(struct ida *ida, unsigned int id)
{
	ida_simple_remove(ida, id);
}
#endif

#endif /* __BACKPORT_IDR_H */
