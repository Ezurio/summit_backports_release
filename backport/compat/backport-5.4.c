#include <linux/uaccess.h>
#include <linux/export.h>

#if LINUX_VERSION_IS_LESS(5,0,0)
#undef user_access_begin
#define user_access_begin(ptr, size) access_ok(VERIFY_READ, ptr, size)
#endif

#ifndef user_access_end
#define user_access_end() do { } while (0)
#endif

#if LINUX_VERSION_IS_LESS(4,8,0)
#undef unsafe_get_user
#define unsafe_get_user(x, ptr, err) do { if (unlikely(__get_user(x, ptr))) goto err; } while (0)
#endif

int check_zeroed_user(const void __user *from, size_t size)
{
	unsigned long val;
	uintptr_t align = (uintptr_t) from % sizeof(unsigned long);

	if (unlikely(size == 0))
		return 1;

	from -= align;
	size += align;

	if (!user_access_begin(from, size))
		return -EFAULT;

	unsafe_get_user(val, (unsigned long __user *) from, err_fault);
	if (align)
		val &= ~aligned_byte_mask(align);

	while (size > sizeof(unsigned long)) {
		if (unlikely(val))
			goto done;

		from += sizeof(unsigned long);
		size -= sizeof(unsigned long);

		unsafe_get_user(val, (unsigned long __user *) from, err_fault);
	}

	if (size < sizeof(unsigned long))
		val &= aligned_byte_mask(size);

done:
	user_access_end();
	return (val == 0);
err_fault:
	user_access_end();
	return -EFAULT;
}
EXPORT_SYMBOL(check_zeroed_user);
