#ifndef _BACKPORTS_ACPI_BUS_H__
#define _BACKPORTS_ACPI_BUS_H__

#include_next <acpi/acpi_bus.h>
#include <linux/version.h>
#include <linux/uuid.h>

#if LINUX_VERSION_IS_LESS(3,14,0)
#define acpi_evaluate_dsm LINUX_BACKPORT(acpi_evaluate_dsm)
static inline
union acpi_object *acpi_evaluate_dsm(acpi_handle handle, const guid_t *guid,
	u64 rev, u64 func, union acpi_object *argv4)
{
	return NULL;
}

static inline
bool acpi_check_dsm(acpi_handle handle, const guid_t *guid,
	u64 rev, u64 funcs)
{
	return false;
}
#elif LINUX_VERSION_IS_LESS(4,13,0) 
static inline union acpi_object *
backport_acpi_evaluate_dsm(acpi_handle handle, const guid_t *guid, 
	u64 rev, u64 func, union acpi_object *argv4)
{
	return acpi_evaluate_dsm(handle, guid->b, rev, func, argv4);
}
#define acpi_evaluate_dsm LINUX_BACKPORT(acpi_evaluate_dsm)

static inline
bool backport_acpi_check_dsm(acpi_handle handle, const guid_t *guid,
	u64 rev, u64 funcs)
{
	return acpi_check_dsm(handle, guid->b, rev, funcs);
}
#define acpi_check_dsm LINUX_BACKPORT(acpi_check_dsm)
#endif /* < 4.13.0 */

#endif /* _BACKPORTS_ACPI_BUS_H__ */
