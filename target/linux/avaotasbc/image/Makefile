# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2013 OpenWrt.org

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

DEVICE_VARS += UBOOT_DEVICE_NAME

FAT32_BLOCK_SIZE=1024
FAT32_BLOCKS=$(shell echo $$(($(CONFIG_TARGET_KERNEL_PARTSIZE)*1024*1024/$(FAT32_BLOCK_SIZE))))

define Build/syterkit-img
	# Creates the final SD/eMMC images, 
	# combining boot partition, root partition as well as the u-boot bootloader

	rm -f $@.boot
	mkfs.fat $@.boot -C $(FAT32_BLOCKS)

	mcopy -i $@.boot $(DTS_DIR)/$(DEVICE_DTS).dtb ::sunxi.dtb
	mcopy -i $@.boot $(IMAGE_KERNEL) ::kernel.img
	mcopy -i $@.boot -s extlinux ::/
	mcopy -i $@.boot bl31.bin ::bl31.bin
	mcopy -i $@.boot scp.bin ::scp.bin
	mcopy -i $@.boot splash.bin ::splash.bin
	./gen_sunxi_sdcard_img.sh $@ \
		$@.boot \
		$(IMAGE_ROOTFS) \
		$(CONFIG_TARGET_KERNEL_PARTSIZE) \
		$(CONFIG_TARGET_ROOTFS_PARTSIZE) \
		syterkit/$(SYTERKIT_DEV)_extlinux_boot_bin_card.bin
	rm -f $@.boot

endef

### Devices ###
define Device/Default
  PROFILES := Default
  KERNEL := kernel-bin
  IMAGES := sysupgrade.img.gz
endef

include $(SUBTARGET).mk

$(eval $(call BuildImage))
