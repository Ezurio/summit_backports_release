#ifndef __BACKPORT_BITOPS_H
#define __BACKPORT_BITOPS_H
#include_next <linux/bitops.h>

#include <generated/utsrelease.h>

#ifndef GENMASK
/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l)		(((U32_C(1) << ((h) - (l) + 1)) - 1) << (l))
#define GENMASK_ULL(h, l)	(((U64_C(1) << ((h) - (l) + 1)) - 1) << (l))

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

#endif /* __BACKPORT_BITOPS_H */
