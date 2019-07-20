//
// Created by tck on 2019/7/19.
//

#ifndef FFMPEGTRAIN_TCKAUDIO_H
#define FFMPEGTRAIN_TCKAUDIO_H

#include "TCKQueue.h"
#include "TCKPlayStatus.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include <libswresample/swresample.h>
};


class TCKAudio {
public:
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    AVCodecParameters *codecpar = NULL;
    TCKQueue *queue = NULL;
    TCKPlayStatus *playstatus = NULL;

    pthread_t thread_play;
    AVPacket *avPacket = NULL;
    AVFrame *avFrame = NULL;
    int ret = 0;
    uint8_t *buffer = NULL;
    int data_size = 0;

public:
    TCKAudio(TCKPlayStatus *playstatus);

    ~TCKAudio();

    void play();
    int resampleAudio();
};


#endif //FFMPEGTRAIN_TCKAUDIO_H
