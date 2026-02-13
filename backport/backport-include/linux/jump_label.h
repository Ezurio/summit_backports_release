/* The 2.6 kernel has an issue with include order due to kernel.h including bitops, the x86 implementation
 * of which then includes alternative.h which then includes jump_label.h, PRIOR to the kernel defining
 * inline methods necessary for atomic.h's inline methods which reference them.
 * As a work around, try to prohibit inclusion prior to kernel.h's header finishing processing.
 * In the 2.6 kernel jump_label had little content, and we can probably get away without the
 * jump_label that alternative.h wanted.
 * An alternate approach might be to simply backport the newer jump_label support with new names, and patch
 * the WiFi and BT code to reference the new names and new header names.  The newer jump label structures
 * cannot readily be passed to 2.6 and 3.x kernel functions anyways.
 */
#if (!defined(__BACKPORT_LINUX_JUMP_LABEL_H)) && (defined(__BACKPORT_KERNEL_INCLUDE_FINISHED) || LINUX_VERSION_IS_GEQ(3,0,0))
#define __BACKPORT_LINUX_JUMP_LABEL_H

#ifndef DECLARE_STATIC_KEY_FALSE
#define DECLARE_STATIC_KEY_FALSE(name)	\
	extern struct static_key_false name
#endif

#if LINUX_VERSION_IS_LESS(3,4,0)
/* Provide some of the old-style names with the new implementation, such that
 * old-style routines will still build.
 * Hopefully, they will link too, since jump labels _can_ be entirely implemented
 * in headers, as we've done.
 * The risk would be if linking to older kernel code and sharing the jump labels...
 * It's sortof a damned-if-you-do, damned-if-you-don't situation, however, with
 * the expectations of the new drivers, particularly with regard to the jump label
 * structures, requiring excessive cleverness to meet with the old jump label implementation,
 * and the expectations of the older kernel headers / kernel code...
 * Take a look at usage in perf_sw_event between revs.
 */
#define jump_label_key static_key
#define jump_label_key_deferred static_key_deferred
#define static_branch static_key_false
#define JUMP_LABEL_INIT {ATOMIC_INIT(0)}
#endif

#if LINUX_VERSION_IS_GEQ(4,3,0)
#include_next <linux/jump_label.h>
#else /* kernels < 4.3 */
#define _LINUX_JUMP_LABEL_H

/* The 4.2 kernel's jump label support can for the most part provide the
 * functionality which is present in kernels 4.3 - 4.15, albeit with a slightly
 * different interface, but because the structure changed quite a bit between
 * 2.6 and 4.2, it is simpler to just provide a non-performant compatibility
 * layer for 4.2 and earlier kernels.  This is what we provide here and in
 * jump_label_ratelimit.h, a backporting of the non-arch-specific compatibility
 * implementation of the jump-label API from 4.3+.
 */

/*
 * Jump label support
 *
 * Copyright (C) 2009-2012 Jason Baron <jbaron@redhat.com>
 * Copyright (C) 2011-2012 Peter Zijlstra <pzijlstr@redhat.com>
 *
 * DEPRECATED API:
 *
 * The use of 'struct static_key' directly, is now DEPRECATED. In addition
 * static_key_{true,false}() is also DEPRECATED. IE DO NOT use the following:
 *
 * struct static_key false = STATIC_KEY_INIT_FALSE;
 * struct static_key true = STATIC_KEY_INIT_TRUE;
 * static_key_true()
 * static_key_false()
 *
 * The updated API replacements are:
 *
 * DEFINE_STATIC_KEY_TRUE(key);
 * DEFINE_STATIC_KEY_FALSE(key);
 * static_branch_likely()
 * static_branch_unlikely()
 *
 * Jump labels provide an interface to generate dynamic branches using
 * self-modifying code. Assuming toolchain and architecture support, if we
 * define a "key" that is initially false via "DEFINE_STATIC_KEY_FALSE(key)",
 * an "if (static_branch_unlikely(&key))" statement is an unconditional branch
 * (which defaults to false - and the true block is placed out of line).
 * Similarly, we can define an initially true key via
 * "DEFINE_STATIC_KEY_TRUE(key)", and use it in the same
 * "if (static_branch_unlikely(&key))", in which case we will generate an
 * unconditional branch to the out-of-line true branch. Keys that are
 * initially true or false can be using in both static_branch_unlikely()
 * and static_branch_likely() statements.
 *
 * At runtime we can change the branch target by setting the key
 * to true via a call to static_branch_enable(), or false using
 * static_branch_disable(). If the direction of the branch is switched by
 * these calls then we run-time modify the branch target via a
 * no-op -> jump or jump -> no-op conversion. For example, for an
 * initially false key that is used in an "if (static_branch_unlikely(&key))"
 * statement, setting the key to true requires us to patch in a jump
 * to the out-of-line of true branch.
 *
 * In addition to static_branch_{enable,disable}, we can also reference count
 * the key or branch direction via static_branch_{inc,dec}. Thus,
 * static_branch_inc() can be thought of as a 'make more true' and
 * static_branch_dec() as a 'make more false'.
 *
 * Since this relies on modifying code, the branch modifying functions
 * must be considered absolute slow paths (machine wide synchronization etc.).
 * OTOH, since the affected branches are unconditional, their runtime overhead
 * will be absolutely minimal, esp. in the default (off) case where the total
 * effect is a single NOP of appropriate size. The on case will patch in a jump
 * to the out-of-line block.
 *
 * When the control is directly exposed to userspace, it is prudent to delay the
 * decrement to avoid high frequency code modifications which can (and do)
 * cause significant performance degradation. Struct static_key_deferred and
 * static_key_slow_dec_deferred() provide for this.
 *
 * Lacking toolchain and or architecture support, static keys fall back to a
 * simple conditional branch.
 *
 * Additional babbling in: Documentation/static-keys.txt
 */

/* do not define HAVE_JUMP_LABEL, no arch-specific implementation */

#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/bug.h>

/* Because we are not really implementing jump tables for < 4.3 kernels, jump_label_init()
 * function does very little, and therefore validation check is not necessary in the case
 * of simply backporting functionality.
 */
#define STATIC_KEY_CHECK_USE() do { } while (0)

struct static_key {
	atomic_t enabled;
};

enum jump_label_type {
	JUMP_LABEL_NOP = 0,
	JUMP_LABEL_JMP,
};

struct module;

#include <linux/atomic.h>

static inline int static_key_count(struct static_key *key)
{
	return atomic_read(&key->enabled);
}

static __always_inline void jump_label_init(void)
{
	/* No init in kernel 4.3 !HAVE_JUMP_LABEL case. */
}

static __always_inline bool static_key_false(struct static_key *key)
{
	if (unlikely(static_key_count(key) > 0))
		return true;
	return false;
}

static __always_inline bool static_key_true(struct static_key *key)
{
	if (likely(static_key_count(key) > 0))
		return true;
	return false;
}

static inline void static_key_slow_inc(struct static_key *key)
{
	STATIC_KEY_CHECK_USE();
	atomic_inc(&key->enabled);
}

static inline void static_key_slow_dec(struct static_key *key)
{
	STATIC_KEY_CHECK_USE();
	atomic_dec(&key->enabled);
}

static inline int jump_label_text_reserved(void *start, void *end)
{
	return 0;
}

static inline void jump_label_lock(void) {}
static inline void jump_label_unlock(void) {}

static inline int jump_label_apply_nops(struct module *mod)
{
	return 0;
}

#define STATIC_KEY_INIT_TRUE	{ .enabled = ATOMIC_INIT(1) }
#define STATIC_KEY_INIT_FALSE	{ .enabled = ATOMIC_INIT(0) }

#define STATIC_KEY_INIT STATIC_KEY_INIT_FALSE
#define jump_label_enabled static_key_enabled

static inline bool static_key_enabled(struct static_key *key)
{
	return static_key_count(key) > 0;
}

static inline void static_key_enable(struct static_key *key)
{
	int count = static_key_count(key);

	WARN_ON_ONCE(count < 0 || count > 1);

	if (!count)
		static_key_slow_inc(key);
}

static inline void static_key_disable(struct static_key *key)
{
	int count = static_key_count(key);

	WARN_ON_ONCE(count < 0 || count > 1);

	if (count)
		static_key_slow_dec(key);
}

/* -------------------------------------------------------------------------- */

/*
 * Two type wrappers around static_key, such that we can use compile time
 * type differentiation to emit the right code.
 *
 * All the below code is macros in order to play type games.
 */

struct static_key_true {
	struct static_key key;
};

struct static_key_false {
	struct static_key key;
};

#define STATIC_KEY_TRUE_INIT  (struct static_key_true) { .key = STATIC_KEY_INIT_TRUE,  }
#define STATIC_KEY_FALSE_INIT (struct static_key_false){ .key = STATIC_KEY_INIT_FALSE, }

#define DEFINE_STATIC_KEY_TRUE(name)	\
	struct static_key_true name = STATIC_KEY_TRUE_INIT

#define DEFINE_STATIC_KEY_FALSE(name)	\
	struct static_key_false name = STATIC_KEY_FALSE_INIT

#define static_branch_likely(x)		likely(static_key_enabled(&(x)->key))
#define static_branch_unlikely(x)	unlikely(static_key_enabled(&(x)->key))

/*
 * Advanced usage; refcount, branch is enabled when: count != 0
 */

#define static_branch_inc(x)		static_key_slow_inc(&(x)->key)
#define static_branch_dec(x)		static_key_slow_dec(&(x)->key)

/*
 * Normal usage; boolean enable/disable.
 */

#define static_branch_enable(x)		static_key_enable(&(x)->key)
#define static_branch_disable(x)	static_key_disable(&(x)->key)

#endif /* kernels < 4.3 */

#if LINUX_VERSION_IS_LESS(3,12,0)
/* Older headers (< 3.12) may assume features moved to jump_label_ratelimit.h
 * are still present in jump_label.h, therefore include ratelimit if
 * compiling for an older kernel.
 */
#include <linux/jump_label_ratelimit.h>
#endif /* kernels < 3.12 */

#if LINUX_VERSION_IS_LESS(3,0,0)
/* WARNING - because old jump_label compatibility IS NOT provided by this
 * implementation, the following COND_STMT definition may not work.
 * Provided only in case linux/perf_event.h gets included, to fix build-time
 * errors, with no expectation of runtime compatibility.
 */
#define COND_STMT(key, stmt)					\
do {								\
	__label__ jl_enabled;					\
	JUMP_LABEL(key, jl_enabled);				\
	if (0) {						\
jl_enabled:							\
		stmt;						\
	}							\
} while (0)
#endif

#endif /* __BACKPORT_LINUX_JUMP_LABEL_H */
