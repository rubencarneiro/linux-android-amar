subdir-ccflags-y += -Werror
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
# In case the platform does NOT support this type of sensors

obj-$(CONFIG_TOUCHSCREEN_NT36XXX)              += NT36XXX/
obj-$(CONFIG_TOUCHSCREEN_HIMAX_CHIPSET)	+= himax83102e_hd_touch/
obj-y += tp_common/
