package com.tck.ffmpegtrain;

import android.media.MediaPlayer;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class TPlayer implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("native-lib");
    }

    private String dataSource;
    private SurfaceHolder holder;
    private onPrepareListener onPrepareListener;

    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }

    public void setSurfaceView(SurfaceView surfaceView) {
        holder = surfaceView.getHolder();
        holder.addCallback(this);
    }

    public void prepare() {
        native_prepare(dataSource);
    }

    public void start() {
        native_start();
    }

    public void stop() {

    }

    public void onError(int errorCode) {
        System.out.println("Java接到回调:"+errorCode);
    }

    public void onPrepare() {
        if (onPrepareListener != null) {
            onPrepareListener.onPrepare();
        }
    }

    public void setOnPrepareListener(TPlayer.onPrepareListener onPrepareListener) {
        this.onPrepareListener = onPrepareListener;
    }

    public interface onPrepareListener {
        void onPrepare();
    }

    public void release() {
        holder.removeCallback(this);
    }


    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {

    }

    /**
     * 画布发生了变化（横竖屏切换、按了home都会回调这个函数）
     *
     * @param surfaceHolder
     * @param i
     * @param i1
     * @param i2
     */
    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        native_setSurface(holder.getSurface());
    }

    /**
     * 销毁画布 (按了home/退出应用/)
     *
     * @param surfaceHolder
     */
    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }

    native void native_prepare(String dataSource);
    native void native_start();
    native void native_setSurface(Surface surface);
}
