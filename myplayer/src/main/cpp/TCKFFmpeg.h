//
// Created by tck on 2019/7/19.
//

#ifndef FFMPEGTRAIN_TCKFFMPEG_H
#define FFMPEGTRAIN_TCKFFMPEG_H


#include "pthread.h"

#include "CallJava.h"
#include "pthread.h"
#include "TCKAudio.h"
#include "TCKPlayStatus.h"

extern "C"
{

#include <libavformat/avformat.h>
};


class TCKFFmpeg {
public:
    CallJava *callJava = NULL;
    const char *url = NULL;
    pthread_t decodeThread;
    AVFormatContext *pFormatCtx = NULL;
    TCKAudio *audio = NULL;
    TCKPlayStatus *playstatus = NULL;
public:
    TCKFFmpeg(TCKPlayStatus *playstatus, CallJava *callJava, const char *url);

    ~TCKFFmpeg();

    void prepared();

    void decodeFFmpegThread();

    void start();

    void pause();

    void resume();
};


#endif //FFMPEGTRAIN_TCKFFMPEG_H
