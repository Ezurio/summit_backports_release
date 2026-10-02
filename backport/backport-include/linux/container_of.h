#ifndef _BACKPORT_LINUX_CONTAINER_OF_H
#define _BACKPORT_LINUX_CONTAINER_OF_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(5,16,0)
#include_next <linux/container_of.h>
#endif

#ifndef container_of_const
#define container_of_const(ptr, type, member)				\
		((type *)container_of(ptr, type, member))
#endif

#endif /* _BACKPORT_LINUX_CONTAINER_OF_H */
