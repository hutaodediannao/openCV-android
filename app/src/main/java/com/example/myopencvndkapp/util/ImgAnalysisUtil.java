package com.example.myopencvndkapp.util;

import android.annotation.SuppressLint;
import android.content.Context;

import com.example.myopencvndkapp.R;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class ImgAnalysisUtil {

    private Context mContext;
    private static ImgAnalysisUtil instance;
    private String mCascadeFilePath;

    private ImgAnalysisUtil(Context context) {
        this.mContext = context;
        mCascadeFilePath = initCascadeFile();
    }

    /**
     * application中初始化一次
     * @param context
     * @return
     */
    public static ImgAnalysisUtil init(Context context) {
        if (instance == null) {
            synchronized (ImgAnalysisUtil.class) {
                if (instance == null) {
                    instance = new ImgAnalysisUtil(context);
                }
            }
        }
        return instance;
    }

    private String initCascadeFile() {
        try {
            // load cascade file from application resources
            @SuppressLint("ResourceType")
            InputStream inputStream = mContext.getResources().openRawResource(R.raw.lbpcascade_frontalface);
            File cascadeDir = mContext.getDir("cascade", Context.MODE_PRIVATE);
            File cascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
            FileOutputStream os = new FileOutputStream(cascadeFile);
            byte[] buffer = new byte[4096];
            int bytesRead = inputStream.read(buffer);
            while (bytesRead != -1) {
                os.write(buffer, 0, bytesRead);
                bytesRead = inputStream.read(buffer);
            }
            inputStream.close();
            os.close();
            return cascadeFile.getAbsolutePath();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * 获取证书路径
     *
     * @return
     */
    public static String getCascadeFilePath() {
        return instance.mCascadeFilePath;
    }
}
