#include <linux/module.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/err.h>
#include <linux/delay.h>

#include "gpio.h"

#ifdef CONFIG_SCHED_HRTICK
	#define	lrdmwl_delay(d) usleep_range(d, d + 10);
#else
	#define	lrdmwl_delay(d) (d < (MAX_UDELAY_MS * 1000) ? udelay(d) : mdelay(d / 1000))
#endif   

static unsigned int reset_pwd_gpio = -1;

module_param(reset_pwd_gpio, uint, 0644);
MODULE_PARM_DESC(reset_pwd_gpio, "WIFI CHIP_PWD reset pin GPIO (deprecated)");

int mwl_gpio_init_legacy(void)
{
	int rc;

	/* Only return failure code if GPIO is configured but request fails */
	if (!gpio_is_valid(reset_pwd_gpio))
		return 0;

	rc = gpio_request_one(reset_pwd_gpio, GPIOF_OUT_INIT_HIGH, "WIFI_RESET");
	if (rc) {
		pr_err("lrdmwl: Unable to obtain WIFI power gpio. %d\n", rc);
		return rc;
	}

#if LINUX_VERSION_IS_GEQ(6,4,0)
	gpiod_export(gpio_to_desc(reset_pwd_gpio), false);
#elif LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,3)
	gpiod_export(gpio_to_desc(reset_pwd_gpio), false);
#else
	gpio_export(reset_pwd_gpio, false);
#endif

	return 0;
}

void mwl_gpio_release_legacy(void)
{
	if (!gpio_is_valid(reset_pwd_gpio))
		return;

	/* We release GPIO and leave the chip in reset when we unload */
	gpio_set_value(reset_pwd_gpio, 0);
	gpio_free(reset_pwd_gpio);
}

#if LINUX_VERSION_IS_GEQ(5,5,0)
int mwl_gpio_init(struct device *dev, struct device_node *np,
	mwl_gpio_t **card_gpio)
{
	mwl_gpio_t *desc;
	int i;

	static const char* gpios[] = {
		"pmu-en",
		"reset",
	};

	for (i = 0; i < ARRAY_SIZE(gpios) && np; i++) {
		desc = devm_fwnode_gpiod_get_index(dev, of_fwnode_handle(np),
			gpios[i], 0, GPIOD_OUT_HIGH, "wifi_pmu_en");

		if (!IS_ERR(desc)) {
			*card_gpio = desc;
			return 0;
		}

		if (PTR_ERR(desc) != -ENOENT)
			return PTR_ERR(desc);
	}

	if (gpio_is_valid(reset_pwd_gpio)) {
		desc = gpio_to_desc(reset_pwd_gpio);
		if (!IS_ERR(desc))
			*card_gpio = desc;
	}

	return 0;
}

bool mwl_gpio_valid(mwl_gpio_t *card_gpio)
{
	return !IS_ERR_OR_NULL(card_gpio);
}

int mwl_gpio_num(mwl_gpio_t *card_gpio)
{
	return mwl_gpio_valid(card_gpio) ? desc_to_gpio(card_gpio) : -1;
}

void mwl_gpio_recover(mwl_gpio_t *card_gpio)
{
	if (mwl_gpio_valid(card_gpio))
		gpiod_direction_output(card_gpio, 0);
}

int mwl_gpio_set(mwl_gpio_t *card_gpio, int value)
{
	if (!mwl_gpio_valid(card_gpio))
		return -ENODEV;

	gpiod_set_value(card_gpio, value);

	if (value)
		lrdmwl_delay(1500);

	return 0;
}
#else
#include <linux/of_gpio.h>

int mwl_gpio_init(struct device *dev, struct device_node *np,
	mwl_gpio_t **card_gpio)
{
	enum of_gpio_flags of_flags = 0;
	unsigned gpio = -1, active_low = 0;
	int rc;
	unsigned long flags;
	mwl_gpio_t *c_gpio;

	if (np) {
		gpio = of_get_named_gpio_flags(np, "pmu-en-gpios", 0, &of_flags);
		if (gpio_is_valid(gpio))
			goto done;

		gpio = of_get_named_gpio_flags(np, "reset-gpios", 0, &of_flags);
		if (gpio_is_valid(gpio))
			goto done;
	}	

	gpio = gpio_is_valid(reset_pwd_gpio) ? reset_pwd_gpio : -1;
	goto exit;

done:
	flags = (of_flags & OF_GPIO_ACTIVE_LOW) ?
		GPIOF_OUT_INIT_LOW : GPIOF_OUT_INIT_HIGH;

#if LINUX_VERSION_IS_LESS(3,17,0)
	active_low = !!(flags & OF_GPIO_ACTIVE_LOW);
#else
	if (of_flags & OF_GPIO_ACTIVE_LOW)
		flags |= GPIOF_ACTIVE_LOW;
#endif
	rc = devm_gpio_request_one(dev, gpio, flags, "wifi_pmu_en");
	if (rc)
		return rc;

exit:
	c_gpio = devm_kzalloc(dev, sizeof(*c_gpio), GFP_KERNEL);
	c_gpio->active_low = active_low;
	c_gpio->gpio = gpio;
	*card_gpio = c_gpio;
	return 0;
}

bool mwl_gpio_valid(mwl_gpio_t *card_gpio)
{
	return card_gpio && gpio_is_valid(card_gpio->gpio);
}

int mwl_gpio_num(mwl_gpio_t *card_gpio)
{
	return mwl_gpio_valid(card_gpio) ? card_gpio->gpio : -1;
}

void mwl_gpio_recover(mwl_gpio_t *card_gpio)
{
	if (mwl_gpio_valid(card_gpio))
		gpio_direction_output(card_gpio->gpio, card_gpio->active_low ? 1 : 0);
}

int mwl_gpio_set(mwl_gpio_t *card_gpio, int value)
{
	if (!mwl_gpio_valid(card_gpio))
		return -ENODEV;

	gpio_set_value(card_gpio->gpio,
		card_gpio->active_low ? !value : value);

	if (value)
		lrdmwl_delay(1500);

	return 0;
}
#endif
