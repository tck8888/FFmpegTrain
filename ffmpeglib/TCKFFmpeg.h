//
// Created by tck88 on 2019/6/29.
//

#ifndef PLAYER_DNFFMPEG_H
#define PLAYER_DNFFMPEG_H

#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

extern  "C" {
#include <libavformat/avformat.h>
}


class TCKFFmpeg {
public:
    TCKFFmpeg(JavaCallHelper* callHelper,const char* dataSource);
    ~TCKFFmpeg();

    void prepare();
    void _prepare();

    void start();
    void _start();

    void setRenderFrameCallback(RenderFrameCallback callback);
private:
    char *dataSource;
    pthread_t pid;
    pthread_t pid_play;
    AVFormatContext *formatContext = 0;
    JavaCallHelper* callHelper;
    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;
    RenderFrameCallback callback;
    bool isPlaying;
};


#endif //PLAYER_DNFFMPEG_H