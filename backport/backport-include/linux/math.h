/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORTS_LINUX_MATH_H
#define __BACKPORTS_LINUX_MATH_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(5,11,0)
#include_next <linux/math.h>
#else
#include <linux/kernel.h>

#ifndef mult_frac
#define mult_frac(x, numer, denom)(			\
{							\
    typeof(x) quot = (x) / (denom);			\
    typeof(x) rem  = (x) % (denom);			\
    (quot * (numer)) + ((rem * (numer)) / (denom));	\
}							\
)
#endif /* mult_frac */

#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#endif

#ifndef DIV_ROUND_UP_ULL
#define DIV_ROUND_UP_ULL(ll,d) \
   ({ unsigned long long _tmp = (ll)+(d)-1; do_div(_tmp, d); _tmp; })
#endif

#ifndef DIV_ROUND_DOWN_ULL
#define DIV_ROUND_DOWN_ULL(ll, d) \
	({ unsigned long long _tmp = (ll); do_div(_tmp, d); _tmp; })
#endif

#ifndef __round_mask
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))
#endif

#ifndef rounddown
#define rounddown(x, y) (				\
{							\
	typeof(x) __x = (x);				\
	__x - (__x % (y));				\
}							\
)
#endif /* rounddown */

#ifndef DIV_ROUND_CLOSEST
#define DIV_ROUND_CLOSEST(x, divisor)(			\
{							\
	typeof(x) __x = x;				\
	typeof(divisor) __d = divisor;			\
	(((typeof(x))-1) > 0 ||				\
	 ((typeof(divisor))-1) > 0 || (__x) > 0) ?	\
		(((__x) + ((__d) / 2)) / (__d)) :	\
		(((__x) - ((__d) / 2)) / (__d));	\
}							\
)
#endif

#ifndef DIV_ROUND_CLOSEST_ULL
#define DIV_ROUND_CLOSEST_ULL(x, divisor)(		\
{							\
	typeof(divisor) __d = divisor;			\
	unsigned long long _tmp = (x) + (__d) / 2;	\
	do_div(_tmp, __d);				\
	_tmp;						\
}							\
)
#endif

#if LINUX_VERSION_IS_LESS(4,5,0)
#undef abs
#define abs(x)	__abs_choose_expr(x, long long,				\
		__abs_choose_expr(x, long,				\
		__abs_choose_expr(x, int,				\
		__abs_choose_expr(x, short,				\
		__abs_choose_expr(x, char,				\
		__builtin_choose_expr(					\
			__builtin_types_compatible_p(typeof(x), char),	\
			(char)({ signed char __x = (x); __x<0?-__x:__x; }), \
			((void)0)))))))

#define __abs_choose_expr(x, type, other) __builtin_choose_expr(	\
	__builtin_types_compatible_p(typeof(x),   signed type) ||	\
	__builtin_types_compatible_p(typeof(x), unsigned type),		\
	({ signed type __x = (x); __x < 0 ? -__x : __x; }), other)
#endif /* LINUX_VERSION_IS_LESS(4,5,0) */

#if LINUX_VERSION_IS_LESS(3,14,0)
static inline u32 reciprocal_scale(u32 val, u32 ep_ro)
{
	return (u32)(((u64) val * ep_ro) >> 32);
}
#endif /* LINUX_VERSION_IS_LESS(3,14,0) */

#endif

#ifndef abs_diff
#define abs_diff(a, b) ({			\
	typeof(a) __a = (a);			\
	typeof(b) __b = (b);			\
	(void)(&__a == &__b);			\
	__a > __b ? (__a - __b) : (__b - __a);	\
})
#endif

#endif /* __BACKPORTS_LINUX_MATH_H */
