package com.tck.myplayer.log;

import android.util.Log;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/19 15:49</p>
 *
 * @author tck
 * @version 1.0
 */
public class MyLog {

    private static final String TAG = "tck6666";

    public static void d(String msg) {
        Log.d(TAG, msg);
    }
    public static void e(String msg) {
        Log.e(TAG, msg);
    }
}
