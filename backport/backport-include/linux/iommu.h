#ifndef _BP_LINUX_IOMMU_H
#define _BP_LINUX_IOMMU_H
#include <linux/version.h>
#include_next <linux/iommu.h>

#if LINUX_VERSION_IS_LESS(6,3,0)
static inline 
int backport_iommu_map(struct iommu_domain *domain, unsigned long iova,
		     phys_addr_t paddr, size_t size, int prot, gfp_t gfp)
{
    return iommu_map(domain, iova, paddr, size, prot);
}
#define iommu_map LINUX_BACKPORT(iommu_map)
#endif /* < 6.3.0 */

#endif /* _BP_LINUX_IOMMU_H */
