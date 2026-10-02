#ifndef __BACKPORT_LINUX_LOCKDEP_H
#define __BACKPORT_LINUX_LOCKDEP_H
#include_next <linux/lockdep.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(3,14,0)
#define lock_acquire_exclusive(l, s, t, n, i)          lock_acquire(l, s, t, 0, 1, n, i)
#endif

#if LINUX_VERSION_IS_LESS(3,9,0)
#undef lockdep_assert_held
#ifdef CONFIG_LOCKDEP
#define lockdep_assert_held(l)	do {				\
		WARN_ON(debug_locks && !lockdep_is_held(l));	\
	} while (0)
#else
#define lockdep_assert_held(l)			do { (void)(l); } while (0)
#endif /* CONFIG_LOCKDEP */
#endif /* LINUX_VERSION_IS_LESS(3,9,0) */

#if LINUX_VERSION_IS_LESS(4,15,0)
#ifndef CONFIG_LOCKDEP
struct lockdep_map { };
#endif /* CONFIG_LOCKDEP */
#endif /* LINUX_VERSION_IS_LESS(4,15,0) */

#ifndef lockdep_assert_not_held
#ifdef CONFIG_LOCKDEP
#ifndef LOCK_STATE_HELD
#define LOCK_STATE_HELD		1
#endif /* LOCK_STATE_HELD */
#define lockdep_assert_not_held(l)	do {				\
		WARN_ON(debug_locks &&					\
			lockdep_is_held(l) == LOCK_STATE_HELD);		\
	} while (0)
#else
#define lockdep_assert_not_held(l)		do { (void)(l); } while (0)
#endif /* CONFIG_LOCKDEP */
#endif /* lockdep_assert_not_held */

#if LINUX_VERSION_IS_LESS(3,12,0)
#ifdef CONFIG_PROVE_LOCKING
	#define lock_acquire_exclusive(l, s, t, n, i) lock_acquire(l, s, t, 0, 2, n, i)
#else
	#define lock_acquire_exclusive(l, s, t, n, i) lock_acquire(l, s, t, 0, 1, n, i)
#endif
#endif /* LINUX_VERSION_IS_LESS(3,12,0) */


#if LINUX_VERSION_IS_LESS(5,11,0) && !defined(CONFIG_LOCKDEP)
#ifndef lock_is_held
extern int lock_is_held(const void *);
#endif
#ifndef lockdep_is_held
extern int lockdep_is_held(const void *);
#endif
#endif

#endif /* __BACKPORT_LINUX_LOCKDEP_H */
