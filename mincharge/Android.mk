# 
# Android.mk for charger
# 

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    mincharge.c

LOCAL_MODULE := libmincharge

LOCAL_CFLAGS += -Wall

ifdef RECOVERY_CHARGE_ENABLE
LOCAL_CFLAGS += -DRECOVERY_CHARGE_ENABLE
endif

include $(BUILD_STATIC_LIBRARY)

