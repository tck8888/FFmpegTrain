//
// Created by tck on 2019/7/19.
//


#include "TCKFFmpeg.h"

TCKFFmpeg::TCKFFmpeg(TCKPlayStatus *playstatus, CallJava *callJava, const char *url) {
    this->playstatus = playstatus;
    this->callJava = callJava;
    this->url = url;
    exit = false;
    pthread_mutex_init(&init_mutex, NULL);
}

TCKFFmpeg::~TCKFFmpeg() {
    pthread_mutex_destroy(&init_mutex);
}


void *decodeFFmpeg(void *data) {
    TCKFFmpeg *tckFFmpeg = (TCKFFmpeg *) data;
    tckFFmpeg->decodeFFmpegThread();
    pthread_exit(&tckFFmpeg->decodeThread);
}

void TCKFFmpeg::prepared() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}

int avformat_callback(void *ctx) {
    TCKFFmpeg *fFmpeg = (TCKFFmpeg *) ctx;
    if (fFmpeg->playstatus->exit) {
        return AVERROR_EOF;
    }
    return 0;
}

void TCKFFmpeg::decodeFFmpegThread() {

    pthread_mutex_lock(&init_mutex);

    av_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();
    pFormatCtx->interrupt_callback.callback = avformat_callback;
    pFormatCtx->interrupt_callback.opaque = this;

    if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) {
        if (LOG_DEBUG) {
            LOGE("avformat_open_input error   %s", url);
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        if (LOG_DEBUG) {
            LOGE("avformat_find_stream_info error   %s", url);
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }

    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        //4. 得到音频流
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (audio == NULL) {
                audio = new TCKAudio(playstatus, pFormatCtx->streams[i]->codecpar->sample_rate,
                                     callJava);
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
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    //6.解码器上下文
    audio->avCodecContext = avcodec_alloc_context3(dec);
    if (!audio->avCodecContext) {
        if (LOG_DEBUG) {
            LOGE("can not alloc new decodecctx");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }

    if (avcodec_parameters_to_context(audio->avCodecContext, audio->codecpar) < 0) {
        if (LOG_DEBUG) {
            LOGE("can not fill avCodecContext");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    //7.打开解码器
    if (avcodec_open2(audio->avCodecContext, dec, 0) != 0) {
        if (LOG_DEBUG) {
            LOGE("cant not open audio streams");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    if (callJava != NULL) {
        if (playstatus != NULL && !playstatus->exit) {
            callJava->onCallPrepared(CHILD_THREAD);
        } else {
            exit = true;
        }
    }
    pthread_mutex_unlock(&init_mutex);
}

void TCKFFmpeg::start() {
    LOGE(" start");
    if (audio == NULL) {
        if (LOG_DEBUG) {
            LOGE("audio is null");
            return;
        }
    }

    audio->play();

    while (playstatus != NULL && !playstatus->exit) {
        AVPacket *avPacket = av_packet_alloc();

        if (av_read_frame(pFormatCtx, avPacket) == 0) {
            if (avPacket->stream_index == audio->streamIndex) {
                //解码操作
                audio->queue->putAvpacket(avPacket);
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            while (playstatus != NULL && !playstatus->exit) {
                if (audio->queue->getQueueSize() > 0) {
                    continue;
                } else {
                    playstatus->exit = true;
                    break;
                }
            }
        }
    }
    exit = true;
    if (LOG_DEBUG) {
        LOGD("解码完成");
    }

}

void TCKFFmpeg::pause() {
    if (audio != NULL) {
        audio->pause();
    }
}

void TCKFFmpeg::resume() {
    if (audio != NULL) {
        audio->resume();
    }
}



void TCKFFmpeg::release() {
    if (LOG_DEBUG) {
        LOGE("开始释放Ffmpe");
    }

    if (playstatus->exit) {
        return;
    }
    if (LOG_DEBUG) {
        LOGE("开始释放Ffmpe2");
    }
    playstatus->exit = true;

    pthread_mutex_lock(&init_mutex);
    int sleepCount = 0;
    while (!exit) {
        if (sleepCount > 1000) {
            exit = true;
        }
        if (LOG_DEBUG) {
            LOGE("wait ffmpeg  exit %d", sleepCount);
        }
        sleepCount++;
        av_usleep(1000 * 10);//暂停10毫秒
    }

    if (LOG_DEBUG) {
        LOGE("释放 Audio");
    }

    if (audio != NULL) {
        audio->release();
        delete (audio);
        audio = NULL;
    }

    if (LOG_DEBUG) {
        LOGE("释放 封装格式上下文");
    }
    if (pFormatCtx != NULL) {
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = NULL;
    }
    if (LOG_DEBUG) {
        LOGE("释放 callJava");
    }
    if (callJava != NULL) {
        callJava = NULL;
    }
    if (LOG_DEBUG) {
        LOGE("释放 playstatus");
    }
    if (playstatus != NULL) {
        playstatus = NULL;
    }
    pthread_mutex_unlock(&init_mutex);

}
