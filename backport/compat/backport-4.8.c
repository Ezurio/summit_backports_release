/*
 * Copyright(c) 2017 Intel Deutschland GmbH
 *
 * Backport functionality introduced in Linux 4.8.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/pci.h>

#ifdef CONFIG_PCI
#ifdef CONFIG_PCI_MSI

/**
 * pci_alloc_irq_vectors - allocate multiple IRQs for a device
 * @dev:		PCI device to operate on
 * @min_vecs:		minimum number of vectors required (must be >= 1)
 * @max_vecs:		maximum (desired) number of vectors
 * @flags:		flags or quirks for the allocation
 *
 * Allocate up to @max_vecs interrupt vectors for @dev, using MSI-X or MSI
 * vectors if available, and fall back to a single legacy vector
 * if neither is available.  Return the number of vectors allocated,
 * (which might be smaller than @max_vecs) if successful, or a negative
 * error code on error. If less than @min_vecs interrupt vectors are
 * available for @dev the function will fail with -ENOSPC.
 *
 * To get the Linux IRQ number used for a vector that can be passed to
 * request_irq() use the pci_irq_vector() helper.
 */
int pci_alloc_irq_vectors(struct pci_dev *dev, unsigned int min_vecs,
		unsigned int max_vecs, unsigned int flags)
{
	int vecs = -ENOSPC;

	if (flags & PCI_IRQ_MSIX) {
		vecs = pci_enable_msix_range(dev, NULL, min_vecs, max_vecs);
		if (vecs > 0)
			return vecs;
	}

	if (flags & PCI_IRQ_MSI) {
		vecs = pci_enable_msi_range(dev, min_vecs, max_vecs);
		if (vecs > 0)
			return vecs;
	}

	/* use legacy irq if allowed */
	if ((flags & PCI_IRQ_INTX) && min_vecs == 1) {
		pci_intx(dev, 1);
		return 1;
	}

	return vecs;
}
EXPORT_SYMBOL_GPL(pci_alloc_irq_vectors);
#endif /* CONFIG_PCI_MSI */
#endif /* CONFIG_PCI */
