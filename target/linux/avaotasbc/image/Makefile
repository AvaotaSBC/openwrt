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
	
	rm -f uImage
	mkimage -A arm -O linux -T kernel -C none -a "0x41000000" -e "0x41000000" -n "Linux kernel" -d $(IMAGE_KERNEL) uImage

	mcopy -i $@.boot $(DTS_DIR)/$(DEVICE_DTS).dtb ::sunxi.dtb
	mcopy -i $@.boot uImage ::kernel.img
	mcopy -i $@.boot -s extlinux ::/
	./gen_sdcard_image.sh $@ \
		$@.boot \
		$(IMAGE_ROOTFS) \
		$(CONFIG_TARGET_KERNEL_PARTSIZE) \
		$(CONFIG_TARGET_ROOTFS_PARTSIZE) \
		sunxi-uboot/boot0_sdcard.fex \
		sunxi-uboot/boot_package.fex \
		36864
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
