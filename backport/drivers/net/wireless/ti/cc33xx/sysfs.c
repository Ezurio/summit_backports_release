// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of cc33xx
 *
 * Copyright (C) 2013 Texas Instruments Inc.
 */

#include <linux/device.h>
#include "acx.h"
#include "sysfs.h"

static ssize_t ble_enable_show(struct device *dev,
				  struct device_attribute *attr,
				  char *buf)
{
	struct cc33xx *cc = dev_get_drvdata(dev);
	ssize_t len;

	mutex_lock(&cc->mutex);
	len = sysfs_emit(buf, "%d\n", cc->ble_enable);
	mutex_unlock(&cc->mutex);

	return len;
}

static ssize_t ble_enable_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct cc33xx *cc = dev_get_drvdata(dev);
	int ret;
	unsigned long value;

	ret = kstrtoul(buf, 10, &value);
	if (value != 1) {
		cc33xx_warning("illegal value in ble_enable (only value allowed is 1)");
		return -EINVAL;
	}

	if (value == cc->ble_enable) {
		return count;
	}

	mutex_lock(&cc->mutex);

	if (unlikely(cc->state != WLCORE_STATE_ON)) {
		/* this will show up on "read" in case we are off */
		cc->ble_enable = value;
		goto out;
	}

	cc33xx_ble_enable(cc, value);
out:
	mutex_unlock(&cc->mutex);
	return count;
}
static DEVICE_ATTR_RW(ble_enable);

static ssize_t cc33xx_sysfs_read_fwlog(struct file *filp, struct kobject *kobj,
				       struct bin_attribute *bin_attr,
				       char *buffer, loff_t pos, size_t count)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct cc33xx *wl = dev_get_drvdata(dev);
	ssize_t len;
	int ret;

	ret = mutex_lock_interruptible(&wl->mutex);
	if (ret < 0)
		return -ERESTARTSYS;

	/* Check if the fwlog is still valid */
	if (wl->fwlog_size < 0) {
		mutex_unlock(&wl->mutex);
		return 0;
	}

	/* Seeking is not supported - old logs are not kept. Disregard pos. */
	len = min_t(size_t, count, wl->fwlog_size);
	wl->fwlog_size -= len;
	memcpy(buffer, wl->fwlog, len);

	/* Make room for new messages */
	memmove(wl->fwlog, wl->fwlog + len, wl->fwlog_size);

	mutex_unlock(&wl->mutex);

	return len;
}

static const struct bin_attribute fwlog_attr = {
	.attr = { .name = "fwlog", .mode = 0400 },
	.read = cc33xx_sysfs_read_fwlog,
};

int wlcore_sysfs_init(struct cc33xx *wl)
{
	int ret;

	ret = device_create_file(wl->dev, &dev_attr_ble_enable);
	if (ret < 0) {
		cc33xx_error("failed to create sysfs file ble_enable");
	}

	/* Create sysfs file for the FW log */
	ret = device_create_bin_file(wl->dev, &fwlog_attr);
	if (ret < 0) {
		cc33xx_error("failed to create sysfs file fwlog");
	}

	return ret;
}

void wlcore_sysfs_free(struct cc33xx *wl)
{
	device_remove_bin_file(wl->dev, &fwlog_attr);
	device_remove_file(wl->dev, &dev_attr_ble_enable);
}
