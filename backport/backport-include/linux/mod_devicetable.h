#ifndef __BACKPORT_MOD_DEVICETABLE_H
#define __BACKPORT_MOD_DEVICETABLE_H
#include_next <linux/mod_devicetable.h>

#ifndef HID_BUS_ANY
#define HID_BUS_ANY                            0xffff
#endif

#ifndef HID_GROUP_ANY
#define HID_GROUP_ANY                          0x0000
#endif

#ifndef HID_ANY_ID
#define HID_ANY_ID                             (~0)
#endif

#if LINUX_VERSION_IS_LESS(5,7,0)
#ifndef MHI_NAME_SIZE
#define MHI_DEVICE_MODALIAS_FMT "mhi:%s"
#define MHI_NAME_SIZE 32

/**
 * struct mhi_device_id - MHI device identification
 * @chan: MHI channel name
 * @driver_data: driver data;
 */
struct mhi_device_id {
	const char chan[MHI_NAME_SIZE];
	kernel_ulong_t driver_data;
};
#endif
#endif

#ifndef DMI_EXACT_MATCH
#define DMI_EXACT_MATCH DMI_MATCH
#endif

#if LINUX_VERSION_IS_LESS(3,0,0)
/* Broadcom's specific AMBA core, see drivers/bcma/ */
struct bcma_device_id {
	__u16	manuf;
	__u16	id;
	__u8	rev;
	__u8	class;
};
#endif

#if LINUX_VERSION_IS_LESS(4,18,0)
#define DMI_PRODUCT_SKU DMI_BOARD_VENDOR
#endif

#endif /* __BACKPORT_MOD_DEVICETABLE_H */
