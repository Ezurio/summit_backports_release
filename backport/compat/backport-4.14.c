// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <crypto/algapi.h>

void __crypto_xor(u8 *dst, const u8 *src1, const u8 *src2, unsigned int len)
{
	int relalign = 0;

	if (!IS_ENABLED(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)) {
		int size = sizeof(unsigned long);
		int d = (((unsigned long)dst ^ (unsigned long)src1) |
			 ((unsigned long)dst ^ (unsigned long)src2)) &
			(size - 1);

		relalign = d ? 1 << __ffs(d) : size;

		/*
		 * If we care about alignment, process as many bytes as
		 * needed to advance dst and src to values whose alignments
		 * equal their relative alignment. This will allow us to
		 * process the remainder of the input using optimal strides.
		 */
		while (((unsigned long)dst & (relalign - 1)) && len > 0) {
			*dst++ = *src1++ ^ *src2++;
			len--;
		}
	}

	while (IS_ENABLED(CONFIG_64BIT) && len >= 8 && !(relalign & 7)) {
		*(u64 *)dst = *(u64 *)src1 ^  *(u64 *)src2;
		dst += 8;
		src1 += 8;
		src2 += 8;
		len -= 8;
	}

	while (len >= 4 && !(relalign & 3)) {
		*(u32 *)dst = *(u32 *)src1 ^ *(u32 *)src2;
		dst += 4;
		src1 += 4;
		src2 += 4;
		len -= 4;
	}

	while (len >= 2 && !(relalign & 1)) {
		*(u16 *)dst = *(u16 *)src1 ^ *(u16 *)src2;
		dst += 2;
		src1 += 2;
		src2 += 2;
		len -= 2;
	}

	while (len--)
		*dst++ = *src1++ ^ *src2++;
}
EXPORT_SYMBOL_GPL(__crypto_xor);

/**
 * memset32() - Fill a memory area with a uint32_t
 * @s: Pointer to the start of the area.
 * @v: The value to fill the area with
 * @count: The number of values to store
 *
 * Differs from memset() in that it fills with a uint32_t instead
 * of a byte.  Remember that @count is the number of uint32_ts to
 * store, not the number of bytes.
 */
void *memset32(uint32_t *s, uint32_t v, size_t count)
{
	uint32_t *xs = s;

	while (count--)
		*xs++ = v;
	return s;
}
EXPORT_SYMBOL(memset32);