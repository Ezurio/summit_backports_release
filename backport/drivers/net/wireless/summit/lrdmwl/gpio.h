#ifndef _MWL_GPIO_H_
#define _MWL_GPIO_H_

#include <linux/device.h>
#include <linux/of.h>

#if LINUX_VERSION_IS_GEQ(5,5,0)
typedef struct gpio_desc mwl_gpio_t;
#else
typedef struct {
	int gpio;
	int active_low;
} mwl_gpio_t;
#endif

int mwl_gpio_init_legacy(void);
void mwl_gpio_release_legacy(void);

int mwl_gpio_init(struct device *dev, struct device_node *np, mwl_gpio_t **card_gpio);
bool mwl_gpio_valid(mwl_gpio_t *card_gpio);
int mwl_gpio_num(mwl_gpio_t *card_gpio);
void mwl_gpio_recover(mwl_gpio_t *card_gpio);
int mwl_gpio_set(mwl_gpio_t *card_gpio, int value);

#endif /* _MWL_GPIO_H_ */
