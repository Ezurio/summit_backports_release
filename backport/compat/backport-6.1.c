// SPDX-License-Identifier: GPL-2.0
#include <linux/device.h>

static int match_any(struct device *dev, void *unused)
{
	return 1;
}

struct device *device_find_any_child(struct device *parent)
{
	return device_find_child(parent, NULL, match_any);
}
EXPORT_SYMBOL_GPL(device_find_any_child);
