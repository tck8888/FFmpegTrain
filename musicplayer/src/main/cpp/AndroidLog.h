//
// Created by yangw on 2018-2-28.
//

#ifndef MYMUSIC_ANDROIDLOG_H
#define MYMUSIC_ANDROIDLOG_H


#include "android/log.h"

#define LOG_DEBUG true

#define LOGD(FORMAT,...) __android_log_print(ANDROID_LOG_DEBUG,"tck6666",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"tck6666",FORMAT,##__VA_ARGS__);

#define AUDIO_SAMPLE_RATE 44100

#endif //MYMUSIC_ANDROIDLOG_H