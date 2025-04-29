#ifndef _BACKPORT_LINUX_KSTRTOX_H
#define _BACKPORT_LINUX_KSTRTOX_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(5,14,0) || \
    LINUX_VERSION_IN_RANGE(5,10,185, 5,11,0)
#include_next <linux/kstrtox.h>
#else
#include <linux/kernel.h>
#endif

#endif
