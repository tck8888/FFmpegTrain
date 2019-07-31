package com.tck.musicplayer;

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

public class AudioPlayerActivity extends AppCompatActivity {

    private SurfaceView surfaceView;
    private Button btnPrepare;
    private Button btnStart;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio_play);
        surfaceView = (SurfaceView) findViewById(R.id.surface_view);
        btnPrepare = (Button) findViewById(R.id.btn_prepare);
        btnStart = (Button) findViewById(R.id.btn_start);
        btnPrepare.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startPlayWithPermissions();

            }
        });
        btnStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AudioPlayer.playPCM(Environment.getExternalStorageDirectory().getAbsolutePath() + "/mydream.pcm");
            }
        });
    }

    private void startPlayWithPermissions() {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this,new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},100);
        }
    }
}
