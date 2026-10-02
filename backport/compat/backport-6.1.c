// SPDX-License-Identifier: GPL-2.0
#include <linux/device.h>
#include <linux/regulator/consumer.h>
#include <linux/err.h>

static int match_any(struct device *dev, void *unused)
{
	return 1;
}

struct device *device_find_any_child(struct device *parent)
{
	return device_find_child(parent, NULL, match_any);
}
EXPORT_SYMBOL_GPL(device_find_any_child);

static void regulator_action_disable(void *d)
{
	struct regulator *r = (struct regulator *)d;

	regulator_disable(r);
}

int devm_regulator_get_enable(struct device *dev, const char *id)
{
	struct regulator *r;
	int ret;

	r = devm_regulator_get(dev, id);
	if (IS_ERR(r))
		return PTR_ERR(r);

	ret = regulator_enable(r);
	if (!ret)
		ret = devm_add_action_or_reset(dev, &regulator_action_disable, r);

	if (ret)
		devm_regulator_put(r);

	return ret;
}
EXPORT_SYMBOL_GPL(devm_regulator_get_enable);
