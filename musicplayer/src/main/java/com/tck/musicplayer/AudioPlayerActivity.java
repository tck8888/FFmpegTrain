package com.tck.musicplayer;

import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class AudioPlayerActivity extends AppCompatActivity {


    private Button btnStart;
    private AudioPlayer audioPlayer;

    private String audioPath= Environment.getExternalStorageDirectory().getAbsolutePath()+"/不仅仅是喜欢.mp3";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio_play);

        audioPlayer = new AudioPlayer();

        btnStart = (Button) findViewById(R.id.btn_start);

        audioPlayer.setDataSource(audioPath);


        btnStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                audioPlayer.play();
            }
        });
    }

}
