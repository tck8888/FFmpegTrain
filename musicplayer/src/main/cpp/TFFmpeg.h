//
// Created by tck on 2019/8/7.
//

#ifndef FFMPEGTRAIN_TCKFFMPEG_H
#define FFMPEGTRAIN_TCKFFMPEG_H

#include <pthread.h>

#include "TCallJava.h"
#include "TAudio.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
};

class TFFmpeg {

public:
    const char *url = NULL;
    TAudio *tckAudio = NULL;
    TCallJava *callJava = NULL;

    AVFormatContext *pFormatContext=NULL;

public:
    TFFmpeg(TCallJava *callJava, const char *url);

    ~TFFmpeg();

public:
    void play();

    void prepare();

    void prepareAsync();

    void callPlayerJniError(int code, char *msg);

    void release();
};


#endif //FFMPEGTRAIN_TCKFFMPEG_H
