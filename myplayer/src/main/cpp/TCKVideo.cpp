//
// Created by tck88 on 2019/7/21.
//

#include "TCKVideo.h"

TCKVideo::TCKVideo(TCKPlayStatus *playStatus, CallJava *callJava) {
    this->playStatus = playStatus;
    this->callJava = callJava;
    this->queue = new TCKQueue(playStatus);
}

TCKVideo::~TCKVideo() {

}

void *playVideo(void *data) {
    TCKVideo *tckVideo = (TCKVideo *) data;

    while (tckVideo->playStatus != NULL && !tckVideo->playStatus->exit) {
        AVPacket *avPacket = av_packet_alloc();
        if (tckVideo->queue->getAvpacket(avPacket) == 0) {
            //解码渲染
            LOGE("线程中获取AVPacket");

        }

        av_packet_free(&avPacket);
        av_free(avPacket);
        avPacket = NULL;
    }

    pthread_exit(&tckVideo->thread_play);
}

void TCKVideo::play() {

    pthread_create(&thread_play, NULL, playVideo, this);
}
