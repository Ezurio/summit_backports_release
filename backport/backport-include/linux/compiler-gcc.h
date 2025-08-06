#ifndef __BACKPORT_LINUX_COMPILER_GCC_H
#define __BACKPORT_LINUX_COMPILER_GCC_H
#include_next <linux/compiler-gcc.h>

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000           \
                     + __GNUC_MINOR__ * 100     \
                     + __GNUC_PATCHLEVEL__)
#endif

#ifndef __alias
#define __alias(symbol)		__attribute__((alias(#symbol)))
#endif

#if !defined(COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW) && (GCC_VERSION >= 50100)
#define COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW 1
#endif

#if GCC_VERSION < 40600
#ifndef static_assert
#define static_assert(x)
#endif
#endif

#endif
