#ifndef __BACKPORT_LINUX_REGULATOR_CONSUMER_H
#define __BACKPORT_LINUX_REGULATOR_CONSUMER_H
#include_next <linux/regulator/consumer.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(3,4,0)
#define devm_regulator_bulk_get LINUX_BACKPORT(devm_regulator_bulk_get)
int devm_regulator_bulk_get(struct device *dev, int num_consumers, struct regulator_bulk_data *consumers);

#define devm_regulator_get LINUX_BACKPORT(devm_regulator_get)
struct regulator *devm_regulator_get(struct device *dev, const char *id);

#define devm_regulator_put LINUX_BACKPORT(devm_regulator_put)
void devm_regulator_put(struct regulator *regulator);
#endif

#if LINUX_VERSION_IS_LESS(6,1,0)
#define devm_regulator_get_enable LINUX_BACKPORT(devm_regulator_get_enable)
int devm_regulator_get_enable(struct device *dev, const char *id);
#endif

#endif /* __BACKPORT_LINUX_REGULATOR_CONSUMER_H */
