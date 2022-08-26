package com.example.myopencvndkapp;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import com.example.myopencvndkapp.util.ImgAnalysisUtil;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class ImgAnliseActivity extends AppCompatActivity {

    private static final String TAG = "ImgAnliseActivity";

    static {
        System.loadLibrary("myopencvndkapp");
    }

    private ImageView iv;
    private Bitmap bitmap;
    private String path;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_img_anlise);
        iv = findViewById(R.id.iv);
        bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.p).copy(Bitmap.Config.ARGB_8888, true);
        path = ImgAnalysisUtil.getCascadeFilePath();
        if (path == null) {
            Toast.makeText(this, "path is null", Toast.LENGTH_SHORT).show();
            Log.i(TAG, "path is NULL");
        } else {
            loadCascade(path);
            Log.i(TAG, "path:" + path);
        }
    }

    public void click(View view) {
        if (path == null) {
            Toast.makeText(this, "path is null", Toast.LENGTH_SHORT).show();
            return;
        }
        checkFace(bitmap);
        iv.setImageBitmap(bitmap);
    }

    public native void checkFace(Bitmap bitmap);

    public native void loadCascade(String filePath);

}