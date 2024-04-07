# SPDX-License-Identifier: GPL-2.0-or-later

include $(TOPDIR)/rules.mk

PKG_NAME:=aic8800-firmware
PKG_RELEASE:=7

PKG_MAINTAINER:=Yuzuki <xxxxx@gmail.com>

include $(INCLUDE_DIR)/package.mk

define Package/aic8800-firmware-default
  SECTION:=firmware
  CATEGORY:=Firmware
endef

define Build/Compile
	true
endef

define Package/aic8800-firmware
  $(Package/aic8800-firmware-default)
  TITLE:=AIC8800 Firmware
endef

define Package/aic8800-firmware/install
	$(INSTALL_DIR) $(1)/lib/firmware/aic8800d80/
	$(INSTALL_DATA) ./files/aic8800/fmacfw.bin $(1)/lib/firmware/fmacfw.bin
	$(INSTALL_DATA) ./files/aic8800/fmacfw_usb.bin $(1)/lib/firmware/fmacfw_usb.bin
	$(INSTALL_DATA) ./files/aic8800/fmacfw_rf.bin $(1)/lib/firmware/fmacfw_rf.bin
	$(INSTALL_DATA) ./files/aic8800/fmacfw_rf_usb.bin $(1)/lib/firmware/fmacfw_rf_usb.bin
	$(INSTALL_DATA) ./files/aic8800/fw_adid.bin $(1)/lib/firmware/fw_adid.bin
	$(INSTALL_DATA) ./files/aic8800/fw_adid_u03.bin $(1)/lib/firmware/fw_adid_u03.bin
	$(INSTALL_DATA) ./files/aic8800/fw_patch.bin $(1)/lib/firmware/fw_patch.bin
	$(INSTALL_DATA) ./files/aic8800/fw_patch_u03.bin $(1)/lib/firmware/fw_patch_u03.bin
	$(INSTALL_DATA) ./files/aic8800/fw_patch_table.bin $(1)/lib/firmware/fw_patch_table.bin
	$(INSTALL_DATA) ./files/aic8800/fw_patch_table_u03.bin $(1)/lib/firmware/fw_patch_table_u03.bin
	$(INSTALL_DATA) ./files/aic8800/aic_userconfig.txt $(1)/lib/firmware/aic_userconfig.txt
	$(INSTALL_DATA) ./files/aic8800/aic8800d80/* $(1)/lib/firmware/aic8800d80/
endef

$(eval $(call BuildPackage,aic8800-firmware))
