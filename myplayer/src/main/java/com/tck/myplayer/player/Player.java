package com.tck.myplayer.player;

import android.text.TextUtils;

import com.tck.myplayer.listener.OnCompleteListener;
import com.tck.myplayer.listener.OnErrorListener;
import com.tck.myplayer.listener.OnLoadListener;
import com.tck.myplayer.listener.OnPauseResumeListener;
import com.tck.myplayer.listener.OnPreparedListener;
import com.tck.myplayer.listener.OnTimeInfoListener;
import com.tck.myplayer.log.MyLog;
import com.tck.myplayer.opengl.TCKGLSurfaceView;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/19 15:44</p>
 *
 * @author tck
 * @version 1.0
 */
public class Player {

    static {
        System.loadLibrary("videoplayer-lib");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }

    /**
     * 数据源
     */
    private static String source;
    private static TimeInfoBean timeInfoBean;
    private static boolean playNext = false;

    private OnPreparedListener onPreparedListener;
    private OnLoadListener onLoadListener;
    private OnPauseResumeListener onPauseResumeListener;
    private OnTimeInfoListener onTimeInfoListener;
    private OnErrorListener onErrorListener;
    private OnCompleteListener onCompleteListener;

    private TCKGLSurfaceView surfaceView;

    public Player() {
    }

    /**
     * 设置数据源
     *
     * @param source
     */
    public void setSource(String source) {
        this.source = source;
    }

    public void prepared() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("source not be empty");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                nativePrepared(source);
            }
        }).start();

    }

    public void setOnPreparedListener(OnPreparedListener onPreparedListener) {
        this.onPreparedListener = onPreparedListener;
    }

    public void setOnLoadListener(OnLoadListener onLoadListener) {
        this.onLoadListener = onLoadListener;
    }

    public void setOnPauseResumeListener(OnPauseResumeListener onPauseResumeListener) {
        this.onPauseResumeListener = onPauseResumeListener;

    }

    public void setOnTimeInfoListener(OnTimeInfoListener onTimeInfoListener) {
        this.onTimeInfoListener = onTimeInfoListener;
    }

    public void setOnErrorListener(OnErrorListener onErrorListener) {
        this.onErrorListener = onErrorListener;
    }

    public void setOnCompleteListener(OnCompleteListener onCompleteListener) {
        this.onCompleteListener = onCompleteListener;
    }

    public void setSurfaceView(TCKGLSurfaceView surfaceView) {
        this.surfaceView = surfaceView;
    }

    public void start() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("source is empty");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                nativeStart();
            }
        }).start();
    }

    public void pause() {
        nativePause();
        if (onPauseResumeListener != null) {
            onPauseResumeListener.onPause(true);
        }
    }

    public void resume() {
        nativeResume();
        if (onPauseResumeListener != null) {
            onPauseResumeListener.onPause(false);
        }
    }


    public void stop() {
        timeInfoBean = null;
        new Thread(new Runnable() {
            @Override
            public void run() {
                nativeStop();
            }
        }).start();
    }

    public void seek(int secds) {
        nativeSeek(secds);
    }

    public void playNext(String url) {
        source = url;
        playNext = true;
        stop();
    }

    public void onCallTimeInfo(int currentTime, int totalTime) {
        if (onTimeInfoListener != null) {
            if (timeInfoBean == null) {
                timeInfoBean = new TimeInfoBean();
            }
            timeInfoBean.setCurrentTime(currentTime);
            timeInfoBean.setTotalTime(totalTime);
            onTimeInfoListener.onTimeInfo(timeInfoBean);
        }
    }


    public void onCallLoad(boolean load) {
        if (onLoadListener != null) {
            onLoadListener.onLoad(load);
        }
    }


    /**
     * c++回调java的方法
     */
    public void onCallPrepared() {
        if (onPreparedListener != null) {
            onPreparedListener.onPrepared();
        }
    }

    public void onCallError(int code, String msg) {
        if (onErrorListener != null) {
            stop();
            onErrorListener.onError(code, msg);
        }
    }

    public void onCallComplete() {
        if (onCompleteListener != null) {
            onCompleteListener.onComplete();
        }
    }

    public void onCallNext() {
        if (playNext) {
            playNext = false;
            prepared();
        }
    }

    public void onCallRenderYUV(int width,
                                int height,
                                byte[] y,
                                byte[] u,
                                byte[] v) {
        MyLog.d("获取到视频的数据width=" + width + " height= " + height);
        if (surfaceView != null) {
            surfaceView.setYUVData(width, height, y, u, v);
        }
    }

    private native void nativePrepared(String source);

    private native void nativeStart();

    private native void nativeResume();

    private native void nativePause();

    private native void nativeStop();

    private native void nativeSeek(int secds);


}
