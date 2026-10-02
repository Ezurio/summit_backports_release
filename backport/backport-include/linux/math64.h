#ifndef __BACKPORT_LINUX_MATH64_H
#define __BACKPORT_LINUX_MATH64_H
#include_next <linux/math64.h>

#if LINUX_VERSION_IS_LESS(3,12,0)

#if BITS_PER_LONG == 64
/**
 * div64_u64_rem - unsigned 64bit divide with 64bit divisor and remainder
 */
#define div64_u64_rem LINUX_BACKPORT(div64_u64_rem)
static inline u64 div64_u64_rem(u64 dividend, u64 divisor, u64 *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}
#elif BITS_PER_LONG == 32
#ifndef div64_u64_rem
#define div64_u64_rem LINUX_BACKPORT(div64_u64_rem)
#define backports_div64_u64_rem_add 1
extern u64 div64_u64_rem(u64 dividend, u64 divisor, u64 *remainder);
#endif

#endif /* BITS_PER_LONG */
#endif /* < 3.12 */

#ifndef DIV_U64_ROUND_UP
#define DIV_U64_ROUND_UP(ll, d)		\
	({ u32 _tmp = (d); div_u64((ll) + _tmp - 1, _tmp); })
#endif

#if LINUX_VERSION_IS_LESS(6,11,0)
#define roundup_u64 LINUX_BACKPORT(roundup_u64)
static inline u64 roundup_u64(u64 x, u32 y)
{
	return DIV_U64_ROUND_UP(x, y) * y;
}
#endif /* < 6.11 */

#endif /* __BACKPORT_LINUX_MATH64_H */
