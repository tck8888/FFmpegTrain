//
// Created by tck88 on 2019/7/20.
//
#include <jni.h>
#include <string>
#include "AndroidLog.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_tck_musicplayer_AudioPlayer_nativePlay(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    // TODO

    env->ReleaseStringUTFChars(url_, url);
}

