package com.example.myopencvndkapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("myopencvndkapp");
    }

    private Bitmap bitmap;
    private ImageView imageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.button).setOnClickListener(this);
        findViewById(R.id.button1).setOnClickListener(this);
        findViewById(R.id.button2).setOnClickListener(this);
        findViewById(R.id.button3).setOnClickListener(this);
        findViewById(R.id.button4).setOnClickListener(this);
        findViewById(R.id.button5).setOnClickListener(this);
        findViewById(R.id.button6).setOnClickListener(this);
        imageView = findViewById(R.id.imageView);
        //获取位图
        bitmap = BitmapFactory.decodeResource(this.getResources(), R.mipmap.ic);
        imageView.setImageBitmap(bitmap);
    }

    public native void grayBitmap(Bitmap bitmap);

    public native void erZhiBitmap(Bitmap bitmap);

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.button:
                grayBitmap(bitmap);
                imageView.setImageBitmap(bitmap);
                break;
            case R.id.button1:
                erZhiBitmap(bitmap);
                imageView.setImageBitmap(bitmap);
                break;
            case R.id.button2:
                startActivity(new Intent(this, ImgAnliseActivity.class));
                break;
            case R.id.button3:
                startActivity(new Intent(this, CheckFaceActivity.class));
                break;
            case R.id.button4:
                startActivity(new Intent(this, BlurActivity.class));
                break;
            case R.id.button5:
                //形态学
                startActivity(new Intent(this, MorphologyActivity.class));
                break;
            case R.id.button6:
                startActivity(new Intent(this, ThresholdActivity.class));
                break;
        }
    }
}