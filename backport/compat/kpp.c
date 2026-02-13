#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/crypto.h>
#include <crypto/algapi.h>
#if LINUX_VERSION_IS_GEQ(3,2,0)
#include <linux/cryptouser.h>
#endif
#include <linux/compiler.h>
#include <net/netlink.h>
#include <crypto/kpp.h>
#include <crypto/internal/kpp.h>

#if LINUX_VERSION_IS_LESS(4,8,0)

extern void *crypto_alloc_tfm(const char *alg_name,
		       const struct crypto_type *frontend, u32 type, u32 mask);

//0x00000008 is reserved for CRYPTO_ALG_TYPE_HASH for old kernels, so let's use 0x0000000f
#define CRYPTO_ALG_TYPE_KPP		0x0000000f

#if LINUX_VERSION_IS_LESS(4,2,0)
#define CRYPTOCFGA_REPORT_AKCIPHER (CRYPTOCFGA_REPORT_CIPHER + 1)
#endif
#define CRYPTOCFGA_REPORT_KPP (CRYPTOCFGA_REPORT_AKCIPHER + 1)

#if LINUX_VERSION_IS_GEQ(3,2,0)
struct bp_crypto_report_kpp {
	char type[CRYPTO_MAX_NAME];
};


#ifdef CONFIG_NET
static int crypto_kpp_report(struct sk_buff *skb, struct crypto_alg *alg)
{
	struct bp_crypto_report_kpp rkpp;

	strncpy(rkpp.type, "kpp", sizeof(rkpp.type));

	if (nla_put(skb, CRYPTOCFGA_REPORT_KPP,
		    sizeof(struct bp_crypto_report_kpp), &rkpp))
		goto nla_put_failure;
	return 0;

nla_put_failure:
	return -EMSGSIZE;
}
#else
static int crypto_kpp_report(struct sk_buff *skb, struct crypto_alg *alg)
{
	return -ENOSYS;
}
#endif
#endif

static unsigned int crypto_alg_extsize(struct crypto_alg *alg)
{
	return alg->cra_ctxsize +
	       (alg->cra_alignmask & ~(crypto_tfm_ctx_alignment() - 1));
}

static inline unsigned int crypto_cipher_ctxsize(struct crypto_alg *alg)
{
	return alg->cra_ctxsize;
}

static inline unsigned int crypto_compress_ctxsize(struct crypto_alg *alg)
{
	return alg->cra_ctxsize;
}

static void crypto_kpp_show(struct seq_file *m, struct crypto_alg *alg)
	__maybe_unused;

static void crypto_kpp_show(struct seq_file *m, struct crypto_alg *alg)
{
	seq_puts(m, "type         : kpp\n");
}

static void crypto_kpp_exit_tfm(struct crypto_tfm *tfm)
{
	struct crypto_kpp *kpp = __crypto_kpp_tfm(tfm);
	struct kpp_alg *alg = crypto_kpp_alg(kpp);

	alg->exit(kpp);
}

static int crypto_kpp_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_kpp *kpp = __crypto_kpp_tfm(tfm);
	struct kpp_alg *alg = crypto_kpp_alg(kpp);

	if (alg->exit)
		kpp->base.exit = crypto_kpp_exit_tfm;

	if (alg->init)
		return alg->init(kpp);

	return 0;
}

static const struct crypto_type crypto_kpp_type = {
	.extsize = crypto_alg_extsize,
	.init_tfm = crypto_kpp_init_tfm,
#ifdef CONFIG_PROC_FS
	.show = crypto_kpp_show,
#endif
#if LINUX_VERSION_IS_GEQ(3,2,0)
	.report = crypto_kpp_report,
#endif
	.maskclear = ~CRYPTO_ALG_TYPE_MASK,
	.maskset = CRYPTO_ALG_TYPE_MASK,
	.type = CRYPTO_ALG_TYPE_KPP,
	.tfmsize = offsetof(struct crypto_kpp, base),
};

struct crypto_kpp *crypto_alloc_kpp(const char *alg_name, u32 type, u32 mask)
{
	return crypto_alloc_tfm(alg_name, &crypto_kpp_type, type, mask);
}
EXPORT_SYMBOL_GPL(crypto_alloc_kpp);

static void kpp_prepare_alg(struct kpp_alg *alg)
{
	struct crypto_alg *base = &alg->base;

	base->cra_type = &crypto_kpp_type;
	base->cra_flags &= ~CRYPTO_ALG_TYPE_MASK;
	base->cra_flags |= CRYPTO_ALG_TYPE_KPP;
}

int crypto_register_kpp(struct kpp_alg *alg)
{
	struct crypto_alg *base = &alg->base;

	kpp_prepare_alg(alg);
	return crypto_register_alg(base);
}

void crypto_unregister_kpp(struct kpp_alg *alg)
{
	crypto_unregister_alg(&alg->base);
}

#endif
