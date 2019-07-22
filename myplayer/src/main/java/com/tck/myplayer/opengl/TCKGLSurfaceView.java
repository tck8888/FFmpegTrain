package com.tck.myplayer.opengl;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/22 8:20</p>
 *
 * @author tck
 * @version 1.0
 */
public class TCKGLSurfaceView extends GLSurfaceView {

    private TCKRender tckRender;

    public TCKGLSurfaceView(Context context) {
        this(context, null);
    }

    public TCKGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(2);
        tckRender = new TCKRender(context);
        setRenderer(tckRender);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    public void setYUVData(int width, int height, byte[] y, byte[] u, byte[] v) {
        if (tckRender != null) {
            tckRender.setYUVRenderData(width, height, y, u, v);
            requestRender();
        }
    }
}

