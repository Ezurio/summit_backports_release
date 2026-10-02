#ifndef _BACKPORT_CRYPTO_SCATTERWALK_H
#define _BACKPORT_CRYPTO_SCATTERWALK_H

#include_next <crypto/scatterwalk.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(2,6,38)
static inline void scatterwalk_crypto_chain(struct scatterlist *head,
					    struct scatterlist *sg,
					    int chain, int num)
{
	if (chain) {
		head->length += sg->length;
		sg = scatterwalk_sg_next(sg);
	}

	if (sg)
		scatterwalk_sg_chain(head, num, sg);
	else
		sg_mark_end(head);
}
#endif /* LINUX_VERSION_IS_LESS(2,38,0) */

#endif /* _BACKPORT_CRYPTO_SCATTERWALK_H */

