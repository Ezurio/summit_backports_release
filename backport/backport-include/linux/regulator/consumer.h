#ifndef __BACKPORT_LINUX_REGULATOR_CONSUMER_H
#define __BACKPORT_LINUX_REGULATOR_CONSUMER_H
#include_next <linux/regulator/consumer.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(3,4,0)
static inline int devm_regulator_bulk_get(struct device *dev, int num_consumers,
					  struct regulator_bulk_data *consumers)
{
	return 0;
}
#endif

#endif /* __BACKPORT_LINUX_REGULATOR_CONSUMER_H */
