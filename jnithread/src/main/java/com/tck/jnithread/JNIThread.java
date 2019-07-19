package com.tck.jnithread;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/19 12:44</p>
 *
 * @author tck
 * @version 1.0
 */
public class JNIThread {


    static {
        System.loadLibrary("jnithread-lib");
    }


    public native void normalThread();

    public native void mutexThread();

    public native void stopMutexThread();

    public native void callbackFromC();


    private OnErrorListener onErrorListener;

    public void setOnErrorListener(OnErrorListener onErrorListener) {
        this.onErrorListener = onErrorListener;
    }


    public void onError(int code, String msg) {
        if (onErrorListener != null) {
            onErrorListener.onError(code, msg);
        }
    }


    public interface OnErrorListener {
        void onError(int code, String msg);
    }


}
