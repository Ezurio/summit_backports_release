#ifndef _COMPAT_LINUX_EXPORT_H
#define _COMPAT_LINUX_EXPORT_H 1

#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(3,2,0)
#include_next <linux/export.h>
#else

#ifndef pr_fmt
#define backport_undef_pr_fmt
#endif /* pr_fmt */

#include <linux/compat.h>
#include <linux/module.h>

#ifdef backport_undef_pr_fmt
#undef pr_fmt
#undef backport_undef_pr_fmt
#endif /* backport_undef_pr_fmt */

#endif /* LINUX_VERSION_IS_GEQ(3,2,0) */

#ifndef EXPORT_SYMBOL_NS_GPL
#define EXPORT_SYMBOL_NS_GPL(sym, ns) EXPORT_SYMBOL_GPL(sym)
#endif /* EXPORT_SYMBOL_NS_GPL */

#endif	/* _COMPAT_LINUX_EXPORT_H */
