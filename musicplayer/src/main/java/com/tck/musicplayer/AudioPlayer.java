package com.tck.musicplayer;

import android.text.TextUtils;

public class AudioPlayer {

    static {
        System.loadLibrary("music-lib");
    }


    private String url;

    public void setDataSource(String url) {
        this.url = url;
    }


    public void play(){
        if (TextUtils.isEmpty(url)){
            throw  new NullPointerException("url is empty");
        }


        nativePlay(url);
    }


    private native void nativePlay(String url);

}
