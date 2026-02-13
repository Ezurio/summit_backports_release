#ifndef __BACKPORT_LINUX_DMA_MAPPING_H
#define __BACKPORT_LINUX_DMA_MAPPING_H
#include_next <linux/dma-mapping.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(3,2,0)
#define dma_zalloc_coherent LINUX_BACKPORT(dma_zalloc_coherent)
static inline void *dma_zalloc_coherent(struct device *dev, size_t size,
					dma_addr_t *dma_handle, gfp_t flag)
{
	void *ret = dma_alloc_coherent(dev, size, dma_handle, flag);
	if (ret)
		memset(ret, 0, size);
	return ret;
}
#endif

#if LINUX_VERSION_IS_LESS(3,13,0)
/*
 * Set both the DMA mask and the coherent DMA mask to the same thing.
 * Note that we don't check the return value from dma_set_coherent_mask()
 * as the DMA API guarantees that the coherent DMA mask can be set to
 * the same or smaller than the streaming DMA mask.
 */
#define dma_set_mask_and_coherent LINUX_BACKPORT(dma_set_mask_and_coherent)
static inline int dma_set_mask_and_coherent(struct device *dev, u64 mask)
{
	int rc = dma_set_mask(dev, mask);
	if (rc == 0)
		dma_set_coherent_mask(dev, mask);
	return rc;
}
#endif /* LINUX_VERSION_IS_LESS(3,13,0) */

#ifndef DMA_MAPPING_ERROR
/*
 * A dma_addr_t can hold any valid DMA or bus address for the platform.  It can
 * be given to a device to use as a DMA source or target.  It is specific to a
 * given device and there may be a translation between the CPU physical address
 * space and the bus address space.
 *
 * DMA_MAPPING_ERROR is the magic error code if a mapping failed.  It should not
 * be used directly in drivers, but checked for using dma_mapping_error()
 * instead.
 */
#define DMA_MAPPING_ERROR		(~(dma_addr_t)0)
#endif /* DMA_MAPPING_ERROR */

#if LINUX_VERSION_IN_RANGE(4,8,0, 4,10,0)
#define dma_map_page_attrs LINUX_BACKPORT(dma_map_page_attrs)
static inline dma_addr_t dma_map_page_attrs(struct device *dev,
	struct page *page,
	size_t offset, size_t size,
	enum dma_data_direction dir,
	unsigned long attrs)
{
	struct dma_map_ops *ops = get_dma_ops(dev);
	dma_addr_t addr;

	kmemcheck_mark_initialized(page_address(page) + offset, size);
	BUG_ON(!valid_dma_direction(dir));
	addr = ops->map_page(dev, page, offset, size, dir, attrs);
	debug_dma_map_page(dev, page, offset, size, dir, addr, false);

	return addr;
}
#endif /* LINUX_VERSION_IN_RANGE(4,8,0, 4,10,0) */

#if LINUX_VERSION_IN_RANGE(4,8,0, 5,4,233)
#define dma_map_sgtable LINUX_BACKPORT(dma_map_sgtable)
static inline int dma_map_sgtable(struct device *dev, struct sg_table *sgt,
		enum dma_data_direction dir, unsigned long attrs)
{
	int nents;

	nents = dma_map_sg_attrs(dev, sgt->sgl, sgt->orig_nents, dir, attrs);
	if (nents <= 0)
		return -EINVAL;
	sgt->nents = nents;
	return 0;
}

#define dma_unmap_sgtable LINUX_BACKPORT(dma_unmap_sgtable)
static inline void dma_unmap_sgtable(struct device *dev, struct sg_table *sgt,
		enum dma_data_direction dir, unsigned long attrs)
{
	dma_unmap_sg_attrs(dev, sgt->sgl, sgt->orig_nents, dir, attrs);
}

#define dma_sync_sgtable_for_cpu LINUX_BACKPORT(dma_sync_sgtable_for_cpu)
static inline void dma_sync_sgtable_for_cpu(struct device *dev,
		struct sg_table *sgt, enum dma_data_direction dir)
{
	dma_sync_sg_for_cpu(dev, sgt->sgl, sgt->orig_nents, dir);
}

#define dma_sync_sgtable_for_device LINUX_BACKPORT(dma_sync_sgtable_for_device)
static inline void dma_sync_sgtable_for_device(struct device *dev,
		struct sg_table *sgt, enum dma_data_direction dir)
{
	dma_sync_sg_for_device(dev, sgt->sgl, sgt->orig_nents, dir);
}
#endif /* LINUX_VERSION_IS_LESS(5, 4, 233) */

#endif /* __BACKPORT_LINUX_DMA_MAPPING_H */
