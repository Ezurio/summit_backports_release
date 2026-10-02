#ifndef __BACKPORT_LINUX_VMALLOC_H
#define __BACKPORT_LINUX_VMALLOC_H
#include <linux/version.h>
#include_next <linux/vmalloc.h>

#if LINUX_VERSION_IS_LESS(5,18,0) && \
    !LINUX_VERSION_IN_RANGE(5,15,53, 5,16,0) && \
    !LINUX_VERSION_IN_RANGE(5,10,210, 5,11,0)
#define vcalloc(n, s) vzalloc(n*s)
#endif

#endif /* __BACKPORT_LINUX_VMALLOC_H_ */
