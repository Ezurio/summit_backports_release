#ifndef __BACKPORT_LINUX_ETHTOOL_H
#define __BACKPORT_LINUX_ETHTOOL_H
#include_next <linux/ethtool.h>
#include <linux/version.h>

#ifndef SPEED_UNKNOWN
#define SPEED_UNKNOWN  -1
#endif /* SPEED_UNKNOWN */

#ifndef DUPLEX_UNKNOWN
#define DUPLEX_UNKNOWN 0xff
#endif /* DUPLEX_UNKNOWN */

#ifndef ETHTOOL_FWVERS_LEN
#define ETHTOOL_FWVERS_LEN 32
#endif

#ifndef ETHTOOL_GET_TS_INFO
#define ETHTOOL_GET_TS_INFO	0x00000041
#endif

#if LINUX_VERSION_IS_LESS(3,5,0)
struct ethtool_ts_info {
	__u32	cmd;
	__u32	so_timestamping;
	__s32	phc_index;
	__u32	tx_types;
	__u32	tx_reserved[3];
	__u32	rx_filters;
	__u32	rx_reserved[3];
};
#endif /* < 3.6 */

#if LINUX_VERSION_IS_LESS(6,11,0)
struct kernel_ethtool_ts_info {
	u32 cmd;
	u32 so_timestamping;
	int phc_index;
	int tx_types;
	int rx_filters;
};
#endif /* < 6.11 */

#endif /* __BACKPORT_LINUX_ETHTOOL_H */
