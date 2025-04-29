/*
 * Copyright (c) 2015  Hauke Mehrtens <hauke@hauke-m.de>
 *
 * Backport functionality introduced in Linux 4.2.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <crypto/scatterwalk.h>
#include <crypto/aead.h>
#include <linux/string.h>

static struct scatterlist *scatterwalk_ffwd(struct scatterlist dst[2],
					    struct scatterlist *src,
					    unsigned int len)
{
	for (;;) {
		if (!len)
			return src;

		if (src->length > len)
			break;

		len -= src->length;
		src = sg_next(src);
	}

	sg_init_table(dst, 2);
	sg_set_page(dst, sg_page(src), src->length - len, src->offset + len);
	scatterwalk_crypto_chain(dst, sg_next(src), 0, 2);

	return dst;
}

struct aead_old_request {
	struct scatterlist srcbuf[2];
	struct scatterlist dstbuf[2];
	struct aead_request subreq;
};

unsigned int crypto_aead_reqsize(struct crypto_aead *tfm)
{
	return crypto_aead_crt(tfm)->reqsize + sizeof(struct aead_old_request);
}
EXPORT_SYMBOL_GPL(crypto_aead_reqsize);

struct aead_request *crypto_backport_convert(struct aead_request *req)
{
	struct aead_old_request *nreq = aead_request_ctx(req);
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct scatterlist *src, *dst;

	src = scatterwalk_ffwd(nreq->srcbuf, req->src, req->assoclen);
	dst = req->src == req->dst ?
	      src : scatterwalk_ffwd(nreq->dstbuf, req->dst, req->assoclen);

	aead_request_set_tfm(&nreq->subreq, aead);
	aead_request_set_callback(&nreq->subreq, aead_request_flags(req),
				  req->base.complete, req->base.data);
	aead_request_set_crypt(&nreq->subreq, src, dst, req->cryptlen,
			       req->iv);
	aead_request_set_assoc(&nreq->subreq, req->src, req->assoclen);

	return &nreq->subreq;
}
EXPORT_SYMBOL_GPL(crypto_backport_convert);

char *strreplace(char *s, char old, char new)
{
	for (; *s; ++s)
		if (*s == old)
			*s = new;
	return s;
}
EXPORT_SYMBOL_GPL(strreplace);

/**
 * sg_nents_for_len - return total count of entries in scatterlist
 *                    needed to satisfy the supplied length
 * @sg:		The scatterlist
 * @len:	The total required length
 *
 * Description:
 * Determines the number of entries in sg that are required to meet
 * the supplied length, taking into acount chaining as well
 *
 * Returns:
 *   the number of sg entries needed, negative error on failure
 *
 **/
int sg_nents_for_len(struct scatterlist *sg, u64 len)
{
	int nents;
	u64 total;

	if (!len)
		return 0;

	for (nents = 0, total = 0; sg; sg = sg_next(sg)) {
		nents++;
		total += sg->length;
		if (total >= len)
			return nents;
	}

	return -EINVAL;
}
EXPORT_SYMBOL(sg_nents_for_len);
