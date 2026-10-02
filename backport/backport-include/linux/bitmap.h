#ifndef __BACKPORTS_LINUX_BITMAP_H
#define __BACKPORTS_LINUX_BITMAP_H
#include_next <linux/bitmap.h>

#ifndef BITMAP_FROM_U64
#if __BITS_PER_LONG == 64
#define BITMAP_FROM_U64(n) (n)
#else
#define BITMAP_FROM_U64(n) ((unsigned long) ((u64)(n) & ULONG_MAX)), \
				((unsigned long) ((u64)(n) >> 32))
#endif /* __BITS_PER_LONG */
#endif /* BITMAP_FROM_U64 */

#if LINUX_VERSION_IS_LESS(4,19,0) && \
    !LINUX_VERSION_IN_RANGE(4,9,212, 4,10,0) && \
    !LINUX_VERSION_IN_RANGE(4,4,212, 4,5,0)

unsigned long *bitmap_alloc(unsigned int nbits, gfp_t flags);
unsigned long *bitmap_zalloc(unsigned int nbits, gfp_t flags);
void bitmap_free(const unsigned long *bitmap);

#endif

#endif /* __BACKPORTS_LINUX_BITMAP_H */
