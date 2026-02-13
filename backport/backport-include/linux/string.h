#ifndef __BACKPORT_LINUX_STRING_H
#define __BACKPORT_LINUX_STRING_H
#include_next <linux/string.h>
#include <linux/version.h>
#include <linux/stddef.h>
#include <linux/overflow.h>

#if LINUX_VERSION_IS_LESS(6,9,0)
#define kmemdup_array LINUX_BACKPORT(kmemdup_array)
static inline
void *kmemdup_array(const void *src, size_t count, size_t element_size, gfp_t gfp)
{
	return kmemdup(src, size_mul(element_size, count), gfp);
}
#endif

#if LINUX_VERSION_IS_LESS(5,2,0)
#define strscpy_pad LINUX_BACKPORT(strscpy_pad)
ssize_t strscpy_pad(char *dest, const char *src, size_t count);
#endif

#ifndef memset_startat
/**
 * memset_startat - Set a value starting at a member to the end of a struct
 *
 * @obj: Address of target struct instance
 * @v: Byte value to repeatedly write
 * @member: struct member to start writing at
 *
 * Note that if there is padding between the prior member and the target
 * member, memset_after() should be used to clear the prior padding.
 */
#define memset_startat(obj, v, member)					\
({									\
	u8 *__ptr = (u8 *)(obj);					\
	typeof(v) __val = (v);						\
	memset(__ptr + offsetof(typeof(*(obj)), member), __val,		\
	       sizeof(*(obj)) - offsetof(typeof(*(obj)), member));	\
})
#endif

#if LINUX_VERSION_IS_LESS(4,14,0)
#define memcpy_and_pad LINUX_BACKPORT(memcpy_and_pad)
static inline void memcpy_and_pad(void *dest, size_t dest_len,
				  const void *src, size_t count, int pad)
{
	if (dest_len > count) {
		memcpy(dest, src, count);
		memset(dest + count, pad,  dest_len - count);
	} else
		memcpy(dest, src, dest_len);
}
#endif

#ifndef memset_after
/**
 * memset_after - Set a value after a struct member to the end of a struct
 *
 * @obj: Address of target struct instance
 * @v: Byte value to repeatedly write
 * @member: after which struct member to start writing bytes
 *
 * This is good for clearing padding following the given member.
 */
#define memset_after(obj, v, member)					\
({									\
	u8 *__ptr = (u8 *)(obj);					\
	typeof(v) __val = (v);						\
	memset(__ptr + offsetofend(typeof(*(obj)), member), __val,	\
	       sizeof(*(obj)) - offsetofend(typeof(*(obj)), member));	\
})
#endif

#if LINUX_VERSION_IS_LESS(4,14,0)
#define memset32 LINUX_BACKPORT(memset32)
extern void *memset32(uint32_t *, uint32_t, __kernel_size_t);
#endif

#if LINUX_VERSION_IS_LESS(4,5,0)
#define memdup_user_nul LINUX_BACKPORT(memdup_user_nul)
extern void *memdup_user_nul(const void __user *, size_t);
#endif

/* this was added in v3.2.65, v3.4.106, v3.10.60, v3.12.33, v3.14.24,
 * v3.17.3 and v3.18 */
#if !(LINUX_VERSION_IS_GEQ(3,17,3) || \
      (LINUX_VERSION_IS_GEQ(3,14,24) && \
      LINUX_VERSION_IS_LESS(3,15,0)) || \
      (LINUX_VERSION_IS_GEQ(3,12,33) && \
      LINUX_VERSION_IS_LESS(3,13,0)) || \
      (LINUX_VERSION_IS_GEQ(3,10,60) && \
      LINUX_VERSION_IS_LESS(3,11,0)) || \
      (LINUX_VERSION_IS_GEQ(3,4,106) && \
      LINUX_VERSION_IS_LESS(3,5,0)) || \
      (LINUX_VERSION_IS_GEQ(3,2,65) && \
      LINUX_VERSION_IS_LESS(3,3,0)))
#define memzero_explicit LINUX_BACKPORT(memzero_explicit)
void memzero_explicit(void *s, size_t count);
#endif

#if LINUX_VERSION_IS_LESS(4,3,0)
ssize_t strscpy(char *dest, const char *src, size_t count);
#endif

#if LINUX_VERSION_IS_LESS(4,2,0)
#define strreplace LINUX_BACKPORT(strreplace)
char *strreplace(char *s, char old, char new);
#endif

#if LINUX_VERSION_IS_LESS(4,6,0)
#define match_string LINUX_BACKPORT(match_string)
int match_string(const char * const *array, size_t n, const char *string);
#endif /* LINUX_VERSION_IS_LESS(4,6,0) */

#if LINUX_VERSION_IS_LESS(3,16,0) && !defined(HAVE_STRCHRNUL)
#define strchrnul LINUX_BACKPORT(strchrnul)
static inline char *strchrnul(const char *s, int c)
{
	while (*s && *s != c)
		s++;
	return (char *)s;
}
#endif

#if LINUX_VERSION_IS_LESS(3,0,0)
extern int strtobool(const char *s, bool *res);
#endif

#ifndef unsafe_memcpy
#define unsafe_memcpy(dst, src, bytes, justification)		memcpy(dst, src, bytes)
#endif

#endif /* __BACKPORT_LINUX_STRING_H */
