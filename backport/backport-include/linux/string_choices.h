#ifndef __BACKPORT_LINUX_STRING_CHOICES_H
#define __BACKPORT_LINUX_STRING_CHOICES_H
#include_next <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,5,0)
#include_next <linux/string_choices.h>
#else
#include <linux/string_helpers.h>
#endif

#endif
