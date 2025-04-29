/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORTS_LINUX_GPIO_CONSUMER_H
#define __BACKPORTS_LINUX_GPIO_CONSUMER_H

#if LINUX_VERSION_IS_GEQ(3,13,0)

#include_next <linux/gpio/consumer.h>

#if LINUX_VERSION_IS_LESS(3,17,0)

#define GPIOD_FLAGS_BIT_DIR_SET		BIT(0)
#define GPIOD_FLAGS_BIT_DIR_OUT		BIT(1)
#define GPIOD_FLAGS_BIT_DIR_VAL		BIT(2)
#define GPIOD_FLAGS_BIT_OPEN_DRAIN	BIT(3)

/**
 * Optional flags that can be passed to one of gpiod_* to configure direction
 * and output value. These values cannot be OR'd.
 */
enum gpiod_flags {
	GPIOD_ASIS	= 0,
	GPIOD_IN	= GPIOD_FLAGS_BIT_DIR_SET,
	GPIOD_OUT_LOW	= GPIOD_FLAGS_BIT_DIR_SET | GPIOD_FLAGS_BIT_DIR_OUT,
	GPIOD_OUT_HIGH	= GPIOD_FLAGS_BIT_DIR_SET | GPIOD_FLAGS_BIT_DIR_OUT |
			  GPIOD_FLAGS_BIT_DIR_VAL,
	GPIOD_OUT_LOW_OPEN_DRAIN = GPIOD_OUT_LOW | GPIOD_FLAGS_BIT_OPEN_DRAIN,
	GPIOD_OUT_HIGH_OPEN_DRAIN = GPIOD_OUT_HIGH | GPIOD_FLAGS_BIT_OPEN_DRAIN,
};

#define devm_gpiod_get_optional LINUX_BACKPORT(devm_gpiod_get_optional)
#define gpiod_get_optional LINUX_BACKPORT(gpiod_get_optional)

#ifdef CONFIG_GPIOLIB

static inline struct gpio_desc *__must_check
devm_gpiod_get_optional(struct device *dev,
						const char *con_id,
						enum gpiod_flags flags)
{
	struct gpio_desc *desc;
	int rc;

	desc = devm_gpiod_get(dev, con_id);
	if (IS_ERR(desc)) {
		if (PTR_ERR(desc) == -ENOENT)
			desc = NULL;
		return desc;
	}

	if (flags & GPIOD_FLAGS_BIT_DIR_OUT)
		rc = gpiod_direction_output(desc,
				!!(flags & GPIOD_FLAGS_BIT_DIR_VAL));
	else
		rc = gpiod_direction_input(desc);
	if (rc) {
		devm_gpiod_put(dev, desc);
		return ERR_PTR(rc);
	}

	return desc;
}

static inline struct gpio_desc *__must_check
gpiod_get_optional(struct device *dev,
						const char *con_id,
						enum gpiod_flags flags)
{
	struct gpio_desc *desc;
	int rc;

	desc = gpiod_get(dev, con_id);
	if (IS_ERR(desc)) {
		if (PTR_ERR(desc) == -ENOENT)
			desc = NULL;
		return desc;
	}

	if (flags & GPIOD_FLAGS_BIT_DIR_OUT)
		rc = gpiod_direction_output(desc,
				!!(flags & GPIOD_FLAGS_BIT_DIR_VAL));
	else
		rc = gpiod_direction_input(desc);
	if (rc) {
		gpiod_put(desc);
		return ERR_PTR(rc);
	}

	return desc;
}

#else /* CONFIG_GPIOLIB */

static inline struct gpio_desc *__must_check
devm_gpiod_get_optional(struct device *dev, const char *con_id,
			  enum gpiod_flags flags)
{
	return NULL;
}

static inline struct gpio_desc *__must_check
gpiod_get_optional(struct device *dev, const char *con_id,
			  enum gpiod_flags flags)
{
	return NULL;
}

#endif /* CONFIG_GPIOLIB */
#endif /* LINUX_VERSION_IS_LESS(3,17,0) */

#else  /* LINUX_VERSION_IS_LESS(3,13,0) */

#define GPIOD_FLAGS_BIT_DIR_SET		BIT(0)
#define GPIOD_FLAGS_BIT_DIR_OUT		BIT(1)
#define GPIOD_FLAGS_BIT_DIR_VAL		BIT(2)
#define GPIOD_FLAGS_BIT_OPEN_DRAIN	BIT(3)

/**
 * Optional flags that can be passed to one of gpiod_* to configure direction
 * and output value. These values cannot be OR'd.
 */
enum gpiod_flags {
	GPIOD_ASIS	= 0,
	GPIOD_IN	= GPIOD_FLAGS_BIT_DIR_SET,
	GPIOD_OUT_LOW	= GPIOD_FLAGS_BIT_DIR_SET | GPIOD_FLAGS_BIT_DIR_OUT,
	GPIOD_OUT_HIGH	= GPIOD_FLAGS_BIT_DIR_SET | GPIOD_FLAGS_BIT_DIR_OUT |
			  GPIOD_FLAGS_BIT_DIR_VAL,
	GPIOD_OUT_LOW_OPEN_DRAIN = GPIOD_OUT_LOW | GPIOD_FLAGS_BIT_OPEN_DRAIN,
	GPIOD_OUT_HIGH_OPEN_DRAIN = GPIOD_OUT_HIGH | GPIOD_FLAGS_BIT_OPEN_DRAIN,
};

static inline struct gpio_desc *__must_check
devm_gpiod_get_optional(struct device *dev, const char *con_id,
			  enum gpiod_flags flags)
{
	return NULL;
}

static inline struct gpio_desc *__must_check
gpiod_get_optional(struct device *dev, const char *con_id,
			  enum gpiod_flags flags)
{
	return NULL;
}

static inline void gpiod_set_value_cansleep(struct gpio_desc *desc, int value)
{}

static inline int gpiod_to_irq(const struct gpio_desc *desc)
{
	return -EINVAL;
}

static inline void gpiod_put(struct gpio_desc *desc)
{}

#endif  /* LINUX_VERSION_IS_LESS(3,13,0) */

#endif /* __BACKPORTS_LINUX_GPIO_CONSUMER_H */
