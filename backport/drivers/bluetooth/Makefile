# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the Linux Bluetooth HCI device drivers.
#

obj-$(CPTCFG_BT_HCIVHCI)	+= hci_vhci.o
obj-$(CPTCFG_BT_HCIUART)	+= hci_uart.o
obj-$(CPTCFG_BT_HCIBCM203X)	+= bcm203x.o
obj-$(CPTCFG_BT_HCIBCM4377)	+= hci_bcm4377.o
obj-$(CPTCFG_BT_HCIBPA10X)	+= bpa10x.o
obj-$(CPTCFG_BT_HCIBFUSB)	+= bfusb.o
obj-$(CPTCFG_BT_HCIDTL1)	+= dtl1_cs.o
obj-$(CPTCFG_BT_HCIBT3C)	+= bt3c_cs.o
obj-$(CPTCFG_BT_HCIBLUECARD)	+= bluecard_cs.o
obj-$(CPTCFG_BT_HCISS1USB)	+= ss1btusb.o
#obj-$(CPTCFG_BT_HCIBTUART)	+= btuart_cs.o

obj-$(CPTCFG_BT_HCIBTUSB)	+= btusb.o
obj-$(CPTCFG_BT_HCIBTSDIO)	+= btsdio.o

obj-$(CPTCFG_BT_INTEL)		+= btintel.o
obj-$(CPTCFG_BT_ATH3K)		+= ath3k.o
obj-$(CPTCFG_BT_TI)			+= btti.o
obj-$(CPTCFG_BT_TI_SDIO)	+= btti_sdio.o
obj-$(CPTCFG_BT_TI_UART)	+= btti_uart.o
obj-$(CPTCFG_BT_MRVL)		+= btmrvl.o
obj-$(CPTCFG_BT_MRVL_SDIO)	+= btmrvl_sdio.o
obj-$(CPTCFG_BT_MTKSDIO)	+= btmtksdio.o
obj-$(CPTCFG_BT_MTKUART)	+= btmtkuart.o
obj-$(CPTCFG_BT_QCOMSMD)	+= btqcomsmd.o
obj-$(CPTCFG_BT_BCM)		+= btbcm.o
obj-$(CPTCFG_BT_RTL)		+= btrtl.o
obj-$(CPTCFG_BT_QCA)		+= btqca.o
obj-$(CPTCFG_BT_MTK)		+= btmtk.o

obj-$(CPTCFG_BT_NXPUART)	+= btnxpuart.o

obj-$(CPTCFG_BT_VIRTIO)		+= virtio_bt.o
obj-$(CPTCFG_BT_NXPUART)	+= btnxpuart.o

obj-$(CPTCFG_BT_HCIUART_NOKIA)	+= hci_nokia.o

obj-$(CPTCFG_BT_HCIRSI)		+= btrsi.o

btti-y			:= btti_main.o
btti-$(CONFIG_DEBUG_FS)	+= btti_debugfs.o

btmrvl-y			:= btmrvl_main.o
btmrvl-$(CONFIG_DEBUG_FS)	+= btmrvl_debugfs.o

hci_uart-y				:= hci_ldisc.o
hci_uart-$(CPTCFG_BT_HCIUART_SERDEV)	+= hci_serdev.o
hci_uart-$(CPTCFG_BT_HCIUART_H4)	+= hci_h4.o
hci_uart-$(CPTCFG_BT_HCIUART_BCSP)	+= hci_bcsp.o
hci_uart-$(CPTCFG_BT_HCIUART_LL)	+= hci_ll.o
hci_uart-$(CPTCFG_BT_HCIUART_ATH3K)	+= hci_ath.o
hci_uart-$(CPTCFG_BT_HCIUART_3WIRE)	+= hci_h5.o
hci_uart-$(CPTCFG_BT_HCIUART_INTEL)	+= hci_intel.o
hci_uart-$(CPTCFG_BT_HCIUART_BCM)	+= hci_bcm.o
hci_uart-$(CPTCFG_BT_HCIUART_QCA)	+= hci_qca.o
hci_uart-$(CPTCFG_BT_HCIUART_AG6XX)	+= hci_ag6xx.o
hci_uart-$(CPTCFG_BT_HCIUART_MRVL)	+= hci_mrvl.o
hci_uart-objs				:= $(hci_uart-y)
