#ifndef __BACKPORT_LINUX_STRING_HELPER_H
#define __BACKPORT_LINUX_STRING_HELPER_H
#include_next <linux/string_helpers.h>

#if LINUX_VERSION_IS_LESS(5,18,0)
#define str_on_off LINUX_BACKPORT(str_on_off)
static inline const char *str_on_off(bool v)
{
	return v ? "on" : "off";
}

#define str_enable_disable LINUX_BACKPORT(str_enable_disable)
static inline const char *str_enable_disable(bool v)
{
	return v ? "enable" : "disable";
}

#define str_enabled_disabled LINUX_BACKPORT(str_enabled_disabled)
static inline const char *str_enabled_disabled(bool v)
{
	return v ? "enabled" : "disabled";
}

#define str_yes_no LINUX_BACKPORT(str_yes_no)
static inline const char *str_yes_no(bool v)
{
	return v ? "yes" : "no";
}
#endif

#ifndef str_disable_enable
#define str_disable_enable(v) str_enable_disable(!(v))
#endif

#if LINUX_VERSION_IS_LESS(6,5,0)
#define str_high_low LINUX_BACKPORT(str_high_low)
static inline const char *str_high_low(bool v)
{
	return v ? "high" : "low";
}
#endif

#if LINUX_VERSION_IS_LESS(6,12,0)
#define str_true_false LINUX_BACKPORT(str_true_false)
static inline const char *str_true_false(bool v)
{
	return v ? "true" : "false";
}
#endif

#endif
