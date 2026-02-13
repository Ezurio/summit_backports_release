#ifndef __BACKPORT_LINUX_STRING_HELPER_H
#define __BACKPORT_LINUX_STRING_HELPER_H
#include_next <linux/string_helpers.h>

#if LINUX_VERSION_IS_LESS(6,5,0)
#define str_high_low LINUX_BACKPORT(str_high_low)
static inline const char *str_high_low(bool v)
{
	return v ? "high" : "low";
}
#endif

#if LINUX_VERSION_IS_LESS(5,18,0)
#define str_on_off LINUX_BACKPORT(str_on_off)
static inline const char *str_on_off(bool v)
{
	return v ? "on" : "off";
}
#endif

#endif