package com.example.myopencvndkapp;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

/**
 * 作者:胡涛
 * 日期:2022-8-28
 * 时间:15:16
 * 功能:形态学操作
 */
public class MorphologyActivity extends AppCompatActivity {

    static {
        System.loadLibrary("myopencvndkapp");
    }

    private ImageView iv;
    private Bitmap bitmap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_morphology);

        iv = findViewById(R.id.iv);
        bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic);
    }

    public void click(View view) {
        switch (view.getId()) {
            case R.id.button:
                open(bitmap);
                break;
            case R.id.button1:
                close(bitmap);
                break;
            case R.id.button2:
                topHat(bitmap);
                break;
            case R.id.button3:
                blackHat(bitmap);
                break;
            default:
                break;
        }
        iv.setImageBitmap(bitmap);
    }

    private native void open(Bitmap bitmap);
    private native void close(Bitmap bitmap);
    private native void topHat(Bitmap bitmap);
    private native void blackHat(Bitmap bitmap);

}