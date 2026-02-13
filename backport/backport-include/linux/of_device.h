#ifndef __BP_OF_DEVICE_H
#define __BP_OF_DEVICE_H
#include_next <linux/of_device.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(4,18,0)
static inline int backport_of_dma_configure(struct device *dev,
					    struct device_node *np,
					    bool force_dma)
{
#if LINUX_VERSION_IS_GEQ(4,15,0)
	dev->bus->force_dma = force_dma;
	return of_dma_configure(dev, np);
#elif LINUX_VERSION_IS_GEQ(4,12,0)
	return of_dma_configure(dev, np);
#elif LINUX_VERSION_IS_GEQ(4,1,0)
	of_dma_configure(dev, np);
	return 0;
#else
	return 0;
#endif
}
#define of_dma_configure LINUX_BACKPORT(of_dma_configure)
#endif /* < 4.18 */

#ifndef CONFIG_OF
#if LINUX_VERSION_IS_LESS(2,6,39)
static inline const struct of_device_id *of_match_device(
		const struct of_device_id *matches, const struct device *dev)
{
	return NULL;
}
#endif
#endif

#endif /* __BP_OF_DEVICE_H */
