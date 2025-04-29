#ifndef __BACKPORT_KERNEL_H
#define __BACKPORT_KERNEL_H

#if LINUX_VERSION_IS_LESS(3,0,0)
/* workaround for include ordering issues with 2.6 kernel headers
 * and upstream changes.
 */
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#endif

#include_next <linux/kernel.h>
#include <linux/version.h>
/*
 * some older kernels don't have this and thus don't
 * include it from kernel.h like new kernels
 */
#include <linux/printk.h>

#ifndef PTR_IF
#define PTR_IF(cond, ptr)	((cond) ? (ptr) : NULL)
#endif

/*
 * This backports:
 *
 *   From a3860c1c5dd1137db23d7786d284939c5761d517 Mon Sep 17 00:00:00 2001
 *   From: Xi Wang <xi.wang@gmail.com>
 *   Date: Thu, 31 May 2012 16:26:04 -0700
 *   Subject: [PATCH] introduce SIZE_MAX
 */
#ifndef SIZE_MAX
#define SIZE_MAX    (~(size_t)0)
#endif

#ifndef mult_frac
/*
 * Multiplies an integer by a fraction, while avoiding unnecessary
 * overflow or loss of precision.
 */
#define mult_frac(x, numer, denom)(			\
{							\
	typeof(x) quot = (x) / (denom);			\
	typeof(x) rem  = (x) % (denom);			\
	(quot * (numer)) + ((rem * (numer)) / (denom));	\
}							\
)
#endif /* mult_frac */

/* This backports:
 *
 * commit 36a26c69b4c70396ef569c3452690fba0c1dec08
 * Author: Nicholas Bellinger <nab@linux-iscsi.org>
 * Date:   Tue Jul 26 00:35:26 2011 -0700
 *
 * 	kernel.h: Add DIV_ROUND_UP_ULL and DIV_ROUND_UP_SECTOR_T macro usage
 */
#ifndef DIV_ROUND_UP_ULL
#define DIV_ROUND_UP_ULL(ll,d) \
	({ unsigned long long _tmp = (ll)+(d)-1; do_div(_tmp, d); _tmp; })
#endif

#if LINUX_VERSION_IS_LESS(3,0,0)
#define kstrtoull_from_user LINUX_BACKPORT(kstrtoull_from_user)
int __must_check kstrtoull_from_user(const char __user *s, size_t count, unsigned int base, unsigned long long *res);
#define kstrtoll_from_user LINUX_BACKPORT(kstrtoll_from_user)
int __must_check kstrtoll_from_user(const char __user *s, size_t count, unsigned int base, long long *res);
#define kstrtoul_from_user LINUX_BACKPORT(kstrtoul_from_user)
int __must_check kstrtoul_from_user(const char __user *s, size_t count, unsigned int base, unsigned long *res);
#define kstrtol_from_user LINUX_BACKPORT(kstrtol_from_user)
int __must_check kstrtol_from_user(const char __user *s, size_t count, unsigned int base, long *res);
#define kstrtouint_from_user LINUX_BACKPORT(kstrtouint_from_user)
int __must_check kstrtouint_from_user(const char __user *s, size_t count, unsigned int base, unsigned int *res);
#define kstrtoint_from_user LINUX_BACKPORT(kstrtoint_from_user)
int __must_check kstrtoint_from_user(const char __user *s, size_t count, unsigned int base, int *res);
#define kstrtou16_from_user LINUX_BACKPORT(kstrtou16_from_user)
int __must_check kstrtou16_from_user(const char __user *s, size_t count, unsigned int base, u16 *res);
#define kstrtos16_from_user LINUX_BACKPORT(kstrtos16_from_user)
int __must_check kstrtos16_from_user(const char __user *s, size_t count, unsigned int base, s16 *res);
#define kstrtou8_from_user LINUX_BACKPORT(kstrtou8_from_user)
int __must_check kstrtou8_from_user(const char __user *s, size_t count, unsigned int base, u8 *res);
#define kstrtos8_from_user LINUX_BACKPORT(kstrtos8_from_user)
int __must_check kstrtos8_from_user(const char __user *s, size_t count, unsigned int base, s8 *res);

#define kstrtou64_from_user LINUX_BACKPORT(kstrtou64_from_user)
static inline int __must_check kstrtou64_from_user(const char __user *s, size_t count, unsigned int base, u64 *res)
{
	return kstrtoull_from_user(s, count, base, res);
}

#define kstrtos64_from_user LINUX_BACKPORT(kstrtos64_from_user)
static inline int __must_check kstrtos64_from_user(const char __user *s, size_t count, unsigned int base, s64 *res)
{
	return kstrtoll_from_user(s, count, base, res);
}

#define kstrtou32_from_user LINUX_BACKPORT(kstrtou32_from_user)
static inline int __must_check kstrtou32_from_user(const char __user *s, size_t count, unsigned int base, u32 *res)
{
	return kstrtouint_from_user(s, count, base, res);
}

#define kstrtos32_from_user LINUX_BACKPORT(kstrtos32_from_user)
static inline int __must_check kstrtos32_from_user(const char __user *s, size_t count, unsigned int base, s32 *res)
{
	return kstrtoint_from_user(s, count, base, res);
}
#endif

#if LINUX_VERSION_IS_LESS(2,6,39)
/*
 * kstrto* was included in kernel 2.6.38.4 and causes conflicts with the
 * version included in compat-drivers. We use strict_strtol to check if
 * kstrto* is already available.
 */
#ifndef strict_strtoull
/* Internal, do not use. */
#define _kstrtoul LINUX_BACKPORT(_kstrtoul)
int __must_check _kstrtoul(const char *s, unsigned int base, unsigned long *res);
#define _kstrtol LINUX_BACKPORT(_kstrtol)
int __must_check _kstrtol(const char *s, unsigned int base, long *res);

#define kstrtoull LINUX_BACKPORT(kstrtoull)
int __must_check kstrtoull(const char *s, unsigned int base, unsigned long long *res);
#define kstrtoll LINUX_BACKPORT(kstrtoll)
int __must_check kstrtoll(const char *s, unsigned int base, long long *res);
#define kstrtoul LINUX_BACKPORT(kstrtoul)
static inline int __must_check kstrtoul(const char *s, unsigned int base, unsigned long *res)
{
	/*
	 * We want to shortcut function call, but
	 * __builtin_types_compatible_p(unsigned long, unsigned long long) = 0.
	 */
	if (sizeof(unsigned long) == sizeof(unsigned long long) &&
	    __alignof__(unsigned long) == __alignof__(unsigned long long))
		return kstrtoull(s, base, (unsigned long long *)res);
	else
		return _kstrtoul(s, base, res);
}

#define kstrtol LINUX_BACKPORT(kstrtol)
static inline int __must_check kstrtol(const char *s, unsigned int base, long *res)
{
	/*
	 * We want to shortcut function call, but
	 * __builtin_types_compatible_p(long, long long) = 0.
	 */
	if (sizeof(long) == sizeof(long long) &&
	    __alignof__(long) == __alignof__(long long))
		return kstrtoll(s, base, (long long *)res);
	else
		return _kstrtol(s, base, res);
}

#define kstrtouint LINUX_BACKPORT(kstrtouint)
int __must_check kstrtouint(const char *s, unsigned int base, unsigned int *res);
#define kstrtoint LINUX_BACKPORT(kstrtoint)
int __must_check kstrtoint(const char *s, unsigned int base, int *res);

#define kstrtou64 LINUX_BACKPORT(kstrtou64)
static inline int __must_check kstrtou64(const char *s, unsigned int base, u64 *res)
{
	return kstrtoull(s, base, res);
}

#define kstrtos64 LINUX_BACKPORT(kstrtos64)
static inline int __must_check kstrtos64(const char *s, unsigned int base, s64 *res)
{
	return kstrtoll(s, base, res);
}

#define kstrtou32 LINUX_BACKPORT(kstrtou32)
static inline int __must_check kstrtou32(const char *s, unsigned int base, u32 *res)
{
	return kstrtouint(s, base, res);
}

#define kstrtos32 LINUX_BACKPORT(kstrtos32)
static inline int __must_check kstrtos32(const char *s, unsigned int base, s32 *res)
{
	return kstrtoint(s, base, res);
}

#define kstrtou16 LINUX_BACKPORT(kstrtou16)
int __must_check kstrtou16(const char *s, unsigned int base, u16 *res);
#define kstrtos16 LINUX_BACKPORT(kstrtos16)
int __must_check kstrtos16(const char *s, unsigned int base, s16 *res);
#define kstrtou8 LINUX_BACKPORT(kstrtou8)
int __must_check kstrtou8(const char *s, unsigned int base, u8 *res);
#define kstrtos8 LINUX_BACKPORT(kstrtos8)
int __must_check kstrtos8(const char *s, unsigned int base, s8 *res);
#endif /* ifndef strict_strtol */

#endif /* < 2.6.39 */

#ifndef USHRT_MAX
#define USHRT_MAX      ((u16)(~0U))
#endif

#ifndef SHRT_MAX
#define SHRT_MAX       ((s16)(USHRT_MAX>>1))
#endif

#ifndef SHRT_MIN
#define SHRT_MIN       ((s16)(-SHRT_MAX - 1))
#endif

#ifndef U8_MAX
#define U8_MAX		((u8)~0U)
#endif

#ifndef S8_MAX
#define S8_MAX		((s8)(U8_MAX>>1))
#endif

#ifndef S8_MIN
#define S8_MIN		((s8)(-S8_MAX - 1))
#endif

#ifndef U16_MAX
#define U16_MAX		((u16)~0U)
#endif

#ifndef S16_MAX
#define S16_MAX		((s16)(U16_MAX>>1))
#endif

#ifndef S16_MIN
#define S16_MIN		((s16)(-S16_MAX - 1))
#endif

#ifndef U32_MAX
#define U32_MAX		((u32)~0U)
#endif

#ifndef S32_MAX
#define S32_MAX		((s32)(U32_MAX>>1))
#endif

#ifndef S32_MIN
#define S32_MIN		((s32)(-S32_MAX - 1))
#endif

#ifndef __round_mask
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))
#endif

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

#ifndef swap
#define swap(a, b) \
	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)
#endif

#ifndef lower_32_bits
#define lower_32_bits(n) ((u32)(n))
#endif

#ifndef clamp
#define clamp(val, min, max) ({			\
	typeof(val) __val = (val);		\
	typeof(min) __min = (min);		\
	typeof(max) __max = (max);		\
	(void) (&__val == &__min);		\
	(void) (&__val == &__max);		\
	__val = __val < __min ? __min: __val;	\
	__val > __max ? __max: __val; })
#endif

#ifndef clamp_t
#define clamp_t(type, val, min, max) ({		\
	type __val = (val);			\
	type __min = (min);			\
	type __max = (max);			\
	__val = __val < __min ? __min: __val;	\
	__val > __max ? __max: __val; })
#endif

#ifndef clamp_val
#define clamp_val(val, min, max) ({             \
	typeof(val) __val = (val);              \
	typeof(val) __min = (min);              \
	typeof(val) __max = (max);              \
	__val = __val < __min ? __min: __val;   \
	__val > __max ? __max: __val; })
#endif

#ifndef rounddown
#define rounddown(x, y) (				\
{							\
	typeof(x) __x = (x);				\
	__x - (__x % (y));				\
}							\
)
#endif /* rounddown */

#if LINUX_VERSION_IS_LESS(2,6,38)
#undef BUILD_BUG_ON
#ifndef __OPTIMIZE__
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#else
extern int __build_bug_on_failed;
#define BUILD_BUG_ON(condition)					\
	do {							\
		((void)sizeof(char[1 - 2*!!(condition)]));	\
		if (condition) __build_bug_on_failed = 1;	\
	} while(0)
#endif
#endif

#if LINUX_VERSION_IS_LESS(3,2,0)
#define hex_byte_pack pack_hex_byte

/* kernels before 3.2 didn't have error checking for the function */
#define hex2bin LINUX_BACKPORT(hex2bin)
int __must_check hex2bin(u8 *dst, const char *src, size_t count);
#endif /* < 3.2 */

#if LINUX_VERSION_IS_LESS(3,18,0)
#undef clamp
#define clamp(val, lo, hi) min((typeof(val))max(val, lo), hi)
#endif /* < 3.18 */

#if LINUX_VERSION_IS_LESS(4,6,0)
#define kstrtobool LINUX_BACKPORT(kstrtobool)
int __must_check kstrtobool(const char *s, bool *res);
#define kstrtobool_from_user LINUX_BACKPORT(kstrtobool_from_user)
int __must_check kstrtobool_from_user(const char __user *s, size_t count, bool *res);
#endif

#if LINUX_VERSION_IS_LESS(4,5,0)

#undef abs
/**
 * abs - return absolute value of an argument
 * @x: the value.  If it is unsigned type, it is converted to signed type first.
 *     char is treated as if it was signed (regardless of whether it really is)
 *     but the macro's return type is preserved as char.
 *
 * Return: an absolute value of x.
 */
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

#endif

#if LINUX_VERSION_IS_LESS(3,14,0)
static inline u32 reciprocal_scale(u32 val, u32 ep_ro)
{
	return (u32)(((u64) val * ep_ro) >> 32);
}
#endif /* LINUX_VERSION_IS_LESS(3,14,0) */

#if LINUX_VERSION_IS_LESS(3,18,0)
#define bin2hex LINUX_BACKPORT(bin2hex)
extern char *bin2hex(char *dst, const void *src, size_t count);
#endif

#endif /* __BACKPORT_KERNEL_H */

/*
 * We have to do this outside the include guard, because
 * out own header (linux/export.h) has to include kernel.h
 * indirectly (through module.h) and then undef's pr_fmt.
 * Then, when the real kernel.h gets included again, it's
 * not defined and we get problems ...
 */
#ifndef pr_fmt
#define pr_fmt(msg) msg
#endif

#ifndef __is_constexpr
#define __is_constexpr(x) \
	(sizeof(int) == sizeof(*(8 ? ((void *)((long)(x) * 0l)) : (int *)8)))
#endif

#define __BACKPORT_KERNEL_INCLUDE_FINISHED
