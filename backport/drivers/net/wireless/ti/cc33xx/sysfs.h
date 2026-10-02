/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * This file is part of cc33xx
 *
 * Copyright (C) 2013 Texas Instruments Inc.
 */

#ifndef __SYSFS_H__
#define __SYSFS_H__


int cc33xx_sysfs_init(struct cc33xx *cc);
void cc33xx_sysfs_free(struct cc33xx *cc);
#endif /* __SYSFS_H__ */
