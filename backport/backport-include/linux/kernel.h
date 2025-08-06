#ifndef __BACKPORT_LINUX_KERNEL_H
#define __BACKPORT_LINUX_KERNEL_H

#include_next <linux/kernel.h>

/*
 * some older kernels don't have this and thus don't
 * include it from kernel.h like new kernels
 */
#include <linux/hex.h>
#include <linux/kstrtox.h>
#include <linux/math.h>
#include <linux/minmax.h>
#include <linux/printk.h>
#include <linux/build_bug.h>

#ifndef PTR_IF
#define PTR_IF(cond, ptr)	((cond) ? (ptr) : NULL)
#endif

#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a)	((typeof(x))ALIGN((x), (a)) - (a))
#endif

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

#endif /* __BACKPORT_LINUX_KERNEL_H */

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

#define __BACKPORT_KERNEL_INCLUDE_FINISHED
