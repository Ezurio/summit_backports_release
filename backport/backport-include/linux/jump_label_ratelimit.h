/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORT_LINUX_JUMP_LABEL_RATELIMIT_H
#define __BACKPORT_LINUX_JUMP_LABEL_RATELIMIT_H

#if LINUX_VERSION_IS_GEQ(4,3,0)
#include_next <linux/jump_label_ratelimit.h>
#else /* kernels < 4.3 */
#define _LINUX_JUMP_LABEL_RATELIMIT_H

/* Assume !HAVE_JUMP_LABEL compatibility layer - see backport-include/linux/jump_label.h */
struct static_key_deferred {
	struct static_key  key;
};
static inline void static_key_slow_dec_deferred(struct static_key_deferred *key)
{
	STATIC_KEY_CHECK_USE();
	static_key_slow_dec(&key->key);
}
static inline void static_key_deferred_flush(void *key)
{
	STATIC_KEY_CHECK_USE();
}
static inline void
jump_label_rate_limit(struct static_key_deferred *key,
		unsigned long rl)
{
	STATIC_KEY_CHECK_USE();
}

#define static_branch_deferred_inc(x)	static_branch_inc(&(x)->key)

#endif /* kernels < 4.3 */

#endif	/* __BACKPORT_LINUX_JUMP_LABEL_RATELIMIT_H */
