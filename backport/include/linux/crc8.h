/* Automatically created during backport process */
#ifndef CPTCFG_BPAUTO_BUILD_CRC8
#include_next <linux/crc8.h>
#else
#undef crc8_populate_msb
#define crc8_populate_msb LINUX_BACKPORT(crc8_populate_msb)
#undef crc8_populate_lsb
#define crc8_populate_lsb LINUX_BACKPORT(crc8_populate_lsb)
#undef crc8
#define crc8 LINUX_BACKPORT(crc8)
#include <linux/backport-crc8.h>
#endif /* CPTCFG_BPAUTO_BUILD_CRC8 */
