#ifndef _BACKPORT_CRYPTO_HASH_H
#define _BACKPORT_CRYPTO_HASH_H
#include_next <crypto/hash.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(4,6,0)
#define shash_desc_zero LINUX_BACKPORT(shash_desc_zero)
static inline void shash_desc_zero(struct shash_desc *desc)
{
	memzero_explicit(desc,
			 sizeof(*desc) + crypto_shash_descsize(desc->tfm));
}
#endif

#if LINUX_VERSION_IS_LESS(4,6,0)
#define ahash_request_zero LINUX_BACKPORT(ahash_request_zero)
static inline void ahash_request_zero(struct ahash_request *req)
{
	memzero_explicit(req, sizeof(*req) +
			      crypto_ahash_reqsize(crypto_ahash_reqtfm(req)));
}
#endif

#ifndef SHASH_DESC_ON_STACK
#define SHASH_DESC_ON_STACK(shash, ctx)				  \
	char __##shash##_desc[sizeof(struct shash_desc) +	  \
		crypto_shash_descsize(ctx)] CRYPTO_MINALIGN_ATTR; \
	struct shash_desc *shash = (struct shash_desc *)__##shash##_desc
#endif

#if LINUX_VERSION_IS_LESS(5,8,0)
int crypto_shash_tfm_digest(struct crypto_shash *tfm, const u8 *data,
			    unsigned int len, u8 *out);
#endif

#endif /* _BACKPORT_CRYPTO_HASH_H */
