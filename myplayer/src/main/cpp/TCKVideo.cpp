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
    TCKVideo *tckVideo = static_cast<TCKVideo *> (data);

    while (tckVideo->playStatus != NULL && !tckVideo->playStatus->exit) {


        if (tckVideo->playStatus->seek) {
            av_usleep(1000 * 100);
            continue;
        }

        if (tckVideo->queue->getQueueSize() == 0) {
            if (!tckVideo->playStatus->load) {
                tckVideo->playStatus->load = true;
                tckVideo->callJava->onCallLoad(CHILD_THREAD, true);
            }

            av_usleep(1000 * 100);
            continue;
        } else {
            if (tckVideo->playStatus->load) {
                tckVideo->playStatus->load = false;
                tckVideo->callJava->onCallLoad(CHILD_THREAD, false);
            }
        }

        AVPacket *avPacket = av_packet_alloc();

        if (tckVideo->queue->getAvpacket(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }

        if (avcodec_send_packet(tckVideo->avCodecContext, avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }

        AVFrame *avFrame = av_frame_alloc();
        if (avcodec_receive_frame(tckVideo->avCodecContext, avFrame) != 0) {
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        //解码渲染
        LOGE("线程中获取AVPacket");

        av_frame_free(&avFrame);
        av_free(avFrame);
        avFrame = NULL;
        av_packet_free(&avPacket);
        av_free(avPacket);
        avPacket = NULL;

    }

    pthread_exit(&tckVideo->thread_play);
}

void TCKVideo::play() {

    pthread_create(&thread_play, NULL, playVideo, this);
}

void TCKVideo::release() {
    if (queue != NULL) {
        delete (queue);
        queue = NULL;
    }
    if (avCodecContext != NULL) {
        avcodec_close(avCodecContext);
        avcodec_free_context(&avCodecContext);
        avCodecContext = NULL;
    }

    if (playStatus != NULL) {
        playStatus = NULL;
    }
    if (callJava != NULL) {
        callJava = NULL;
    }
}
