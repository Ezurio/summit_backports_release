#ifndef __BACKPORT_GSO_H
#define __BACKPORT_GSO_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,5,0)
#include_next <net/gso.h>
#elif LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,4)
#include_next <net/gso.h>
#else
#include <linux/netdevice.h>
#endif

#endif
