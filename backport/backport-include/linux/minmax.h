#ifndef __BACKPORT_LINUX_MINMAX_H
#define __BACKPORT_LINUX_MINMAX_H
#include <linux/version.h>

#if  LINUX_VERSION_IS_GEQ(5,10,0)
#include_next <linux/minmax.h>
#else
#include <linux/kernel.h>
#endif /* LINUX_VERSION_IS_GEQ(5,10,0) */

#endif /* __BACKPORT_LINUX_MINMAX_H */