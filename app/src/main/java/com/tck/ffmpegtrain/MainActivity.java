package com.tck.ffmpegtrain;

import android.Manifest;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.tck.common.PermissionsUtils;
import com.tck.musicplayer.AudioPlayerActivity;

public class MainActivity extends AppCompatActivity {

    private Button btnAudio;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnAudio = (Button) findViewById(R.id.btn_audio);

        btnAudio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!PermissionsUtils.hasStoragePermissions(MainActivity.this)) {
                    ActivityCompat.requestPermissions(MainActivity.this,
                            new String[]{
                                    Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE},
                            100);
                } else {
                    open(AudioPlayerActivity.class);
                }
            }
        });

    }


    private void open(Class clazz) {
        if (clazz != null) {
            Intent intent = new Intent(this, clazz);
            startActivity(intent);
        }
    }
}
