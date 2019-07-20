//
// Created by tck on 2019/7/19.
//

#ifndef FFMPEGTRAIN_TCKAUDIO_H
#define FFMPEGTRAIN_TCKAUDIO_H

extern "C"
{
#include "libavcodec/avcodec.h"
};


class TCKAudio {
public:
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    AVCodecParameters *codecpar = NULL;
public:
    TCKAudio();
    ~TCKAudio();
};


#endif //FFMPEGTRAIN_TCKAUDIO_H
