package com.tck.openslaudio;

public class AudioPlayer {

    static {
        System.loadLibrary("openslaudio-lib");
    }

    public native static void playPCM(String url);
}
