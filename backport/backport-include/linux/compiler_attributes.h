#ifndef _BACKPORTS_LINUX_COMPILER_ATTRIBUTES_H
#define _BACKPORTS_LINUX_COMPILER_ATTRIBUTES_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(4,20,0)
#include_next <linux/compiler_attributes.h>
#endif

#ifndef __has_attribute
# define __has_attribute(x) 0
#endif

/*
 * Add the pseudo keyword 'fallthrough' so case statement blocks
 * must end with any of these keywords:
 *   break;
 *   fallthrough;
 *   goto <label>;
 *   return [expression];
 *
 *  gcc: https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html#Statement-Attributes
 */
#ifndef fallthrough
#if __has_attribute(__fallthrough__)
# define fallthrough                    __attribute__((__fallthrough__))
#else
# define fallthrough                    do {} while (0)  /* fallthrough */
#endif
#endif /* fallthrough */

#ifndef __nonstring
#if __has_attribute(__nonstring__)
# define __nonstring                    __attribute__((__nonstring__))
#else
# define __nonstring
#endif
#endif /* __nonstring */

#if LINUX_VERSION_IS_LESS(6,10,0) && !LINUX_VERSION_IN_RANGE(6,6,64, 6,7,0)
#ifndef __counted_by

#if __has_attribute(__counted_by__)
# define __counted_by(member)		__attribute__((__counted_by__(member)))
#else
# define __counted_by(member)
#endif
#endif /* __counted_by */
#endif /* LINUX_VERSION_IS_LESS(6,10,0) */

#ifndef __cleanup
#if __has_attribute(__cleanup__)
# define __cleanup(func)			__attribute__((__cleanup__(func)))
#else
# define __cleanup(func)
#endif
#endif /* __cleanup */

#endif /* _BACKPORTS_LINUX_COMPILER_ATTRIBUTES_H */
