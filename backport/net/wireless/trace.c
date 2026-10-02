#include <linux/module.h>
#if LINUX_VERSION_IS_LESS(3,1,0)
#include <linux/interrupt.h>
#endif

#ifndef __CHECKER__
#include <linux/rtnetlink.h>
#include <linux/etherdevice.h>
#define CREATE_TRACE_POINTS
#include "trace.h"

#endif
