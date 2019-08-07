//
// Created by tck on 2019/8/7.
//

#include "TFFmpeg.h"

TFFmpeg::TFFmpeg(TCallJava *callJava, const char *url) {
    this->callJava = callJava;
    this->url = url;
}

TFFmpeg::~TFFmpeg() {
    release();
}

void TFFmpeg::play() {

}

void TFFmpeg::prepare() {
    av_register_all();
    avformat_network_init();

    int ret = avformat_open_input(&pFormatContext, url, NULL, NULL);

    if (ret != 0) {

        LOGE("avformat_open_input error");

    }

}

void TFFmpeg::prepareAsync() {


}

void TFFmpeg::callPlayerJniError(int code, char *msg) {


}

void TFFmpeg::release() {

}


