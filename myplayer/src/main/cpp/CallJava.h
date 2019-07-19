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

    jmethodID jmid_parpared;

public:
    CallJava(_JavaVM *javaVM, JNIEnv *env, jobject *obj);

    ~CallJava();

    void onCallPrepared(int type);

};

#endif //FFMPEGTRAIN_CALLJAVA_H