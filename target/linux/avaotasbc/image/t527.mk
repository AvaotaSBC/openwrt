define Device/avaotasbc_avaota-a1
  KERNEL_NAME := Image
  DEVICE_VENDOR := AvaotaSBC
  DEVICE_MODEL := Avaota-A1
  DEVICE_DTS = allwinner/sun55i-t527-avaota-a1
  SYTERKIT_DEV = avaota-a1
  DEVICE_PACKAGES := kmod-mac80211 aic8800-firmware kmod-aic8800-bt kmod-aic8800-wlan
  IMAGE/sysupgrade.img.gz := syterkit-img | gzip | append-metadata
endef
TARGET_DEVICES += avaotasbc_avaota-a1
