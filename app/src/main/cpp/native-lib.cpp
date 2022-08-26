#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
using namespace cv;

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_example_myopencvndkapp_MainActivity_getGrayImage(JNIEnv *env, jobject instance, jintArray _pixels, jint width, jint height) {
    jint *pixels = env->GetIntArrayElements(_pixels , NULL);
    if(pixels==NULL){
        return NULL;
    }
    Mat imgData(height , width , CV_8UC4 , pixels);
    uchar *ptr = imgData.ptr(0);
    for (int i = 0; i < width * height; i++) {
        int grayScale = (int) (ptr[4 * i + 2] * 0.299+ ptr[4 * i + 1] * 0.587  + ptr[4 * i + 0] * 0.114);
        ptr[4 * i + 1] = (uchar) grayScale;
        ptr[4 * i + 2] = (uchar) grayScale;
        ptr[4 * i + 0] = (uchar) grayScale;
    }
    int size = width * height;
    jintArray result = env->NewIntArray(size);
    env->SetIntArrayRegion(result, 0, size, pixels);
    env->ReleaseIntArrayElements(_pixels, pixels, 0);
    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_MainActivity_grayBitmap(JNIEnv *env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void *pixels;
    AndroidBitmap_getInfo(env, bitmap, &info);
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat src(info.height, info.width, CV_8UC4, pixels);
        Mat gray;
        cvtColor(src, gray, COLOR_BGRA2GRAY);
        cvtColor(gray, src, COLOR_GRAY2RGBA);
        circle(src, Point(info.width / 2, info.height / 2), info.height / 6, Scalar(255, 0, 0), 10);
        GaussianBlur(src, src, Size(25, 25), 0);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_MainActivity_erZhiBitmap(JNIEnv *env, jobject thiz,
                                                         jobject bitmap) {
    AndroidBitmapInfo info;
    void *pixels;
    AndroidBitmap_getInfo(env, bitmap, &info);
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat src(info.height, info.width, CV_8UC4, pixels);

        //灰度处理
        Mat gray(info.height, info.width, CV_8UC1);
        cvtColor(src, gray, COLOR_BGRA2GRAY);

        //二值化处理
        threshold(gray, gray, 150, 255, THRESH_BINARY);//#自定义，把平均值高于的都截断，（mean=127,127的值都变成255，127以下保持不变）

        cvtColor(gray, src, COLOR_GRAY2BGRA);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}