package com.example.myopencvndkapp;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.Image;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.util.Size;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.camera.core.Camera;
import androidx.camera.core.CameraSelector;
import androidx.camera.core.ImageAnalysis;
import androidx.camera.core.ImageProxy;
import androidx.camera.core.Preview;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.camera.view.PreviewView;
import androidx.core.content.ContextCompat;

import com.example.myopencvndkapp.util.ImgAnalysisUtil;
import com.google.common.util.concurrent.ListenableFuture;
import com.google.mlkit.common.MlKitException;
import com.google.mlkit.vision.common.InputImage;
import com.google.mlkit.vision.common.internal.ImageConvertUtils;

import java.nio.ByteBuffer;
import java.util.concurrent.Executors;

public class CheckFaceActivity extends AppCompatActivity {

    private static final String TAG = "CheckFaceActivity";
    private PreviewView mPreviewView;

    static {
        System.loadLibrary("myopencvndkapp");
    }

    private String path;
    private ImageView imageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_check_face);
        //去掉导航栏
        getSupportActionBar().hide();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            //透明状态栏
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            //透明导航栏
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
        }
        mPreviewView = findViewById(R.id.previewView);
        imageView = findViewById(R.id.imageView);
        requestPermissions(new String[]{Manifest.permission.CAMERA}, 200);
        path = ImgAnalysisUtil.getCascadeFilePath();
        if (path == null) {
            Toast.makeText(this, "path is null", Toast.LENGTH_SHORT).show();
            Log.i(TAG, "path is NULL");
        } else {
            loadCascade(path);
            Log.i(TAG, "path:" + path);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case 200:
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED)
                    startCameraPreview();
                break;
            default:
                Toast.makeText(this, "暂无相机权限", Toast.LENGTH_SHORT).show();
                break;
        }
    }

    ListenableFuture<ProcessCameraProvider> cameraProviderFuture;
    private long mCurrentTime;
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            if (msg.what == 200) {
                try {
                    Log.i(TAG, "currentThreadName2:==========> " + Thread.currentThread().getName());
                    ImageProxy imageProxy = (ImageProxy) msg.obj;
                    @SuppressLint("UnsafeExperimentalUsageError")
                    Image mediaImage = imageProxy.getImage();
                    InputImage image = InputImage.fromMediaImage(mediaImage, imageProxy.getImageInfo().getRotationDegrees());
                    Bitmap bitmap = ImageConvertUtils.getInstance().getUpRightBitmap(image);
                    //对bitmap人脸检测绘制方框
                    checkFace(bitmap);
                    imageView.setImageBitmap(bitmap);
                    imageProxy.close();
                } catch (MlKitException e) {
                    e.printStackTrace();
                }
            }
        }
    };

    /**
     * 开始相机预览
     */
    private void startCameraPreview() {
        cameraProviderFuture = ProcessCameraProvider.getInstance(this);
        cameraProviderFuture.addListener(() -> {
            Log.i(TAG, "开始回调listener事件");
            //用于将相机的生命周期绑定到生命周期所有者
            try {
                ProcessCameraProvider cameraProvider = cameraProviderFuture.get();
                //创建预览
                Preview preview = new Preview.Builder().build();
                //选择后置摄像头
                CameraSelector cameraSelector = new CameraSelector.Builder().requireLensFacing(CameraSelector.LENS_FACING_FRONT).build();
                //在重新绑定之前取消绑定
                cameraProvider.unbindAll();
                //将生命周期,选择摄像头,预览,绑定到相机ImageAnalysis
                ImageAnalysis imageAnalysis = new ImageAnalysis.Builder()
                        .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                        .build();

                imageAnalysis.setAnalyzer(Executors.newSingleThreadExecutor(), imageProxy -> {
                    //处理每一帧图片本地操作...
                    Log.i(TAG, "currentThreadName1:==========> " + Thread.currentThread().getName());
                    if (System.currentTimeMillis() - mCurrentTime > 20) {
                        Message message = new Message();
                        message.what = 200;
                        message.obj = imageProxy;
                        mHandler.sendMessage(message);
                        mCurrentTime = System.currentTimeMillis();
                    } else {
                        imageProxy.close();
                    }
                });
                Camera camera = cameraProvider.bindToLifecycle(CheckFaceActivity.this, cameraSelector, imageAnalysis, preview);
                //设置预览的View
                preview.setSurfaceProvider(mPreviewView.createSurfaceProvider(camera.getCameraInfo()));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }, ContextCompat.getMainExecutor(this));
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    @Override
    protected void onDestroy() {
        super.onDestroy();
        mPreviewView.releasePointerCapture();
    }

    public native void loadCascade(String filePath);

    public native void checkFace(Bitmap bitmap);
}