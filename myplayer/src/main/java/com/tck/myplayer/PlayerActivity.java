package com.tck.myplayer;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.tck.myplayer.listener.OnPreparedListener;
import com.tck.myplayer.log.MyLog;
import com.tck.myplayer.player.Player;

import java.io.File;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/19 15:33</p>
 *
 * @author tck
 * @version 1.0
 */
public class PlayerActivity extends AppCompatActivity {

    private SurfaceView surfaceView;
    private Button btnPrepare;
    private Button btnStart;
    private Button btnPause;
    // private String videoUrl = "https://display-work-video.oss-cn-hangzhou.aliyuncs.com/105201.mp4";
    private String videoUrl = "http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3";
    private Player player;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);
        surfaceView = (SurfaceView) findViewById(R.id.surface_view);
        btnPrepare = (Button) findViewById(R.id.btn_prepare);
        btnStart = (Button) findViewById(R.id.btn_start);
        btnPause = (Button) findViewById(R.id.btn_pause);

        player = new Player();

        player.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared() {
                MyLog.d("java收到native Prepared成功");
            }
        });


        btnPrepare.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                player.setSource(videoUrl);
                player.prepared();
            }
        });
        btnStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                player.start();
            }
        });
        btnPause.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startPlayWithPermissions();
            }
        });
    }

    private void startPlayWithPermissions() {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED&&
        ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.READ_EXTERNAL_STORAGE},
                    100);
        }
    }
}
