#ifndef __BACKPORT_LINUX_LIMITS_H
#define __BACKPORT_LINUX_LIMITS_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(5,1,0) || LINUX_VERSION_IS_LESS(3,7,0)
#include_next <linux/limits.h>
#else
#include <uapi/linux/limits.h>
#include <linux/kernel.h>
#endif

#ifndef SIZE_MAX
#define SIZE_MAX    (~(size_t)0)
#endif

#ifndef USHRT_MAX
#define USHRT_MAX      ((u16)(~0U))
#endif

#ifndef SHRT_MAX
#define SHRT_MAX       ((s16)(USHRT_MAX>>1))
#endif

#ifndef SHRT_MIN
#define SHRT_MIN       ((s16)(-SHRT_MAX - 1))
#endif

#ifndef U8_MAX
#define U8_MAX		((u8)~0U)
#endif

#ifndef S8_MAX
#define S8_MAX		((s8)(U8_MAX>>1))
#endif

#ifndef S8_MIN
#define S8_MIN		((s8)(-S8_MAX - 1))
#endif

#ifndef U16_MAX
#define U16_MAX		((u16)~0U)
#endif

#ifndef S16_MAX
#define S16_MAX		((s16)(U16_MAX>>1))
#endif

#ifndef S16_MIN
#define S16_MIN		((s16)(-S16_MAX - 1))
#endif

#ifndef U32_MAX
#define U32_MAX		((u32)~0U)
#endif

#ifndef S32_MAX
#define S32_MAX		((s32)(U32_MAX>>1))
#endif

#ifndef S32_MIN
#define S32_MIN		((s32)(-S32_MAX - 1))
#endif

#ifndef U64_MAX
#define U64_MAX		((u64)~0ULL)
#endif

#ifndef S64_MAX
#define S64_MAX		((s64)(U64_MAX>>1))
#endif

#ifndef S64_MIN
#define S64_MIN		((s64)(-S64_MAX - 1))
#endif

#endif /* __BACKPORT_LINUX_LIMITS_H */
