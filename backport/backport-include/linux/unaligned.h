#ifndef __BACKPORT_LINUX_UNALIGNED_H
#define __BACKPORT_LINUX_UNALIGNED_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,12,0)
#include_next <linux/unaligned.h>
#else
#include_next <asm/unaligned.h>

#if LINUX_VERSION_IS_LESS(5,7,0)
static inline u32 __get_unaligned_be24(const u8 *p)
{
	return p[0] << 16 | p[1] << 8 | p[2];
}

static inline u32 get_unaligned_be24(const void *p)
{
	return __get_unaligned_be24(p);
}

static inline u32 __get_unaligned_le24(const u8 *p)
{
	return p[0] | p[1] << 8 | p[2] << 16;
}

static inline u32 get_unaligned_le24(const void *p)
{
	return __get_unaligned_le24(p);
}
#endif /* < 5.7 */
#endif

#endif /* __BACKPORT_LINUX_UNALIGNED_H */
