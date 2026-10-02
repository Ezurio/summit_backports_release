#ifndef __BACKPORT_LINUX_WORDPART_H
#define __BACKPORT_LINUX_WORDPART_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,9,0)
#include_next <linux/wordpart.h>
#else
#include <linux/kernel.h>

#ifndef lower_32_bits
#define lower_32_bits(n) ((u32)(n))
#endif

#endif 

#endif /* __BACKPORT_LINUX_WORDPART_H */
