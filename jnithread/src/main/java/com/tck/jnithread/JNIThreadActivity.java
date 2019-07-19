package com.tck.jnithread;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/19 12:42</p>
 *
 * @author tck
 * @version 1.0
 */
public class JNIThreadActivity extends AppCompatActivity {

    private Button btnCreateNormalThread;
    private JNIThread jniThread;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_jni_thread);


        btnCreateNormalThread = (Button) findViewById(R.id.btn_create_normal_thread);


        jniThread = new JNIThread();

        btnCreateNormalThread.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                jniThread.normalThread();
            }
        });
    }
}
