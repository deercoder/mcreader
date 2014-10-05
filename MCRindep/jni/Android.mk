# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless require~/OpenCV-2.4.9-android-sdk/sdk/native/jni/OpenCV.mk
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_INSTALL_MODULES:=on

#For OpenCV
OPENCV_MK_PATH:=/home/changliu/OpenCV-2.4.9-android-sdk/sdk/native/jni/OpenCV.mk
include $(OPENCV_MK_PATH)

LOCAL_MODULE    := MCRindep
LOCAL_SRC_FILES := MCRindep.cpp DisplayMatcher.cpp generic.c heap.c host.c Image.cpp imop.c kdtree.c mathop_sse2.c mathop.c random.c sift.c siftmatcher.cpp

#Enables use of exceptions
LOCAL_CPPFLAGS += -fexceptions

#Enables use of the android logging library files
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

#Forces to compile to this platform to allow compatability with OpenCV
TARGET_ARCH_ABI := armeabi-v7a

include $(BUILD_SHARED_LIBRARY)
