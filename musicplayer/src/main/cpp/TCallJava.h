//
// Created by tck on 2019/8/7.
//

#ifndef FFMPEGTRAIN_JAVACALL_H
#define FFMPEGTRAIN_JAVACALL_H

#include <jni.h>

class TCallJava {

    jobject jAudioTrackObj;
    jmethodID jAudioTrackWriteMid;
    JavaVM *javaVM;
    JNIEnv *jniEnv;
    jmethodID jPlayerErrorMid;
    jobject jPlayerObj;

public:
    TCallJava(JavaVM *javaVM, JNIEnv *jniEnv, jobject jPlayerObj);
    ~TCallJava();

private:
    void initcreateAudioTrack();

public:
    void callAudioTrackWrite(jbyteArray audioData, int offsetInBytes, int sizeInBytes);

    void callPlayerError(int code, char *msg);
};


#endif //FFMPEGTRAIN_JAVACALL_H
