obj-$(CPTCFG_LRDMWL)	+= lrdmwl.o
lrdmwl-y			+= main.o
lrdmwl-y			+= mac80211.o
lrdmwl-y			+= fwcmd.o
lrdmwl-y			+= tx.o
lrdmwl-y			+= rx.o
lrdmwl-y			+= isr.o
lrdmwl-y			+= vendor_cmd.o
lrdmwl-$(CONFIG_THERMAL)	+= thermal.o
lrdmwl-$(CONFIG_DEBUG_FS)	+= debugfs.o
lrdmwl-$(CONFIG_SYSFS)		+= sysfs.o

obj-$(CPTCFG_LRDMWL_PCIE) += lrdmwl_pcie.o
lrdmwl_pcie-y += pcie.o
lrdmwl_pcie-y += pfu.o

obj-$(CPTCFG_LRDMWL_SDIO) += lrdmwl_sdio.o
lrdmwl_sdio-y += sdio.o
lrdmwl_sdio-y += gpio.o

obj-$(CPTCFG_LRDMWL_USB) += lrdmwl_usb.o
lrdmwl_usb-y += usb.o
lrdmwl_usb-y += gpio.o

ccflags-y += -D__CHECK_ENDIAN__
