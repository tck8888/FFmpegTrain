//
// Created by tck on 2019/7/19.
//


#include "TCKFFmpeg.h"

TCKFFmpeg::TCKFFmpeg(TCKPlayStatus *playstatus, CallJava *callJava, const char *url) {
    this->playstatus = playstatus;
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
    av_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) {
        if (LOG_DEBUG) {
            LOGE("avformat_open_input error   %s", url);
        }
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        if (LOG_DEBUG) {
            LOGE("avformat_find_stream_info error   %s", url);
        }
        return;
    }

    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        //4. 得到音频流
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (audio == NULL) {
                audio = new TCKAudio(playstatus);
                audio->streamIndex = i;
                audio->codecpar = pFormatCtx->streams[i]->codecpar;
            }
        }
    }

    //5. 获取解码器
    AVCodec *dec = avcodec_find_decoder(audio->codecpar->codec_id);
    if (!dec) {
        if (LOG_DEBUG) {
            LOGE("can not find decoder");
        }
        return;
    }
    //6.解码器上下文
    audio->avCodecContext = avcodec_alloc_context3(dec);
    if (!audio->avCodecContext) {
        if (LOG_DEBUG) {
            LOGE("can not alloc new decodecctx");
        }
        return;
    }

    if (avcodec_parameters_to_context(audio->avCodecContext, audio->codecpar) < 0) {
        if (LOG_DEBUG) {
            LOGE("can not fill avCodecContext");
        }
        return;
    }
    //7.打开解码器
    if (avcodec_open2(audio->avCodecContext, dec, 0) != 0) {
        if (LOG_DEBUG) {
            LOGE("cant not open audio streams");
        }
        return;
    }
    callJava->onCallPrepared(CHILD_THREAD);
}

void TCKFFmpeg::start() {
    LOGE(" start");
    if (audio == NULL) {
        if (LOG_DEBUG) {
            LOGE("audio is null");
            return;
        }
    }

    int count = 0;

    while (1) {
        AVPacket *avPacket = av_packet_alloc();

        if (av_read_frame(pFormatCtx, avPacket) == 0) {
            if (avPacket->stream_index == audio->streamIndex) {

                //解码操作
                count++;
                if (LOG_DEBUG) {
                    LOGE("解码第 %d 帧", count);
                }
                audio->queue->putAvpacket(avPacket);
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            if (LOG_DEBUG) {
                LOGE("decode finished");
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            break;
        }
    }

    //模拟出队
    while (audio->queue->getQueueSize() > 0) {
        AVPacket *packet = av_packet_alloc();
        audio->queue->getAvpacket(packet);
        av_packet_free(&packet);
        av_free(packet);
        packet = NULL;
    }

    if (LOG_DEBUG) {
        LOGD("解码完成");
    }

}
