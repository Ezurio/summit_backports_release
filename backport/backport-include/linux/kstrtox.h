#ifndef _BACKPORT_LINUX_KSTRTOX_H
#define _BACKPORT_LINUX_KSTRTOX_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(5,14,0) || \
    LINUX_VERSION_IN_RANGE(5,10,185, 5,11,0)
#include_next <linux/kstrtox.h>
#else
#include <linux/kernel.h>

#if LINUX_VERSION_IS_LESS(4,6,0)
#define kstrtobool LINUX_BACKPORT(kstrtobool)
int __must_check kstrtobool(const char *s, bool *res);
#define kstrtobool_from_user LINUX_BACKPORT(kstrtobool_from_user)
int __must_check kstrtobool_from_user(const char __user *s, size_t count, bool *res);
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

#endif

#endif
