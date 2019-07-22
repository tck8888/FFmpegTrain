//
// Created by tck88 on 2019/7/21.
//

#ifndef FFMPEGTRAIN_TCKVIDEO_H
#define FFMPEGTRAIN_TCKVIDEO_H

#include "pthread.h"

#include "TCKQueue.h"
#include "CallJava.h"
#include "TCKPlayStatus.h"
#include "TCKAudio.h"

extern "C"
{
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libavcodec/avcodec.h>
};

class TCKVideo {

public:
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    AVCodecParameters *codecpar = NULL;
    TCKQueue *queue = NULL;
    TCKPlayStatus *playStatus = NULL;
    CallJava *callJava = NULL;

    AVRational time_base;

    pthread_t thread_play;

    TCKAudio *audio = NULL;

    int clock = 0;

    double delayTime = 0;

    double defaultDelayTime = 0.04;

public:
    TCKVideo(TCKPlayStatus *playStatus, CallJava *callJava);

    ~TCKVideo();

    void play();

    void release();

    double getFrameDiff(AVFrame *avFrame);

    double getDelayTime(double diff);
};


#endif //FFMPEGTRAIN_TCKVIDEO_H
