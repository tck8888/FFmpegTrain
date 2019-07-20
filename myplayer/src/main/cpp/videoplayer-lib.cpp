#include <jni.h>
#include <string>
#include "TCKFFmpeg.h"
#include "TCKPlayStatus.h"

extern "C"
{
#include <libavformat/avformat.h>
}


_JavaVM *javaVM = NULL;
CallJava *callJava = NULL;

TCKFFmpeg *fFmpeg = NULL;
TCKPlayStatus *playstatus = NULL;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("JNI_OnLoad 失败");
        return result;
    }
    LOGD("JNI_OnLoad 成功");
    return JNI_VERSION_1_4;

}


extern "C"
JNIEXPORT void JNICALL
Java_com_tck_myplayer_player_Player_nativePrepared(JNIEnv *env, jobject instance, jstring source_) {
    const char *source = env->GetStringUTFChars(source_, 0);
    if (fFmpeg == NULL) {
        if (callJava == NULL) {
            callJava = new CallJava(javaVM, env, &instance);
        }
        playstatus = new TCKPlayStatus();
        fFmpeg = new TCKFFmpeg(playstatus, callJava, source);
        fFmpeg->prepared();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_myplayer_player_Player_nativeStart(JNIEnv *env, jobject instance) {
    if (fFmpeg != NULL) {
        fFmpeg->start();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tck_myplayer_player_Player_nativePause(JNIEnv *env, jobject instance) {
    if (fFmpeg != NULL) {
        fFmpeg->pause();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_myplayer_player_Player_nativeResume(JNIEnv *env, jobject instance) {
    if (fFmpeg != NULL) {
        fFmpeg->resume();
    }
}