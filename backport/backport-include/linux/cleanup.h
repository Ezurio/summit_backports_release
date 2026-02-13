#ifndef _BACKPORTS_LINUX_CLEANUP_H
#define _BACKPORTS_LINUX_CLEANUP_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,5,0) || LINUX_VERSION_IN_RANGE(6,1,79, 6,2,0)
#include_next <linux/cleanup.h>
#else
#include <linux/compiler.h>

#ifndef __free
#define DEFINE_FREE(_name, _type, _free) \
	static inline void __free_##_name(void *p) { _type _T = *(_type *)p; _free; }
#define __free(_name)	__cleanup(__free_##_name)
#endif /* __free */

#ifndef no_free_ptr
#define no_free_ptr(p) \
	({ __auto_type __ptr = (p); (p) = NULL; __ptr; })
#endif /* no_free_ptr */

#ifndef return_ptr
#define return_ptr(p)	return no_free_ptr(p)
#endif /* return_ptr */

#endif

#endif /* _BACKPORTS_LINUX_CLEANUP_H */
