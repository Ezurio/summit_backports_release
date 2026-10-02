/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * ECDH params to be used with kpp API
 *
 * Copyright (c) 2016, Intel Corporation
 * Authors: Salvatore Benedetto <salvatore.benedetto@intel.com>
 */
#ifndef __BP_ECDH_H
#define __BP_ECDH_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(4,8,0)
#include_next <crypto/ecdh.h>
#else

/* Curves IDs */
#define ECC_CURVE_NIST_P192	0x0001
#define ECC_CURVE_NIST_P256	0x0002

struct ecdh {
	unsigned short curve_id;
	char *key;
	unsigned short key_size;
};

unsigned int crypto_ecdh_key_len(const struct ecdh *params);
int crypto_ecdh_encode_key(char *buf, unsigned int len, const struct ecdh *p);
int crypto_ecdh_decode_key(const char *buf, unsigned int len, struct ecdh *p);

#endif /* >= 4.8 */

#endif /* __BP_ECDH_H */