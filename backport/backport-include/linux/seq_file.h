#ifndef __BACKPORTS_LINUX_SEQ_FILE_H
#define __BACKPORTS_LINUX_SEQ_FILE_H
#include_next <linux/seq_file.h>

#ifndef DEFINE_SHOW_ATTRIBUTE
#define DEFINE_SHOW_ATTRIBUTE(__name)					\
static int __name ## _open(struct inode *inode, struct file *file)	\
{									\
	return single_open(file, __name ## _show, inode->i_private);	\
}									\
									\
static const struct file_operations __name ## _fops = {			\
	.owner		= THIS_MODULE,					\
	.open		= __name ## _open,				\
	.read		= seq_read,					\
	.llseek		= seq_lseek,					\
	.release	= single_release,				\
}
#endif

#if LINUX_VERSION_IS_LESS(3,19,0)
static inline bool seq_has_overflowed(struct seq_file *m)
{
	return m->count == m->size;
}
#endif

#if LINUX_VERSION_IS_LESS(3,7,0)
static inline struct user_namespace *seq_user_ns(struct seq_file *seq)
{
	extern struct user_namespace init_user_ns;
	return &init_user_ns;
}
#endif

#endif /* __BACKPORTS_LINUX_SEQ_FILE_H */
