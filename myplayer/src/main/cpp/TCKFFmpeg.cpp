//
// Created by tck on 2019/7/19.
//

#include "TCKFFmpeg.h"

TCKFFmpeg::TCKFFmpeg(CallJava *callJava, const char *url) {
    this->callJava = callJava;
    this->url = url;
}

TCKFFmpeg::~TCKFFmpeg() {

}


void *decodeFFmpeg(void *data) {
    TCKFFmpeg *tckFFmpeg = (TCKFFmpeg *) data;
    tckFFmpeg->decodeFFmpegThread();
    pthread_exit(&tckFFmpeg->decodeThread);
}

void TCKFFmpeg::prepared() {
    LOGD("开始初始化")
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}

void TCKFFmpeg::decodeFFmpegThread() {
    avcodec_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();

    // if (avformat_open_input(&pFormatCtx,url,))

    // avformat_open_input(&pFormatCtx);
}

void TCKFFmpeg::start() {

}
