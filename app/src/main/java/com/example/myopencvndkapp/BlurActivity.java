package com.example.myopencvndkapp;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

public class BlurActivity extends AppCompatActivity implements View.OnClickListener {

    private ImageView iv;
    private Bitmap bitmap;

    static {
        System.loadLibrary("myopencvndkapp");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_blur);

        iv = findViewById(R.id.iv);
        findViewById(R.id.button).setOnClickListener(this);
        findViewById(R.id.button1).setOnClickListener(this);
        findViewById(R.id.button2).setOnClickListener(this);
        findViewById(R.id.button3).setOnClickListener(this);

        Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.mipmap.ic);
        bitmap = bmp.copy(Bitmap.Config.ARGB_8888, true);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.button:
                //中值滤波
                medianBlur(bitmap);
                break;
            case R.id.button1:
                //最大值滤波
                dilate(bitmap);
                break;
            case R.id.button2:
                //最小值滤波
                erode(bitmap);
                break;
            case R.id.button3:
                //高斯滤波
                gune(bitmap);
                break;
            default:
                break;
        }
        iv.setImageBitmap(bitmap);
    }

    private native void gune(Bitmap bitmap);

    private native void erode(Bitmap bitmap);

    private native void dilate(Bitmap bitmap);

    public native void medianBlur(Bitmap bitmap);
}