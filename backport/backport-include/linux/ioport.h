#ifndef __BACKPORT_LINUX_IOPORT_H
#define __BACKPORT_LINUX_IOPORT_H
#include_next <linux/ioport.h>
#include <linux/version.h>

#ifndef IORESOURCE_REG
#define IORESOURCE_REG		0x00000300
#endif

#if LINUX_VERSION_IS_LESS(4,6,0)
#undef DEFINE_RES_NAMED
#define DEFINE_RES_NAMED(_start, _size, _name, _flags)			\
(struct resource) {							\
		.start = (_start),					\
		.end = (_start) + (_size) - 1,				\
		.name = (_name),					\
		.flags = (_flags),					\
	}
#elif LINUX_VERSION_IS_LESS(6,2,0)
#undef DEFINE_RES_NAMED
#define DEFINE_RES_NAMED(_start, _size, _name, _flags)			\
(struct resource) {							\
		.start = (_start),					\
		.end = (_start) + (_size) - 1,				\
		.name = (_name),					\
		.flags = (_flags),					\
		.desc = IORES_DESC_NONE,				\
	}
#endif

#endif /* __BACKPORT_LINUX_IOPORT_H */
