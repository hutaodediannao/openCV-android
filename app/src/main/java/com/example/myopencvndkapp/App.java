package com.example.myopencvndkapp;

import android.app.Application;

import com.example.myopencvndkapp.util.ImgAnalysisUtil;

public class App extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        ImgAnalysisUtil.init(this);
    }
}
