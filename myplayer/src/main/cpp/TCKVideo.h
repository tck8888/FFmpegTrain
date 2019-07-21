//
// Created by tck88 on 2019/7/21.
//

#ifndef FFMPEGTRAIN_TCKVIDEO_H
#define FFMPEGTRAIN_TCKVIDEO_H

#include "pthread.h"

#include "TCKQueue.h"
#include "CallJava.h"
#include "TCKPlayStatus.h"

extern "C" {
#include "include/libavcodec/avcodec.h"
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
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

public:
    TCKVideo(TCKPlayStatus *playStatus,CallJava *callJava);

    ~TCKVideo();

    void play();

    void release();
};


#endif //FFMPEGTRAIN_TCKVIDEO_H
