#ifndef __BACKPORT_BITOPS_H
#define __BACKPORT_BITOPS_H
#include_next <linux/bitops.h>

#include <generated/utsrelease.h>

#ifndef BITS_PER_LONG_LONG
#define BITS_PER_LONG_LONG 64
#endif

#ifndef GENMASK
/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) \
	(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define GENMASK_ULL(h, l) \
	(((~0ULL) << (l)) & (~0ULL >> (BITS_PER_LONG_LONG - 1 - (h))))
#endif

#ifndef BIT_ULL
#define BIT_ULL(nr) (1ULL << (nr))
#endif

#ifndef BITS_PER_TYPE
#define BITS_PER_TYPE(type) (sizeof(type) * BITS_PER_BYTE)
#endif

#ifndef BITS_TO_BYTES
#define BITS_TO_BYTES(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE)
#endif

#ifndef aligned_byte_mask
#ifdef __LITTLE_ENDIAN
#  define aligned_byte_mask(n) ((1UL << 8*(n))-1)
#else
#  define aligned_byte_mask(n) (~0xffUL << (BITS_PER_LONG - 8 - 8*(n)))
#endif
#endif /* aligned_byte_mask */

#ifndef assign_bit
#define assign_bit(nr, addr, value)					\
	((value) ? set_bit((nr), (addr)) : clear_bit((nr), (addr)))
#endif /* assign_bit */


#endif /* __BACKPORT_BITOPS_H */
