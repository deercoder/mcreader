#ifndef SIFT_ANDROID_LOG_H
#define SIFT_ANDROID_LOG_H


#include<android/log.h>
#define LOGGING_TAG    "MCR_JNI_DEBUGGING"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOGGING_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOGGING_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOGGING_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOGGING_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOGGING_TAG, __VA_ARGS__)


#endif
