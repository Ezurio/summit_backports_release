/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BACKPORT_LINUX_CRYPTO_H
#define _BACKPORT_LINUX_CRYPTO_H
#include <linux/version.h>

#include_next <linux/crypto.h>

#ifndef DECLARE_CRYPTO_WAIT
#include <linux/completion.h>

struct crypto_wait {
	struct completion completion;
	int err;
};

#define DECLARE_CRYPTO_WAIT(_wait) \
	struct crypto_wait _wait = { \
		COMPLETION_INITIALIZER_ONSTACK((_wait).completion), 0 }

void crypto_req_done(struct crypto_async_request *req, int err);

static inline int crypto_wait_req(int err, struct crypto_wait *wait)
{
	switch (err) {
	case -EINPROGRESS:
	case -EBUSY:
		wait_for_completion(&wait->completion);
		reinit_completion(&wait->completion);
		err = wait->err;
		break;
	};

	return err;
}

static inline void crypto_init_wait(struct crypto_wait *wait)
{
	init_completion(&wait->completion);
}

#endif

#if LINUX_VERSION_IS_LESS(5,0,0)
static inline void crypto_stats_init(struct crypto_alg *alg)
{}
static inline void crypto_stats_get(struct crypto_alg *alg)
{}
static inline void crypto_stats_ablkcipher_encrypt(unsigned int nbytes, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_ablkcipher_decrypt(unsigned int nbytes, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_aead_encrypt(unsigned int cryptlen, struct crypto_alg *alg, int ret)
{}
static inline void crypto_stats_aead_decrypt(unsigned int cryptlen, struct crypto_alg *alg, int ret)
{}
static inline void crypto_stats_ahash_update(unsigned int nbytes, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_ahash_final(unsigned int nbytes, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_akcipher_encrypt(unsigned int src_len, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_akcipher_decrypt(unsigned int src_len, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_akcipher_sign(int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_akcipher_verify(int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_compress(unsigned int slen, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_decompress(unsigned int slen, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_kpp_set_secret(struct crypto_alg *alg, int ret)
{}
static inline void crypto_stats_kpp_generate_public_key(struct crypto_alg *alg, int ret)
{}
static inline void crypto_stats_kpp_compute_shared_secret(struct crypto_alg *alg, int ret)
{}
static inline void crypto_stats_rng_seed(struct crypto_alg *alg, int ret)
{}
static inline void crypto_stats_rng_generate(struct crypto_alg *alg, unsigned int dlen, int ret)
{}
static inline void crypto_stats_skcipher_encrypt(unsigned int cryptlen, int ret, struct crypto_alg *alg)
{}
static inline void crypto_stats_skcipher_decrypt(unsigned int cryptlen, int ret, struct crypto_alg *alg)
{}
#endif /* LINUX_VERSION_IS_LESS(5,0,0) */

#endif /* _BACKPORT_LINUX_CRYPTO_H */

