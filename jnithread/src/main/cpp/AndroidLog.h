
#ifndef JNITHREAD_ANDROIDLOG_H_H
#define JNITHREAD_ANDROIDLOG_H_H

#endif //JNITHREAD_ANDROIDLOG_H_H

#include <android/log.h>

#define LOGD(FORMAT,...) __android_log_print(ANDROID_LOG_DEBUG, "tck6666", FORMAT, ##__VA_ARGS__);

