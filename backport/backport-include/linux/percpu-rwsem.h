/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORTS_LINUX_PERCPU_RWSEM_H
#define __BACKPORTS_LINUX_PERCPU_RWSEM_H

#if LINUX_VERSION_IS_GEQ(3,7,0)
#include_next <linux/percpu-rwsem.h>
#else
struct percpu_rw_semaphore {
	unsigned dummy;
};
#endif

#if LINUX_VERSION_IS_LESS(4,7,0)
#undef percpu_init_rwsem
#define percpu_init_rwsem(x) 0
#define percpu_free_rwsem(x)
#define percpu_down_read_trylock(x) true
#define percpu_up_read(x)
#define percpu_up_write(x)
#define percpu_down_read(x)
#define percpu_down_write(x)
#elif LINUX_VERSION_IS_LESS(4,9,0)
#define percpu_down_read_trylock(x) true
#endif

#endif
