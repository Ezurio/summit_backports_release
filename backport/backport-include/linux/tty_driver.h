#ifndef __BACKPORT_TTY_DRIVER_H
#define __BACKPORT_TTY_DRIVER_H
#include_next <linux/tty_driver.h>

#if LINUX_VERSION_IS_LESS(3,7,0)

/* Use TTY_DRIVER_* flags below */

#if LINUX_VERSION_IS_LESS(3,4,0)
#define tty_alloc_driver(lines, flags) \
		alloc_tty_driver(lines)
#else
#define tty_alloc_driver(lines, flags) \
		__alloc_tty_driver(lines, THIS_MODULE)
#endif
#endif /* < 3.7 */

#endif /* __BACKPORT_TTY_DRIVER_H */
