/* Automatically created during backport process */
#ifndef CPTCFG_BPAUTO_BUILD_CRC16
#include_next <linux/crc16.h>
#else
#undef crc16
#define crc16 LINUX_BACKPORT(crc16)
#include <linux/backport-crc16.h>
#endif /* CPTCFG_BPAUTO_BUILD_CRC16 */
