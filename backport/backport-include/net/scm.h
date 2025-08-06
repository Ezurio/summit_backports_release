#ifndef __BACKPORT_NET_SCM_H
#define __BACKPORT_NET_SCM_H

#include_next <net/scm.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(3,7,0)
#define scm_creds ucred
#endif

#endif /* __BACKPORT_NET_SCM_H */