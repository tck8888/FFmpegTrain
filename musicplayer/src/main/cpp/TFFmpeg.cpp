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

void *threadPlay(void *context) {
    TFFmpeg * pFFmpeg = (TFFmpeg *) context;
    pFFmpeg->prepare();
    return 0;
}

void TFFmpeg::play() {
    pthread_t playThread;
    pthread_create(&playThread,NULL,threadPlay,this);
    pthread_detach(playThread);
}

void TFFmpeg::prepare() {
    av_register_all();
    avformat_network_init();

    int ret = avformat_open_input(&pFormatContext, url, NULL, NULL);

    if (ret != 0) {
        LOGE("avformat_open_input error");
        callPlayerJniError(ret, av_err2str(ret));
        return;
    }

    ret = avformat_find_stream_info(pFormatContext, NULL);

    if (ret < 0) {
        LOGE("avformat_find_stream_info error");
        callPlayerJniError(ret, av_err2str(ret));
        return;
    }

    int audioStreamIndex = av_find_best_stream(pFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

    if (audioStreamIndex < 0) {
        LOGE("av_find_best_stream error");
        callPlayerJniError(FIND_STREAM_ERROR_CODE, "av_find_best_stream error");
        return;
    }

    AVCodec *pCodec = avcodec_find_decoder(
            pFormatContext->streams[audioStreamIndex]->codecpar->codec_id);

    if (pCodec == NULL) {
        LOGE("avcodec_find_decoder error");
        callPlayerJniError(CODEC_FIND_DECODER_ERROR_CODE, "avcodec_find_decoder error");
        return;
    }

    //打开解码器
    pCodecContext = avcodec_alloc_context3(pCodec);

    if (pCodecContext == NULL) {
        LOGE("avcodec_alloc_context3 error");
        callPlayerJniError(CODEC_ALLOC_CONTEXT_ERROR_CODE, "avcodec_alloc_context3 error");
        return;
    }

    ret = avcodec_parameters_to_context(pCodecContext,
                                        pFormatContext->streams[audioStreamIndex]->codecpar);

    if (ret < 0) {
        LOGE("avcodec_parameters_to_context error");
        callPlayerJniError(ret, av_err2str(ret));
        return;
    }

    ret = avcodec_open2(pCodecContext, pCodec, NULL);
    if (ret != 0) {
        LOGE("avcodec_open2 error");
        callPlayerJniError(ret, av_err2str(ret));
        return;
    }

    //重采样

    swrContext = swr_alloc_set_opts(
            NULL,
            AV_CH_LAYOUT_STEREO,
            AV_SAMPLE_FMT_S16,
            AUDIO_SAMPLE_RATE,
            pCodecContext->channel_layout,
            pCodecContext->sample_fmt,
            pCodecContext->sample_rate,
            0,
            NULL
    );

    if (swrContext == NULL) {
        LOGE("swr_alloc_set_opts error");
        callPlayerJniError(SWR_ALLOC_SET_OPTS_ERROR_CODE, "swr_alloc_set_opts error");
        return;
    }


    ret = swr_init(swrContext);

    if (ret < 0) {
        LOGE("swr_init error");
        callPlayerJniError(SWR_CONTEXT_INIT_ERROR_CODE, "swr_init error");
        return;
    }


    AVPacket *pPacket = av_packet_alloc();
    AVFrame *pFrame = av_frame_alloc();

    int index = 0;

    while (av_read_frame(pFormatContext, pPacket) >= 0) {
        if (pPacket->stream_index == audioStreamIndex) {
            int codecSendPacketRes = avcodec_send_packet(pCodecContext, pPacket);
            if (codecSendPacketRes == 0) {
                int codecReceiveFrameRes = avcodec_receive_frame(pCodecContext, pFrame);
                if (codecReceiveFrameRes == 0) {
                    index++;
                    LOGE("解码第 %d 帧", index);

                    //swr_convert(swrContext,)
                }
            }
        }

        //解引用
        av_packet_unref(pPacket);
        av_frame_unref(pFrame);
    }

    av_packet_free(&pPacket);
    av_frame_free(&pFrame);

}

void TFFmpeg::prepareAsync() {


}

void TFFmpeg::callPlayerJniError(int code, char *msg) {

    release();

    callJava->callPlayerError(code, msg);

}

void TFFmpeg::release() {
    if (pCodecContext != NULL) {
        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
        pCodecContext = NULL;
    }

    if (pFormatContext != NULL) {
        avformat_close_input(&pFormatContext);
        avformat_free_context(pFormatContext);
        pFormatContext = NULL;
    }

    if (swrContext != NULL) {
        swr_free(&swrContext);
        free(swrContext);
        swrContext = NULL;
    }

    //TODO:

    avformat_network_deinit();
}


