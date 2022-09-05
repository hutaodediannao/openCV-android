package com.example.myopencvndkapp.featureDetection;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.myopencvndkapp.R;

/**
 * 作者:胡涛
 * 日期:2022-8-30
 * 时间:0:34
 * 功能:拉普拉斯算子
 */
public class LaplacianActivity extends AppCompatActivity {
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

        iv.setImageBitmap(bitmap);
        Button btn = findViewById(R.id.button);
        btn.setText("拉普拉斯算子");
    }

    public void click(View view) {
        switch (view.getId()) {
            case R.id.button:
                laplacian(bitmap);
                break;
            default:
                break;
        }
        iv.setImageBitmap(bitmap);
    }

    private native void laplacian(Bitmap bitmap);
}