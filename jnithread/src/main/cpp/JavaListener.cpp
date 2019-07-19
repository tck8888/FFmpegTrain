//
// Created by tck on 2019/7/19.
//
#include "JavaListener.h"

JavaListener::JavaListener(JavaVM *vm, _JNIEnv *env, jobject obj) {
    this->jvm = vm;
    this->jenv = env;
    this->jobj = obj;

    jclass clz = env->GetObjectClass(jobj);

    if (!clz) {
        return;
    }
    this->jmid = env->GetMethodID(clz, "onError", "(ILjava/lang/String;)V");
}

JavaListener::~JavaListener() {

}

void JavaListener::onError(int type, int code, const char *msg) {
    if (type == 0) {
        JNIEnv *env;
        jvm->AttachCurrentThread(&env,0);
        jstring jmsg = env->NewStringUTF(msg);
        env->CallVoidMethod(jobj,jmid,code,jmsg);
        env->DeleteLocalRef(jmsg);
        jvm->DetachCurrentThread();
    } else if(type==1){
        jstring jmsg = jenv->NewStringUTF(msg);
        jenv->CallVoidMethod(jobj, jmid, code, jmsg);
        jenv->DeleteLocalRef(jmsg);
    }
}
