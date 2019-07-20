//
// Created by tck88 on 2019/7/20.
//

#ifndef FFMPEGTRAIN_TCKQUEUE_H
#define FFMPEGTRAIN_TCKQUEUE_H

#include "queue"
#include "pthread.h"

#include "AndroidLog.h"
#include "TCKPlayStatus.h"

extern "C"
{
#include "libavcodec/avcodec.h"
};

class TCKQueue {
public:
    std::queue<AVPacket *> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    TCKPlayStatus *playstatus = NULL;
public:
    TCKQueue(TCKPlayStatus *playstatus);

    ~TCKQueue();

    int putAvpacket(AVPacket *packet);

    int getAvpacket(AVPacket *packet);

    int getQueueSize();

};


#endif //FFMPEGTRAIN_TCKQUEUE_H
