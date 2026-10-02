// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/kernel.h>
#include <crypto/hash.h>

int crypto_shash_tfm_digest(struct crypto_shash *tfm, const u8 *data,
			    unsigned int len, u8 *out)
{
	SHASH_DESC_ON_STACK(desc, tfm);
	int err;

	desc->tfm = tfm;

	err = crypto_shash_digest(desc, data, len, out);

	shash_desc_zero(desc);

	return err;
}
EXPORT_SYMBOL_GPL(crypto_shash_tfm_digest);
