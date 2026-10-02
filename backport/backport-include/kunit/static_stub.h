#ifndef _BACKPORT_LINUX_KUNIT_STATIC_STUB_H
#define _BACKPORT_LINUX_KUNIT_STATIC_STUB_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,3,0)
#include_next <kunit/static_stub.h>
#endif

#ifndef KUNIT_STATIC_STUB_REDIRECT
#define KUNIT_STATIC_STUB_REDIRECT(real_fn_name, args...) do {} while (0)
#endif

#endif /* _BACKPORT_LINUX_KUNIT_STATIC_STUB_H */
