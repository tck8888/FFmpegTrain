package com.tck.jnithread;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

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
    private Button btnCProduceConsume;
    private Button btnStopCProduceConsume;
    private Button btnCallBackC;
    private JNIThread jniThread;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_jni_thread);

        btnCreateNormalThread = (Button) findViewById(R.id.btn_create_normal_thread);
        btnCProduceConsume = (Button) findViewById(R.id.btn_c_produce_consume);
        btnStopCProduceConsume = (Button) findViewById(R.id.btn_stop_c_produce_consume);
        btnCallBackC = (Button) findViewById(R.id.btn_call_back_c);


        jniThread = new JNIThread();

        btnCreateNormalThread.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                jniThread.normalThread();
            }
        });
        btnCProduceConsume.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                jniThread.mutexThread();
            }
        });
        btnStopCProduceConsume.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                jniThread.stopMutexThread();
            }
        });

        jniThread.setOnErrorListener(new JNIThread.OnErrorListener() {
            @Override
            public void onError(final int code, final String msg) {
                Log.d("tck6666", "code=" + code + " msg=" + msg);
            }
        });

        btnCallBackC.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                jniThread.callbackFromC();
            }
        });
    }
}
