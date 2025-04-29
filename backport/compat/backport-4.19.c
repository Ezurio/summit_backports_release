#include <linux/export.h>
#include <linux/slab.h>

#if !LINUX_VERSION_IN_RANGE(4,9,212, 4,10,0) && \
    !LINUX_VERSION_IN_RANGE(4,4,212, 4,5,0)

unsigned long *bitmap_alloc(unsigned int nbits, gfp_t flags)
{
	return kmalloc_array(BITS_TO_LONGS(nbits), sizeof(unsigned long),
			     flags);
}
EXPORT_SYMBOL(bitmap_alloc);

unsigned long *bitmap_zalloc(unsigned int nbits, gfp_t flags)
{
	return bitmap_alloc(nbits, flags | __GFP_ZERO);
}
EXPORT_SYMBOL(bitmap_zalloc);

void bitmap_free(const unsigned long *bitmap)
{
	kfree(bitmap);
}
EXPORT_SYMBOL(bitmap_free);

#endif
