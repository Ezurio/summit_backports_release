#include <linux/device.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/debugfs.h>

int debugfs_change_name(struct dentry *dentry, const char *fmt, ...)
{
	char *new_name;
	struct dentry *dirn;
	va_list ap;

	if (IS_ERR_OR_NULL(dentry))
		return 0;

	va_start(ap, fmt);
	new_name = kvasprintf(GFP_KERNEL, fmt, ap);
	va_end(ap);

	if (!new_name)
		return -ENOMEM;

	dirn = debugfs_rename(dentry->d_parent, dentry, dentry->d_parent, new_name);
	kfree(new_name);

	return IS_ERR(dirn) ? PTR_ERR(dirn) : 0;
}
EXPORT_SYMBOL_GPL(debugfs_change_name);
