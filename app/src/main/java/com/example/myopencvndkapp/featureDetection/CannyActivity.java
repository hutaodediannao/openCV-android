package com.example.myopencvndkapp.featureDetection;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.myopencvndkapp.R;

/**
 * 作者:胡涛
 * 日期:2022-8-30
 * 时间:0:34
 * 功能:边缘检测Canny
 */
public class CannyActivity extends AppCompatActivity {
    static {
        System.loadLibrary("myopencvndkapp");
    }

    private ImageView iv;
    private Bitmap bitmap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_laplacian);
        iv = findViewById(R.id.iv);
        bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic);
    }

    public void click(View view) {
        switch (view.getId()) {
            case R.id.button:
                canny(bitmap);
                break;
            default:
                break;
        }
        iv.setImageBitmap(bitmap);
    }

    private native void canny(Bitmap bitmap);
}