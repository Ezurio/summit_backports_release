#ifndef __BP_LINUX_ASM_IO_H
#define __BP_LINUX_ASM_IO_H
#include_next <asm/io.h>

#ifndef writel_relaxed
#define writel_relaxed writel_relaxed
static inline void writel_relaxed(u32 value, volatile void __iomem *addr)
{
	__raw_writel(__cpu_to_le32(value), addr);
}
#endif

#endif /* __BP_LINUX_ASM_IO_H */
