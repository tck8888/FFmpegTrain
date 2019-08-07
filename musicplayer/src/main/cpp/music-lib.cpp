//
// Created by tck88 on 2019/7/20.
//
#include <jni.h>
#include <string>

#include "AndroidLog.h"
#include "TCallJava.h"
#include "TFFmpeg.h"

// 在 c++ 中采用 c 的这种编译方式
extern "C" {
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
}

jobject initCreateAudioTrack(JNIEnv *env) {
    jclass jAudioTrackClass = env->FindClass("android/media/AudioTrack");
    jmethodID jAudioTackCMid = env->GetMethodID(jAudioTrackClass, "<init>", "(IIIIII)V");

    int streamType = 3;
    int sampleRateInHz = AUDIO_SAMPLE_RATE;
    int channelConfig = (0x4 | 0x8);
    int audioFormat = 2;
    int mode = 1;

    jmethodID getMinBufferSizeMid = env->GetStaticMethodID(jAudioTrackClass, "getMinBufferSize",
                                                           "(III)I");
    int bufferSizeInBytes = env->CallStaticIntMethod(jAudioTrackClass, getMinBufferSizeMid,
                                                     sampleRateInHz, channelConfig, audioFormat);
    LOGE("bufferSizeInBytes = %d", bufferSizeInBytes);

    jobject jAudioTrackObj = env->NewObject(jAudioTrackClass, jAudioTackCMid, streamType,
                                            sampleRateInHz, channelConfig, audioFormat,
                                            bufferSizeInBytes, mode);

    // play
    jmethodID playMid = env->GetMethodID(jAudioTrackClass, "play", "()V");
    env->CallVoidMethod(jAudioTrackObj, playMid);

    return jAudioTrackObj;

}


TFFmpeg *tfFmpeg = NULL;
TCallJava *callJava = NULL;

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_musicplayer_AudioPlayer_nativePlay(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);


    callJava = new TCallJava(NULL,env,instance);

    tfFmpeg = new TFFmpeg(callJava,url);
    tfFmpeg->play();

    av_register_all();

    avformat_network_init();

    avcodec_register_all();

    AVFormatContext *ic = NULL;

    //打开文件
    int ret = avformat_open_input(&ic, url, NULL, NULL);

    if (ret != 0) {
        LOGE("avformat_open_input error")
        return;
    }

    //获取流信息
    ret = avformat_find_stream_info(ic, NULL);

    if (ret < 0) {
        LOGE("avformat_find_stream_info error")
        return;
    }

    int videoStream = 0;
    int audioStream = 1;
    int fps = 0;
    for (int i = 0; i < ic->nb_streams; ++i) {
        AVStream *as = ic->streams[i];

        if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            LOGE("视频数据")
        } else if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            LOGE("音频频数据")
            audioStream = i;
            LOGE("sample_rate=%d channels=%d sample_format=%d",
                 as->codecpar->sample_rate,
                 as->codecpar->channels,
                 as->codecpar->format
            );
        }
    }
    //获取音频流信息
    // audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

    //打开解码器
    //软解码
    AVCodec *codec = avcodec_find_decoder(ic->streams[audioStream]->codecpar->codec_id);
    if (codec == NULL) {
        LOGE("avcodec_find_decoder error")
        return;
    }

    //解码器初始化
    AVCodecContext *vc = avcodec_alloc_context3(codec);

    ret = avcodec_parameters_to_context(vc, ic->streams[audioStream]->codecpar);
    if (ret < 0) {
        LOGE("avcodec_parameters_to_context error")
        return;
    }

    ret = avcodec_open2(vc, codec, NULL);
    if (ret != 0) {
        LOGE("avcodec_open2 error")
        return;
    }

    LOGE("avcodec_open2 success")
    env->ReleaseStringUTFChars(url_, url);
}

