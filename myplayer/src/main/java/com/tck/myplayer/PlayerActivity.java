package com.tck.myplayer;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.tck.myplayer.listener.OnCompleteListener;
import com.tck.myplayer.listener.OnErrorListener;
import com.tck.myplayer.listener.OnLoadListener;
import com.tck.myplayer.listener.OnPauseResumeListener;
import com.tck.myplayer.listener.OnPreparedListener;
import com.tck.myplayer.listener.OnTimeInfoListener;
import com.tck.myplayer.log.MyLog;
import com.tck.myplayer.player.Player;
import com.tck.myplayer.player.TimeInfoBean;
import com.tck.myplayer.util.TimeUtil;

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
    private Button btnStart;
    private Button btnPause;
    private Button btnResume;
    private Button btnStop;
    private Button btnNext;
    private SeekBar seekBar;
    private TextView tvTime;


    // private String videoUrl = "https://display-work-video.oss-cn-hangzhou.aliyuncs.com/105201.mp4";
    private String videoUrl = "http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3";
    private Player player;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);

        surfaceView = (SurfaceView) findViewById(R.id.surface_view);
        btnStart = (Button) findViewById(R.id.btn_start);
        btnPause = (Button) findViewById(R.id.btn_pause);
        btnResume = (Button) findViewById(R.id.btn_resume);
        btnStop = (Button) findViewById(R.id.btn_stop);
        btnNext = (Button) findViewById(R.id.btn_next);
        seekBar = (SeekBar) findViewById(R.id.seek_bar);
        tvTime = (TextView) findViewById(R.id.tv_time);


        player = new Player();

        player.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared() {
                MyLog.d("准备好了，可以开始播放音乐");
                player.start();
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


        player.setOnTimeInfoListener(new OnTimeInfoListener() {
            @Override
            public void onTimeInfo(TimeInfoBean timeInfoBean) {
                Message message = Message.obtain();
                message.what = 1;
                message.obj = timeInfoBean;
                handler.sendMessage(message);
            }
        });

        player.setOnErrorListener(new OnErrorListener() {
            @Override
            public void onError(int code, String msg) {
                MyLog.e("code= " + code + " msg= " + msg);
            }
        });

        player.setOnCompleteListener(new OnCompleteListener() {
            @Override
            public void onComplete() {
                MyLog.d("播放完成");
            }
        });

        btnStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                player.setSource(videoUrl);
                player.prepared();
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
        btnStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                player.stop();
            }
        });
        seekBar.setMax(219);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                player.seek(progress);
            }
        });

        btnNext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                player.playNext("http://ngcdn004.cnr.cn/live/dszs/index.m3u8");
            }
        });
    }

    Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == 1) {
                TimeInfoBean timeInfoBean = (TimeInfoBean) msg.obj;
                tvTime.setText(TimeUtil.secdsToDateFormat(timeInfoBean.getTotalTime(), timeInfoBean.getTotalTime())
                        + "/" + TimeUtil.secdsToDateFormat(timeInfoBean.getCurrentTime(), timeInfoBean.getTotalTime()));
            }
        }
    };

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
