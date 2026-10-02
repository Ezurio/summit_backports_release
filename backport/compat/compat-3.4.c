/*
 * Copyright 2012  Luis R. Rodriguez <mcgrof@frijolero.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Backport functionality introduced in Linux 3.4.
 */

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/compat.h>
#include <asm/uaccess.h>
#include <linux/tty.h>
#include <linux/rtnetlink.h>
#include <linux/netdevice.h>

#if LINUX_VERSION_IS_GEQ(3,2,0)
#include <linux/regmap.h>
#include <linux/i2c.h>
#include <linux/spi/spi.h>
#endif /* LINUX_VERSION_IS_GEQ(3,2,0) */

#include <linux/regulator/consumer.h>
#include <linux/err.h>

#if LINUX_VERSION_IS_GEQ(3,2,0)

#if defined(CONFIG_REGMAP)
static void devm_regmap_release(struct device *dev, void *res)
{
	regmap_exit(*(struct regmap **)res);
}

#if defined(CONFIG_REGMAP_I2C)
static int regmap_i2c_write(
			    struct device *dev,
			    const void *data,
			    size_t count)
{
	struct i2c_client *i2c = to_i2c_client(dev);
	int ret;

	ret = i2c_master_send(i2c, data, count);
	if (ret == count)
		return 0;
	else if (ret < 0)
		return ret;
	else
		return -EIO;
}

static int regmap_i2c_gather_write(
				   struct device *dev,
				   const void *reg, size_t reg_size,
				   const void *val, size_t val_size)
{
	struct i2c_client *i2c = to_i2c_client(dev);
	struct i2c_msg xfer[2];
	int ret;

	/* If the I2C controller can't do a gather tell the core, it
	 * will substitute in a linear write for us.
	 */
	if (!i2c_check_functionality(i2c->adapter, I2C_FUNC_NOSTART))
		return -ENOTSUPP;

	xfer[0].addr = i2c->addr;
	xfer[0].flags = 0;
	xfer[0].len = reg_size;
	xfer[0].buf = (void *)reg;

	xfer[1].addr = i2c->addr;
	xfer[1].flags = I2C_M_NOSTART;
	xfer[1].len = val_size;
	xfer[1].buf = (void *)val;

	ret = i2c_transfer(i2c->adapter, xfer, 2);
	if (ret == 2)
		return 0;
	if (ret < 0)
		return ret;
	else
		return -EIO;
}

static int regmap_i2c_read(
			   struct device *dev,
			   const void *reg, size_t reg_size,
			   void *val, size_t val_size)
{
	struct i2c_client *i2c = to_i2c_client(dev);
	struct i2c_msg xfer[2];
	int ret;

	xfer[0].addr = i2c->addr;
	xfer[0].flags = 0;
	xfer[0].len = reg_size;
	xfer[0].buf = (void *)reg;

	xfer[1].addr = i2c->addr;
	xfer[1].flags = I2C_M_RD;
	xfer[1].len = val_size;
	xfer[1].buf = val;

	ret = i2c_transfer(i2c->adapter, xfer, 2);
	if (ret == 2)
		return 0;
	else if (ret < 0)
		return ret;
	else
		return -EIO;
}

static struct regmap_bus regmap_i2c = {
	.write = regmap_i2c_write,
	.gather_write = regmap_i2c_gather_write,
	.read = regmap_i2c_read,
};
#endif /* defined(CONFIG_REGMAP_I2C) */

/**
 * devm_regmap_init(): Initialise managed register map
 *
 * @dev: Device that will be interacted with
 * @bus: Bus-specific callbacks to use with device
 * @bus_context: Data passed to bus-specific callbacks
 * @config: Configuration for register map
 *
 * The return value will be an ERR_PTR() on error or a valid pointer
 * to a struct regmap.  This function should generally not be called
 * directly, it should be called by bus-specific init functions.  The
 * map will be automatically freed by the device management code.
 */
struct regmap *devm_regmap_init(struct device *dev,
				const struct regmap_bus *bus,
				const struct regmap_config *config)
{
	struct regmap **ptr, *regmap;

	ptr = devres_alloc(devm_regmap_release, sizeof(*ptr), GFP_KERNEL);
	if (!ptr)
		return ERR_PTR(-ENOMEM);

	regmap = regmap_init(dev,
			     bus,
			     config);
	if (!IS_ERR(regmap)) {
		*ptr = regmap;
		devres_add(dev, ptr);
	} else {
		devres_free(ptr);
	}

	return regmap;
}
EXPORT_SYMBOL_GPL(devm_regmap_init);

#if defined(CONFIG_REGMAP_I2C)
/**
 * devm_regmap_init_i2c(): Initialise managed register map
 *
 * @i2c: Device that will be interacted with
 * @config: Configuration for register map
 *
 * The return value will be an ERR_PTR() on error or a valid pointer
 * to a struct regmap.  The regmap will be automatically freed by the
 * device management code.
 */
struct regmap *devm_regmap_init_i2c(struct i2c_client *i2c,
				    const struct regmap_config *config)
{
	return devm_regmap_init(&i2c->dev, &regmap_i2c, config);
}
EXPORT_SYMBOL_GPL(devm_regmap_init_i2c);
#endif /* defined(CONFIG_REGMAP_I2C) */

#endif /* defined(CONFIG_REGMAP) */
#endif /* LINUX_VERSION_IS_GEQ(3,2,0) */

int simple_open(struct inode *inode, struct file *file)
{
	if (inode->i_private)
		file->private_data = inode->i_private;
	return 0;
}
EXPORT_SYMBOL_GPL(simple_open);

#ifdef CONFIG_COMPAT
static int __compat_put_timespec(const struct timespec *ts, struct compat_timespec __user *cts)
{
	return (!access_ok(VERIFY_WRITE, cts, sizeof(*cts)) ||
			__put_user(ts->tv_sec, &cts->tv_sec) ||
			__put_user(ts->tv_nsec, &cts->tv_nsec)) ? -EFAULT : 0;
}

int compat_put_timespec(const struct timespec *ts, void __user *uts)
{
	if (COMPAT_USE_64BIT_TIME)
		return copy_to_user(uts, ts, sizeof *ts) ? -EFAULT : 0;
	else
		return __compat_put_timespec(ts, uts);
}
EXPORT_SYMBOL_GPL(compat_put_timespec);
#endif

int tty_standard_install(struct tty_driver *driver, struct tty_struct *tty)
{
	int ret = tty_init_termios(tty);
	if (ret)
		return ret;

	tty_driver_kref_get(driver);
	tty->count++;
	driver->ttys[tty->index] = tty;
	return 0;
}
EXPORT_SYMBOL_GPL(tty_standard_install);

/* netdev_stats_to_stats64 was defined in kernel 3.3.x, but not exported. */
/* Convert net_device_stats to rtnl_link_stats64.  They have the same
 * fields in the same order, with only the type differing.
 */
void netdev_stats_to_stats64(struct rtnl_link_stats64 *stats64,
			     const struct net_device_stats *netdev_stats)
{
#if BITS_PER_LONG == 64
	BUILD_BUG_ON(sizeof(*stats64) != sizeof(*netdev_stats));
	memcpy(stats64, netdev_stats, sizeof(*stats64));
#else
	size_t i, n = sizeof(*stats64) / sizeof(u64);
	const unsigned long *src = (const unsigned long *)netdev_stats;
	u64 *dst = (u64 *)stats64;

	BUILD_BUG_ON(sizeof(*netdev_stats) / sizeof(unsigned long) !=
		     sizeof(*stats64) / sizeof(u64));
	for (i = 0; i < n; i++)
		dst[i] = src[i];
#endif
}
EXPORT_SYMBOL(netdev_stats_to_stats64);

struct regulator_bulk_devres {
	struct regulator_bulk_data *consumers;
	int num_consumers;
};

int devm_regulator_bulk_get(struct device *dev, int num_consumers,
			    struct regulator_bulk_data *consumers)
{
	int i;
	int ret;

	for (i = 0; i < num_consumers; i++)
		consumers[i].consumer = NULL;

	for (i = 0; i < num_consumers; i++) {
		consumers[i].consumer = devm_regulator_get(dev,
							   consumers[i].supply);
		if (IS_ERR(consumers[i].consumer)) {
			ret = PTR_ERR(consumers[i].consumer);
			dev_err(dev, "Failed to get supply '%s': %d\n",
				consumers[i].supply, ret);
			consumers[i].consumer = NULL;
			goto err;
		}
	}

	return 0;

err:
	for (i = 0; i < num_consumers && consumers[i].consumer; i++)
		devm_regulator_put(consumers[i].consumer);

	return ret;
}
EXPORT_SYMBOL_GPL(devm_regulator_bulk_get);

static void devm_regulator_release(struct device *dev, void *res)
{
	regulator_put(*(struct regulator **)res);
}

struct regulator *devm_regulator_get(struct device *dev, const char *id)
{
	struct regulator **ptr, *regulator;

	ptr = devres_alloc(devm_regulator_release, sizeof(*ptr), GFP_KERNEL);
	if (!ptr)
		return ERR_PTR(-ENOMEM);

	regulator = regulator_get(dev, id);
	if (!IS_ERR(regulator)) {
		*ptr = regulator;
		devres_add(dev, ptr);
	} else {
		devres_free(ptr);
	}

	return regulator;
}
EXPORT_SYMBOL_GPL(devm_regulator_get);

static int devm_regulator_match(struct device *dev, void *res, void *data)
{
	struct regulator **r = res;
	if (!r || !*r) {
		WARN_ON(!r || !*r);
		return 0;
	}
	return *r == data;
}

/* struct here is actually a regulator struct, but the regulator struct has changed with kernel versions
   and we only care about the first field, the rest is opaque, so defined backport struct to 'expose' this first field.
   It is highly unlikely 'dev' will move in the actual regulator struct.
*/
struct backport_regulator {
	struct device *dev;
};

void devm_regulator_put(struct regulator *regulator)
{
	int rc;
	struct backport_regulator *bk_regulator = (struct backport_regulator*) regulator;

	rc = devres_destroy(bk_regulator->dev, devm_regulator_release,
			    devm_regulator_match, regulator);
	if (rc == 0)
		regulator_put(regulator);
	else
		WARN_ON(rc);
}
EXPORT_SYMBOL_GPL(devm_regulator_put);