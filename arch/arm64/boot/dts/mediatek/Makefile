# SPDX-License-Identifier: GPL-2.0
dtb-$(CONFIG_ARCH_MEDIATEK) += mt6765.dtb
dtb-$(CONFIG_ARCH_MEDIATEK) += amar_row_wifi.dtb

ifeq ($(strip $(CONFIG_BUILD_ARM64_APPENDED_DTB_IMAGE)), y)
dtb-y += $(addsuffix .dtb, $(notdir $(subst $\",,$(CONFIG_BUILD_ARM64_APPENDED_DTB_IMAGE_NAMES))))
endif

ifeq ($(strip $(CONFIG_BUILD_ARM64_DTB_OVERLAY_IMAGE)), y)
dtb-y += $(addsuffix .dtb, $(notdir $(subst $\",,$(CONFIG_BUILD_ARM64_DTB_OVERLAY_IMAGE_NAMES))))
endif

always		:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb
