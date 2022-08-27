package com.example.myopencvndkapp;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

public class BlurActivity extends AppCompatActivity {

    private ImageView iv;
    private Bitmap bitmap;

    static {
        System.loadLibrary("myopencvndkapp");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_blur);
        Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.mipmap.ic);
        bitmap = bmp.copy(Bitmap.Config.ARGB_8888, true);

        iv = findViewById(R.id.iv);
    }

    public void click(View view) {
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
            case R.id.button4:
                //高斯双边滤波
                bilateralFilter(bitmap);
                break;
            case R.id.button5:
                //均值迁移滤波
                pyrMeanShiftFiltering(bitmap);
                break;
            case R.id.button6:
                //模糊操作
                mohu(bitmap);
                break;
            case R.id.button7:
                //锐化

                break;
            case R.id.button8:
                //梯度

                break;
            default:
                break;
        }
        iv.setImageBitmap(bitmap);
    }

    private native void mohu(Bitmap bitmap);

    private native void pyrMeanShiftFiltering(Bitmap bitmap);

    private native void bilateralFilter(Bitmap bitmap);

    private native void gune(Bitmap bitmap);

    private native void erode(Bitmap bitmap);

    private native void dilate(Bitmap bitmap);

    public native void medianBlur(Bitmap bitmap);
}