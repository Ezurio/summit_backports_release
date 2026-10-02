#ifndef _BACKPORT_LINUX_JHASH_H
#define _BACKPORT_LINUX_JHASH_H
#include <linux/version.h>
#include_next <linux/jhash.h>

#if LINUX_VERSION_IS_LESS(2,6,38)
#include <linux/unaligned/packed_struct.h>
#endif

#endif /* _BACKPORT_LINUX_JHASH_H */
