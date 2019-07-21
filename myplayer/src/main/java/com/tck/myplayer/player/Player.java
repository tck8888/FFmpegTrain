package com.tck.myplayer.player;

import android.text.TextUtils;

import com.tck.myplayer.listener.OnLoadListener;
import com.tck.myplayer.listener.OnPauseResumeListener;
import com.tck.myplayer.listener.OnPreparedListener;
import com.tck.myplayer.listener.OnTimeInfoListener;
import com.tck.myplayer.log.MyLog;

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
    private String source;
    private static TimeInfoBean timeInfoBean;

    private OnPreparedListener onPreparedListener;
    private OnLoadListener onLoadListener;
    private OnPauseResumeListener onPauseResumeListener;
    private OnTimeInfoListener onTimeInfoListener;

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
        onCallLoad(true);
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
        new Thread(new Runnable() {
            @Override
            public void run() {
                nativeStop();
            }
        }).start();
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


    private native void nativePrepared(String source);

    private native void nativeStart();

    private native void nativeResume();

    private native void nativePause();

    private native void nativeStop();


}
