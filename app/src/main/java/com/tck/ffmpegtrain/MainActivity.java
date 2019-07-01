package com.tck.ffmpegtrain;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {


    private TPlayer tPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SurfaceView surfaceView = findViewById(R.id.surfaceView);

        tPlayer = new TPlayer();
        tPlayer.setSurfaceView(surfaceView);
        tPlayer.setDataSource("http://vod.hkstv.tv/vod/data/userdata/vod/transcode/18bfb478c8013e65fc34ab5e9b0264f1/18bfb478c8013e65fc34ab5e9b0264f11/18bfb478c8013e65fc34ab5e9b0264f12/18bfb478c8013e65fc34ab5e9b0264f1.m3u8");

        tPlayer.setOnPrepareListener(new TPlayer.onPrepareListener() {
            @Override
            public void onPrepare() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, "准备好了", Toast.LENGTH_SHORT).show();
                        System.out.println("=====================准备好了");
                    }
                });
                tPlayer.start();
            }
        });

        findViewById(R.id.btn_play).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tPlayer.prepare();
            }
        });
    }


}
