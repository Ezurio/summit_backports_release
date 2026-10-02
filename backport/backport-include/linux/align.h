#ifndef _BACKPORTS_LINUX_ALIGN_H
#define _BACKPORTS_LINUX_ALIGN_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(5,13,0)
#include_next <linux/align.h>
#else /* < 5.13 */
#include <linux/kernel.h> 
#endif /* < 5.13 */

#endif /* _BACKPORTS_LINUX_ALIGN_H */
