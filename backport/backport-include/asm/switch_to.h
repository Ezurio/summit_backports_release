#ifndef __BACKPORT_SWITCH_TO_H
#define __BACKPORT_SWITCH_TO_H

#if LINUX_VERSION_IS_GEQ(3,4,0)
#include_next <asm/switch_to.h>
#else
#include <asm/system.h>
#endif

#endif
