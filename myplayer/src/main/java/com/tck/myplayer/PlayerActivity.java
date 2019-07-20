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

import com.tck.myplayer.listener.OnLoadListener;
import com.tck.myplayer.listener.OnPauseResumeListener;
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
    private Button btnResume;
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
        btnResume = (Button) findViewById(R.id.btn_resume);

        player = new Player();

        player.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared() {
                MyLog.d("准备好了，可以开始播放音乐");
            }
        });
        player.setOnLoadListener(new OnLoadListener() {
            @Override
            public void onLoad(boolean load) {
                if (load) {
                    MyLog.d("加载中");
                } else {
                    MyLog.d("播放中");
                }
            }
        });

        btnPrepare.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                player.setSource(videoUrl);
                player.prepared();
            }
        });

        player.setOnPauseResumeListener(new OnPauseResumeListener() {
            @Override
            public void onPause(boolean pause) {
                if (pause) {
                    MyLog.d("暂停");
                } else {
                    MyLog.d("恢复播放");
                }
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
                player.pause();
            }
        });
        btnResume.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                player.resume();
            }
        });
    }

    private void startPlayWithPermissions() {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED &&
                ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
                        != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE},
                    100);
        }
    }
}
