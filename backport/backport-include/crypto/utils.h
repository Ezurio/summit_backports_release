#ifndef _BACKPORT_CRYPTO_UTILS_H
#define _BACKPORT_CRYPTO_UTILS_H

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,4,0)
#include_next <crypto/utils.h>
#else
#include <crypto/algapi.h>
#endif

#endif
