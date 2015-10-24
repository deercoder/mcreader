LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#opencv
OPENCVROOT:= /home/changliu/OpenCV-2.4.9-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk

#add by chang
LOCAL_CFLAGS := -DVL_DISABLE_SSE2 -DVL_DISABLE_AVX
#add ends

FILE_LIST := $(wildcard $(LOCAL_PATH)/vlfeat/*.c)
FILE_LIST := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES := com_example_ndk_opencv_androidstudio_NativeClass.cpp
LOCAL_SRC_FILES += $(FILE_LIST)
LOCAL_LDLIBS += -llog -lm
LOCAL_MODULE := MyLib

include $(BUILD_SHARED_LIBRARY)
