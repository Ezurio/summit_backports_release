#ifndef _BACKPORT_LINUX_DEVICE_FAUX_H
#define _BACKPORT_LINUX_DEVICE_FAUX_H
#include <linux/platform_device.h>

#define faux_device_create(a, b, c) platform_device_register_simple(a, 0, NULL, 0)
#define faux_device_destroy(a) platform_device_unregister(a)
#define faux_device platform_device

#endif /* _BACKPORT_LINUX_DEVICE_FAUX_H */
