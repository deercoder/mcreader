# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#For OpenCV
OPENCV_MK_PATH:=../OpenCV-2.3.1/share/OpenCV/OpenCV.mk
ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
	#try to load OpenCV.mk from default install location
	include $(TOOLCHAIN_PREBUILT_ROOT)/user/share/OpenCV/OpenCV.mk
else
	include $(OPENCV_MK_PATH)
endif

LOCAL_MODULE    := MCRindep
LOCAL_SRC_FILES := MCRindep.cpp DisplayMatcher.cpp generic.c heap.c host.c Image.cpp imop.c kdtree.c mathop_sse2.c mathop.c random.c sift.c siftmatcher.cpp 

#Enables use of exceptions
LOCAL_CPPFLAGS += -fexceptions

#Enables use of the android logging library files
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

#Forces to compile to this platform to allow compatability with OpenCV
TARGET_ARCH_ABI := armeabi-v7a

include $(BUILD_SHARED_LIBRARY)
