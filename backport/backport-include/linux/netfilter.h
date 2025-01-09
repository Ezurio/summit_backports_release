#ifndef __BACKPORT_LINUX_NETFILTER_H
#define __BACKPORT_LINUX_NETFILTER_H

#if LINUX_VERSION_IS_LESS(3,4,0)
#undef CONFIG_JUMP_LABEL
/* Jump label support changed considerably in newer kernels - to support
 * new-style jump label API used by drivers on older kernels, newer jump labels
 * were backported.
 * Rather than support older jump-label API required by netfilter, just disable
 * netfilter's jump label support for backports drivers to simplify.
 */
#endif /* kernels < 3.4.0 */

#include_next <linux/netfilter.h>

#endif /* __BACKPORT_LINUX_NETFILTER_H */
