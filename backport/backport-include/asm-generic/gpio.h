/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BACKPORT_ASM_GENERIC_GPIO_H
#define __BACKPORT_ASM_GENERIC_GPIO_H

#include_next <asm-generic/gpio.h>

#if LINUX_VERSION_IS_LESS(3,3,0)
#include <linux/of.h>
#endif

#endif /* __BACKPORT_ASM_GENERIC_GPIO_H */
