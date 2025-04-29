#ifndef __BACKPORT_LINUX_EFI_H
#define __BACKPORT_LINUX_EFI_H
#include_next <linux/efi.h>

#if LINUX_VERSION_IS_LESS(5,7,0)
#define EFI_RT_SUPPORTED_GET_VARIABLE				0x0010

static inline bool efi_rt_services_supported(unsigned int mask)
{
	return false;
}
#endif

#endif /* __BACKPORT_LINUX_EFI_H */
