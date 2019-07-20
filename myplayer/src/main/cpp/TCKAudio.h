//
// Created by tck on 2019/7/19.
//

#ifndef FFMPEGTRAIN_TCKAUDIO_H
#define FFMPEGTRAIN_TCKAUDIO_H

#include "TCKQueue.h"
#include "TCKPlayStatus.h"
#include "CallJava.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include <libswresample/swresample.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
};


class TCKAudio {
public:
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    AVCodecParameters *codecpar = NULL;
    TCKQueue *queue = NULL;
    TCKPlayStatus *playstatus = NULL;
    CallJava *callJava = NULL;

    pthread_t thread_play;
    AVPacket *avPacket = NULL;
    AVFrame *avFrame = NULL;
    int ret = 0;
    uint8_t *buffer = NULL;
    int data_size = 0;

    int sample_rate = 0;

    int duration = 0;
    AVRational time_base;
    double clock;//总的播放时长
    double now_time;//当前frame时间
    double last_time; //上一次调用时间


    // 引擎接口
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;

    //混音器
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    //pcm
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;

public:
    TCKAudio(TCKPlayStatus *playstatus, int sample_rate,CallJava *callJava);

    ~TCKAudio();

    void play();

    int resampleAudio();

    void initOpenSLES();

    int getCurrentSampleRateForOpensles(int sample_rate);

    void pause();

    void resume();
};


#endif //FFMPEGTRAIN_TCKAUDIO_H
