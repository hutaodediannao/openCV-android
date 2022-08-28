package com.example.myopencvndkapp;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

/**
 * 作者:胡涛
 * 日期:2022-8-28
 * 时间:15:16
 * 功能:阈值化与阈值操作
 */
public class ThresholdActivity extends AppCompatActivity {

    static {
        System.loadLibrary("myopencvndkapp");
    }

    private ImageView iv;
    private Bitmap bitmap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_threshod);

        iv = findViewById(R.id.iv);
        bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic);
    }

    public void click(View view) {
        switch (view.getId()) {
            case R.id.button:
                threshold(bitmap, 1);
                break;
            case R.id.button1:
                threshold(bitmap, 2);
                break;
            case R.id.button2:
                threshold(bitmap, 3);
                break;
            case R.id.button3:
                threshold(bitmap, 4);
                break;
            case R.id.button4:
                threshold(bitmap, 5);
                break;
            case R.id.button5:
                threshold(bitmap, 6);
                break;
            case R.id.button6:
                threshold(bitmap, 7);
                break;
            default:
                break;
        }
        iv.setImageBitmap(bitmap);
    }

    public native void threshold(Bitmap bitmap, int type);

}