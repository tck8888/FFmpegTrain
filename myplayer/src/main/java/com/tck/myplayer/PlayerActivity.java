package com.tck.myplayer;

import android.os.Bundle;
import android.view.SurfaceView;
import android.widget.Button;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.tck.myplayer.listener.OnPreparedListener;
import com.tck.myplayer.log.MyLog;
import com.tck.myplayer.player.Player;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/19 15:33</p>
 *
 * @author tck
 * @version 1.0
 */
public class PlayerActivity extends AppCompatActivity {

    private SurfaceView surfaceView;

    private String videoUrl = "https://display-work-video.oss-cn-hangzhou.aliyuncs.com/105201.mp4";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);
        surfaceView = (SurfaceView) findViewById(R.id.surface_view);

        Player player = new Player();
        player.setSource(videoUrl);
        player.prepared();
        player.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared() {
                MyLog.d("java收到native Prepared成功");
            }
        });
    }
}
