//
// Created by tck on 2019/7/19.
//

#ifndef FFMPEGTRAIN_CALLJAVA_H
#define FFMPEGTRAIN_CALLJAVA_H

#include "jni.h"
#include <linux/stddef.h>
#include <cwchar>
#include "AndroidLog.h"

#define MAIN_THREAD 0
#define CHILD_THREAD 1

class CallJava {

public:
    _JavaVM *javaVM = NULL;
    JNIEnv *jniEnv = NULL;
    jobject jobj;

    jmethodID jmid_prepared;
    jmethodID jmid_load;
    jmethodID jmid_timeinfo;
    jmethodID jmid_error;
    jmethodID jmid_complete;
    jmethodID jmid_renderyuv;

public:
    CallJava(_JavaVM *javaVM, JNIEnv *env, jobject *obj);

    ~CallJava();

    void onCallPrepared(int type);

    void onCallLoad(int type, bool load);

    void onCallTimeInfo(int type, int curr, int total);

    void onCallError(int type, int code, char *msg);

    void onCallComplete(int type);

    void onCallRenderYUV(int width,
                         int height,
                         uint8_t *fy,
                         uint8_t *fu,
                         uint8_t *fv);
};

#endif //FFMPEGTRAIN_CALLJAVA_H
