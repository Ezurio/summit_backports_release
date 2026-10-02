#ifndef _BACKPORT_LINUX_FORTIFY_STRING_H
#define _BACKPORT_LINUX_FORTIFY_STRING_H
#include <linux/version.h>

#if LINUX_VERSION_IN_RANGE(6,1,0, 6,3,0)
#undef __struct_size
#endif

#include_next <linux/fortify-string.h>

#endif /* _BACKPORT_LINUX_FORTIFY_STRING_H */
