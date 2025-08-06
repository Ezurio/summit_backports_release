#ifndef __BP_ALGAPI_H
#define __BP_ALGAPI_H

#include <linux/version.h>
#include_next <crypto/algapi.h>

#if LINUX_VERSION_IS_LESS(3,13,0)
#define __crypto_memneq LINUX_BACKPORT(__crypto_memneq)
noinline unsigned long __crypto_memneq(const void *a, const void *b, size_t size);
#define crypto_memneq LINUX_BACKPORT(crypto_memneq)
static inline int crypto_memneq(const void *a, const void *b, size_t size)
{
        return __crypto_memneq(a, b, size) != 0UL ? 1 : 0;
}
#endif

#if LINUX_VERSION_IS_LESS(4,14,0)
#define __crypto_xor LINUX_BACKPORT(__crypto_xor)
void __crypto_xor(u8 *dst, const u8 *src1, const u8 *src2, unsigned int len);

#define crypto_xor_cpy LINUX_BACKPORT(crypto_xor_cpy)
static inline void crypto_xor_cpy(u8 *dst, const u8 *src1, const u8 *src2,
				  unsigned int size)
{
	if (IS_ENABLED(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) &&
	    __builtin_constant_p(size) &&
	    (size % sizeof(unsigned long)) == 0) {
		unsigned long *d = (unsigned long *)dst;
		unsigned long *s1 = (unsigned long *)src1;
		unsigned long *s2 = (unsigned long *)src2;

		while (size > 0) {
			*d++ = *s1++ ^ *s2++;
			size -= sizeof(unsigned long);
		}
	} else {
		__crypto_xor(dst, src1, src2, size);
	}
}
#endif

#if LINUX_VERSION_IS_LESS(4,1,0)
#define CRYPTO_ALG_INTERNAL		0x00002000
#endif

#endif /* __BP_ALGAPI_H */
