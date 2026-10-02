/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * This file is part of cc33xx
 *
 * Copyright (C) 2009 Nokia Corporation
 *
 * Contact: Luciano Coelho <luciano.coelho@nokia.com>
 */

#ifndef __INIT_H__
#define __INIT_H__

#include "cc33xx.h"


int cc33xx_hw_init(struct cc33xx *cc);
int cc33xx_download_ini_params_and_wait(struct cc33xx *cc);
int cc33xx_init_vif_specific(struct cc33xx *cc, struct ieee80211_vif *vif);


#endif /* __INIT_H__ */
