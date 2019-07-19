#include <jni.h>
#include <string>

#include "pthread.h"

#include "AndroidLog.h"

pthread_t thread;

void *normalCallBack(void *data) {
    LOGD("create normal thread from C++!");
    pthread_exit(&thread);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_jnithread_JNIThread_normalThread(JNIEnv *env, jobject instance) {

    pthread_create(&thread, NULL, normalCallBack, NULL);
}