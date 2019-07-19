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


    public native  void normalThread();
}
