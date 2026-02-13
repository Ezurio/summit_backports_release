#ifndef __BACKPORT_LINUX_RPS_H
#define __BACKPORT_LINUX_RPS_H
#if LINUX_VERSION_IS_GEQ(6,9,0)
#include_next <net/rps.h>
#endif
#endif
