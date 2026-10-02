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

#if LINUX_VERSION_IS_LESS(5,5,0)
#undef EXPORT_SYMBOL_NS
#define EXPORT_SYMBOL_NS(sym, ns) EXPORT_SYMBOL(sym)
#elif LINUX_VERSION_IS_LESS(6,13,0)
#undef EXPORT_SYMBOL_NS
#define EXPORT_SYMBOL_NS(sym, ns) __EXPORT_SYMBOL(sym, "", ns)
#endif /* < 6.13.0 */

#if LINUX_VERSION_IS_LESS(5,5,0)
#undef EXPORT_SYMBOL_NS_GPL
#define EXPORT_SYMBOL_NS_GPL(sym, ns) EXPORT_SYMBOL_GPL(sym)
#elif LINUX_VERSION_IS_LESS(6,5,0)
#undef EXPORT_SYMBOL_NS_GPL
#define EXPORT_SYMBOL_NS_GPL(sym, ns) __EXPORT_SYMBOL(sym, "_gpl", ns)
#elif LINUX_VERSION_IS_LESS(6,13,0)
#undef EXPORT_SYMBOL_NS_GPL
#define EXPORT_SYMBOL_NS_GPL(sym, ns) __EXPORT_SYMBOL(sym, "GPL", ns)
#endif /* < 6.13.0 */

#endif	/* _COMPAT_LINUX_EXPORT_H */
