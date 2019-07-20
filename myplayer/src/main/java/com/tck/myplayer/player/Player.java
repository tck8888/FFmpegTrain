package com.tck.myplayer.player;

import android.text.TextUtils;

import com.tck.myplayer.listener.OnPreparedListener;
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

    private OnPreparedListener onPreparedListener;

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


    /**
     * c++回调java的方法
     */
    public void onCallPrepared() {
        if (onPreparedListener != null) {
            onPreparedListener.onPrepared();
        }
    }


    public native void nativePrepared(String source);

    public native void nativeStart();

}
