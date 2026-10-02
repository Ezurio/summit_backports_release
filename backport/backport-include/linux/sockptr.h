#ifndef _BACKPORT_LINUX_SOCKPTR_H
#define _BACKPORT_LINUX_SOCKPTR_H
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(5,4,0)
#define copy_from_sockptr(a,b,c) copy_from_user(a,b,c)
#define sockptr_t char __user *

#define copy_struct_from_sockptr LINUX_BACKPORT(copy_struct_from_sockptr)
static inline int copy_struct_from_sockptr(void *dst, size_t ksize,
		sockptr_t src, size_t usize)
{
	size_t size = ksize;

	/* Deal with trailing bytes. */
	if (usize < ksize) {
		size = usize;
		memset(dst + size, 0, ksize - usize);
	}
	/* Copy the interoperable parts of the struct. */
	if (copy_from_user(dst, src, size))
		return -EFAULT;
	return 0;
}
#elif LINUX_VERSION_IS_LESS(5,9,0)
#define copy_from_sockptr(a,b,c) copy_from_user(a,b,c)
#define sockptr_t char __user *

#define copy_struct_from_sockptr LINUX_BACKPORT(copy_struct_from_sockptr)
static inline int copy_struct_from_sockptr(void *dst, size_t ksize,
		sockptr_t src, size_t usize)
{
	size_t size = min(ksize, usize);

	return copy_struct_from_user(dst, ksize, src, size);
}
#else
#include_next <linux/sockptr.h>

#if LINUX_VERSION_IS_LESS(6,7,0)
#define copy_struct_from_sockptr LINUX_BACKPORT(copy_struct_from_sockptr)
static inline int copy_struct_from_sockptr(void *dst, size_t ksize,
		sockptr_t src, size_t usize)
{
	size_t size = min(ksize, usize);
	size_t rest = max(ksize, usize) - size;

	if (!sockptr_is_kernel(src))
		return copy_struct_from_user(dst, ksize, src.user, size);

	if (usize < ksize) {
		memset(dst + size, 0, rest);
	} else if (usize > ksize) {
		char *p = src.kernel;

		while (rest--) {
			if (*p++)
				return -E2BIG;
		}
	}
	memcpy(dst, src.kernel, size);
	return 0;
}
#endif
#endif

#if LINUX_VERSION_IS_LESS(6,8,0) && \
    !LINUX_VERSION_IN_RANGE(6,6,47, 6,7,0) && \
    !LINUX_VERSION_IN_RANGE(6,1,119, 6,2,0)
#define copy_safe_from_sockptr LINUX_BACKPORT(copy_safe_from_sockptr)
static inline int copy_safe_from_sockptr(void *dst, size_t ksize,
					 sockptr_t optval, unsigned int optlen)
{
	if (optlen < ksize)
		return -EINVAL;
	if (copy_from_sockptr(dst, optval, ksize))
		return -EFAULT;
	return 0;
}
#endif

#endif /* _BACKPORT_LINUX_SOCKPTR_H */
