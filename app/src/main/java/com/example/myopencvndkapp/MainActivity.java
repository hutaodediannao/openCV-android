package com.example.myopencvndkapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.myopencvndkapp.featureDetection.CannyActivity;
import com.example.myopencvndkapp.featureDetection.HoughCirclesActivity;
import com.example.myopencvndkapp.featureDetection.HoughLinesActivity;
import com.example.myopencvndkapp.featureDetection.LaplacianActivity;
import com.example.myopencvndkapp.featureDetection.TiDuActivity;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("myopencvndkapp");
    }

    private Bitmap bitmap;
    private ImageView imageView;
    private View item0, item1, item2;
    private TextView tv0, tv1, tv2;
    private Button mButton7;
    private Button mButton8;
    private Button mButton9;
    private Button mButton10;
    private Button mButton11;
    private Button mButton12;
    private Button mButton13;
    private Button mButton14;
    private Button mButton15;

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
        findViewById(R.id.button7).setOnClickListener(this);
        findViewById(R.id.button8).setOnClickListener(this);
        findViewById(R.id.button9).setOnClickListener(this);
        findViewById(R.id.button10).setOnClickListener(this);
        findViewById(R.id.button11).setOnClickListener(this);
        findViewById(R.id.button12).setOnClickListener(this);
        findViewById(R.id.button13).setOnClickListener(this);
        findViewById(R.id.button14).setOnClickListener(this);
        findViewById(R.id.button15).setOnClickListener(this);

        tv0 = findViewById(R.id.tv0);
        tv1 = findViewById(R.id.tv1);
        tv2 = findViewById(R.id.tv2);

        item0 = findViewById(R.id.item0);
        item0.setVisibility(View.GONE);
        item2 = findViewById(R.id.item2);

        item1 = findViewById(R.id.item1);
        item1.setVisibility(View.GONE);
        item2.setVisibility(View.GONE);

        imageView = findViewById(R.id.imageView);
        //获取位图
        bitmap = BitmapFactory.decodeResource(this.getResources(), R.mipmap.ic);
        imageView.setImageBitmap(bitmap);

        mButton7 = findViewById(R.id.button7);
        mButton8 = findViewById(R.id.button8);
        mButton9 = findViewById(R.id.button9);
        mButton10 = findViewById(R.id.button10);
        mButton11 = findViewById(R.id.button11);
        mButton12 = findViewById(R.id.button12);
        mButton13 = findViewById(R.id.button13);
        mButton14 = findViewById(R.id.button14);
        mButton15 = findViewById(R.id.button15);

        tv0.setOnClickListener(this);
        tv1.setOnClickListener(this);
        tv2.setOnClickListener(this);
    }

    public native void grayBitmap(Bitmap bitmap);

    public native void erZhiBitmap(Bitmap bitmap);

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.tv0:
                setRightTvIcon(item0, tv0);
                break;
            case R.id.tv1:
                setRightTvIcon(item1, tv1);
                break;
            case R.id.tv2:
                setRightTvIcon(item2, tv2);
                break;
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
                //阈值化
                startActivity(new Intent(this, ThresholdActivity.class));
                break;
            //基本特征检测
            case R.id.button7:
                //梯度计算
                startActivity(new Intent(this, TiDuActivity.class));
                break;
            case R.id.button8:
                //拉普拉斯算子
                startActivity(new Intent(this, LaplacianActivity.class));
                break;
            case R.id.button9:
                //边缘检测
                startActivity(new Intent(this, CannyActivity.class));
                break;
            case R.id.button10:
                //霍夫直线检测
                startActivity(new Intent(this, HoughLinesActivity.class));
                break;
            case R.id.button11:
                //霍夫圆检测
                startActivity(new Intent(this, HoughCirclesActivity.class));
                break;
            case R.id.button12:
                break;
            case R.id.button13:
                break;
            case R.id.button14:
                break;
            case R.id.button15:
                break;
            default:
                break;
        }
    }

    private void setRightTvIcon(View item0, TextView tv0) {
        item0.setVisibility(item0.getVisibility() == View.VISIBLE ? View.GONE : View.VISIBLE);
        Drawable rightDrawable = getResources().getDrawable(item0.getVisibility() == View.VISIBLE ? R.drawable.ic_baseline_arrow_drop_down_24 : R.drawable.ic_baseline_arrow_right_24);
        rightDrawable.setBounds(0, 0, rightDrawable.getMinimumWidth(), rightDrawable.getMinimumHeight());
        tv0.setCompoundDrawables(null, null, rightDrawable, null);
    }
}