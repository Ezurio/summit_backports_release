#ifndef _BP_LINUX_NVMEM_CONSUMER_H
#define _BP_LINUX_NVMEM_CONSUMER_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(4,3,0) 
#include_next <linux/nvmem-consumer.h>
#else /* < 4.3.0 */
struct device;
struct nvmem_cell;

static inline struct nvmem_cell *devm_nvmem_cell_get(struct device *dev,
						     const char *id)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline void *nvmem_cell_read(struct nvmem_cell *cell, size_t *len)
{
	return ERR_PTR(-EOPNOTSUPP);
}
#endif /* < 4.3.0 */

#endif  /* _BP_LINUX_NVMEM_CONSUMER_H */
