/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORT_LINUX_KOBJECT_H
#define __BACKPORT_LINUX_KOBJECT_H

#if LINUX_VERSION_IN_RANGE(4,4,0, 4,10,0)
#include <linux/atomic.h>

#ifndef atomic_read
/* To remove copious compile warnings of "expected ‘const atomic_t *’ {aka
 * ‘const struct <anonymous> *’} but argument is of type ‘refcount_t *’ {aka
 * ‘struct refcount_struct *’}", just cast to const atomic_t *.
 */
#define atomic_read(kref)  atomic_read((const atomic_t *) kref)

#include_next <linux/kobject.h>

#undef atomic_read
#else

#include_next <linux/kobject.h>

#endif
#else
#include_next <linux/kobject.h>
#endif

#endif	/* __BACKPORT_LINUX_KOBJECT_H */
