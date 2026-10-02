/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2024 Linaro Ltd.
 */

#ifndef __BACKPORT_LINUX_PWRSEQ_CONSUMER_H
#define __BACKPORT_LINUX_PWRSEQ_CONSUMER_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,11,0)
#include_next <linux/pwrseq/consumer.h>
#else
#include <linux/err.h>

struct device;
struct pwrseq_desc;

static inline struct pwrseq_desc * __must_check
pwrseq_get(struct device *dev, const char *target)
{
	return ERR_PTR(-ENOSYS);
}

static inline void pwrseq_put(struct pwrseq_desc *desc)
{
}

static inline struct pwrseq_desc * __must_check
devm_pwrseq_get(struct device *dev, const char *target)
{
	return ERR_PTR(-ENOSYS);
}

static inline int pwrseq_power_on(struct pwrseq_desc *desc)
{
	return -ENOSYS;
}

static inline int pwrseq_power_off(struct pwrseq_desc *desc)
{
	return -ENOSYS;
}
#endif

#endif /* __BACKPORT_LINUX_PWRSEQ_CONSUMER_H */
