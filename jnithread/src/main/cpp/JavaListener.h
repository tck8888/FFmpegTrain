//
// Created by tck on 2019/7/19.
//

#ifndef FFMPEGTRAIN_JAVALISTENER_H
#define FFMPEGTRAIN_JAVALISTENER_H

#include "jni.h"


class JavaListener {

public:
    JavaVM *jvm;
    _JNIEnv *jenv;
    jobject jobj;
    jmethodID jmid;
public:
    JavaListener(JavaVM *vm, _JNIEnv *env, jobject obj);
    ~JavaListener();

    /**
     * 1:主线程
     * 0：子线程
     * @param type
     * @param code
     * @param msg
     */
    void onError(int type, int code, const char *msg);


};


#endif //FFMPEGTRAIN_JAVALISTENER_H
