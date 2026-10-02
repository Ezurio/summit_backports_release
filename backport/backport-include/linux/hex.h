#ifndef __BACKPORT_LINUX_HEX_H
#define __BACKPORT_LINUX_HEX_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,4,0)
#include_next <linux/hex.h>
#else
#include <linux/kernel.h>

#if LINUX_VERSION_IS_LESS(3,18,0)
#define bin2hex LINUX_BACKPORT(bin2hex)
extern char *bin2hex(char *dst, const void *src, size_t count);
#endif /* < 3.18 */

#if LINUX_VERSION_IS_LESS(3,2,0)
#define hex_byte_pack pack_hex_byte

/* kernels before 3.2 didn't have error checking for the function */
#define hex2bin LINUX_BACKPORT(hex2bin)
int __must_check hex2bin(u8 *dst, const char *src, size_t count);
#endif /* < 3.2 */

#endif /* < 6.4 */

#endif /* __BACKPORT_LINUX_HEX_H */
