/*
 * Linux backport symbols for kernels 3.2.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/of.h>

int hex2bin(u8 *dst, const char *src, size_t count)
{
	while (count--) {
		int hi = hex_to_bin(*src++);
		int lo = hex_to_bin(*src++);

		if ((hi < 0) || (lo < 0))
			return -1;

		*dst++ = (hi << 4) | lo;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(hex2bin);

#ifdef CONFIG_OF
int of_property_read_string_index(struct device_node *np, const char *propname,
				  int index, const char **output)
{
	struct property *prop = of_find_property(np, propname, NULL);
	int i = 0;
	size_t l = 0, total = 0;
	const char *p;

	if (!prop)
		return -EINVAL;
	if (!prop->value)
		return -ENODATA;
	if (strnlen(prop->value, prop->length) >= prop->length)
		return -EILSEQ;

	p = prop->value;

	for (i = 0; total < prop->length; total += l, p += l) {
		l = strlen(p) + 1;
		if ((*p != 0) && (i++ == index)) {
			*output = p;
			return 0;
		}
	}
	return -ENODATA;
}
EXPORT_SYMBOL_GPL(of_property_read_string_index);
#endif
