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

bool nexit = true;

pthread_t thread_start;

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
        callJava->onCallLoad(MAIN_THREAD, true);
        playstatus = new TCKPlayStatus();
        fFmpeg = new TCKFFmpeg(playstatus, callJava, source);
        fFmpeg->prepared();
    }
}

void *startCallBack(void *data)
{
    TCKFFmpeg *fFmpeg = (TCKFFmpeg *) data;
    fFmpeg->start();
    pthread_exit(&thread_start);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_tck_myplayer_player_Player_nativeStart(JNIEnv *env, jobject instance) {
    if(fFmpeg != NULL)
    {
        pthread_create(&thread_start, NULL, startCallBack, fFmpeg);
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

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_myplayer_player_Player_nativeStop(JNIEnv *env, jobject instance) {

    if (!nexit) {
        return;
    }
    nexit = false;

    if (fFmpeg != NULL) {
        fFmpeg->release();
        delete (fFmpeg);
        fFmpeg = NULL;
        if (callJava != NULL) {
            delete (callJava);
            callJava = NULL;
        }
        if (playstatus != NULL) {
            delete (playstatus);
            playstatus = NULL;
        }
    }
    nexit = true;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_myplayer_player_Player_nativeSeek(JNIEnv *env, jobject instance, jint secds) {
    if (fFmpeg != NULL) {
        fFmpeg->seek(secds);
    }
}