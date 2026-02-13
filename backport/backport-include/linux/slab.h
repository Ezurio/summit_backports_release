#ifndef __BACKPORT_LINUX_SLAB_H
#define __BACKPORT_LINUX_SLAB_H
#include_next <linux/slab.h>
#include <linux/version.h>
#include <linux/cleanup.h>
#include <linux/err.h>

#if LINUX_VERSION_IS_LESS(3,4,0)
/* This backports:
 *
 * commit a8203725dfded5c1f79dca3368a4a273e24b59bb
 * Author: Xi Wang <xi.wang@gmail.com>
 * Date:   Mon Mar 5 15:14:41 2012 -0800
 *
 * 	slab: introduce kmalloc_array()
 */

#include <linux/kernel.h> /* for SIZE_MAX */

#define kmalloc_array LINUX_BACKPORT(kmalloc_array)
static inline void *kmalloc_array(size_t n, size_t size, gfp_t flags)
{
	if (size != 0 && n > SIZE_MAX / size)
		return NULL;
	return __kmalloc(n * size, flags);
}
#endif

#if LINUX_VERSION_IS_LESS(5,9,0)
#define kfree_sensitive(x)	kzfree(x)
#endif

#if LINUX_VERSION_IS_LESS(6,5,0) && !LINUX_VERSION_IN_RANGE(6,1,79, 6,2,0)
DEFINE_FREE(kfree, void *, if (!IS_ERR_OR_NULL(_T)) kfree(_T))
#endif

#endif /* __BACKPORT_LINUX_SLAB_H */
