#ifndef __BACKPORT_INTERNAL_CIPHER_H
#define __BACKPORT_INTERNAL_CIPHER_H
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(5,12,0)
#include <linux/crypto.h>
#else
#include_next <crypto/internal/cipher.h>
#endif /* LINUX_VERSION_IS_LESS(5,12,0) */

#endif /* __BACKPORT_INTERNAL_CIPHER_H */
