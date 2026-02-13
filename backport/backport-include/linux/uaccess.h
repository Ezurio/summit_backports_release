#ifndef __BACKPORT_LINUX_UACCESS_H
#define __BACKPORT_LINUX_UACCESS_H
#include_next <linux/uaccess.h>

#include <linux/version.h>
#if LINUX_VERSION_IS_LESS(5,4,0)
#include <linux/minmax.h>
#define check_zeroed_user LINUX_BACKPORT(check_zeroed_user)
int check_zeroed_user(const void __user *from, size_t size);

#define copy_struct_from_user LINUX_BACKPORT(copy_struct_from_user)
static inline int
copy_struct_from_user(void *dst, size_t ksize, const void __user *src,
		      size_t usize)
{
	size_t size = min(ksize, usize);
	size_t rest = max(ksize, usize) - size;

	/* Deal with trailing bytes. */
	if (usize < ksize) {
		memset(dst + size, 0, rest);
	} else if (usize > ksize) {
		int ret = check_zeroed_user(src + size, rest);
		if (ret <= 0)
			return ret ?: -E2BIG;
	}
	/* Copy the interoperable parts of the struct. */
	if (copy_from_user(dst, src, size))
		return -EFAULT;
	return 0;
}
#endif

#endif /* __BACKPORT_LINUX_UACCESS_H */
