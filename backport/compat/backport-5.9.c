#include <linux/device.h>
#include <linux/err.h>
#include <linux/export.h>

int dev_err_probe(const struct device *dev, int err, const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;

	if (err != -EPROBE_DEFER) {
		dev_err(dev, "error %pe: %pV", ERR_PTR(err), &vaf);
	} else {
		dev_dbg(dev, "error %pe: %pV", ERR_PTR(err), &vaf);
	}

	va_end(args);

	return err;
}
EXPORT_SYMBOL_GPL(dev_err_probe);
