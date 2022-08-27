#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include <string>
#include <android/log.h>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;

#ifndef LOG_TAG
#define LOG_TAG "HELLO_JNI"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG ,__VA_ARGS__) // 定义LOGF类型
#endif

CascadeClassifier classifier;

void mat2Bitmap(JNIEnv *env, Mat &mat, jobject &bitmap) {
    AndroidBitmapInfo info;
    void *pixels;
    AndroidBitmap_getInfo(env, bitmap, &info);
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        if (mat.type() == CV_8UC1) {
            cvtColor(mat, temp, COLOR_GRAY2BGRA);
        } else if (mat.type() == CV_8UC2) {
            cvtColor(mat, temp, COLOR_BGR5652BGRA);
        } else if (mat.type() == CV_8UC4) {
            mat.copyTo(temp);
        }
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        if (mat.type() == CV_8UC1) {
            cvtColor(mat, temp, COLOR_GRAY2BGR565);
        } else if (mat.type() == CV_8UC2) {
            mat.copyTo(temp);
        } else if (mat.type() == CV_8UC4) {
            cvtColor(mat, temp, COLOR_BGR5652BGRA);
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

void bitmap2Mat(JNIEnv *env, jobject &bitmap, Mat &mat) {
    AndroidBitmapInfo info;
    void *pixels;
    AndroidBitmap_getInfo(env, bitmap, &info);
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    mat.create(info.height, info.width, CV_8UC4);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        temp.copyTo(mat);
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        cvtColor(temp, mat, COLOR_BGR5652BGRA);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_ImgAnliseActivity_checkFace(JNIEnv *env, jobject thiz,
                                                            jobject bitmap) {
    LOGI("开始调用bitmap2mat");
    // 检测人脸  , opencv 有一个非常关键的类是 Mat ，opencv 是 C 和 C++ 写的，只会处理 Mat , android里面是Bitmap
    // 1. Bitmap 转成 opencv 能操作的 C++ 对象 Mat , Mat 是一个矩阵
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    //置灰显示
    Mat grayMat(mat.rows, mat.cols, CV_8UC1);
    cvtColor(mat, grayMat, COLOR_BGRA2GRAY);

    //把灰色mat赋值到bitmap中
//    mat2Bitmap(env, grayMat, bitmap);

    LOGI("开始调用均衡补偿");
    // 再次处理 直方均衡补偿
    Mat equalize_mat;
    equalizeHist(grayMat, equalize_mat);

    LOGI("开始调用人脸检测");
    // 识别人脸，也可以直接用 彩色图去做,识别人脸要加载人脸分类器文件
    vector<Rect> faces;
    classifier.detectMultiScale(equalize_mat, faces, 1.1, 5);
    LOGE("人脸个数：%lu", faces.size());
    if (!faces.empty()) {
        for (Rect faceRect: faces) {
            // 在人脸部分画个图
            rectangle(mat, faceRect, Scalar(255, 0, 0), 3);
            // 把 mat 我们又放到 bitmap 里面
            mat2Bitmap(env, mat, bitmap);
            LOGI("转换完成");
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_ImgAnliseActivity_loadCascade(JNIEnv *env, jobject thiz,
                                                              jstring file_path) {
    const char *filePath = env->GetStringUTFChars(file_path, JNI_FALSE);
    classifier.load(filePath);
    env->ReleaseStringUTFChars(file_path, filePath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_CheckFaceActivity_loadCascade(JNIEnv *env, jobject thiz,
                                                              jstring file_path) {
    // TODO: implement loadCascade()
    const char *filePath = env->GetStringUTFChars(file_path, JNI_FALSE);
    classifier.load(filePath);
    env->ReleaseStringUTFChars(file_path, filePath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_CheckFaceActivity_checkFace(JNIEnv *env, jobject thiz,
                                                            jobject bitmap) {
    LOGI("开始调用bitmap2mat");
    // 检测人脸  , opencv 有一个非常关键的类是 Mat ，opencv 是 C 和 C++ 写的，只会处理 Mat , android里面是Bitmap
    // 1. Bitmap 转成 opencv 能操作的 C++ 对象 Mat , Mat 是一个矩阵
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    //置灰显示
    Mat grayMat(mat.rows, mat.cols, CV_8UC1);
    cvtColor(mat, grayMat, COLOR_BGRA2GRAY);

    //把灰色mat赋值到bitmap中
//    mat2Bitmap(env, grayMat, bitmap);

    LOGI("开始调用均衡补偿");
    // 再次处理 直方均衡补偿
    Mat equalize_mat;
    equalizeHist(grayMat, equalize_mat);

    LOGI("开始调用人脸检测");
    // 识别人脸，也可以直接用 彩色图去做,识别人脸要加载人脸分类器文件
    vector<Rect> faces;
    classifier.detectMultiScale(equalize_mat, faces, 1.1, 5);
    LOGE("人脸个数：%lu", faces.size());
    if (!faces.empty()) {
        for (Rect faceRect: faces) {
            // 在人脸部分画个图
            rectangle(mat, faceRect, Scalar(255, 0, 0), 3);
            // 文字
            const String text = "Hello DaiLi";
            int fontFace = CV_FONT_BLACK;   // 字体
            double fontScale = 1;           // 字体缩放比
            int thickness = 2;              // 画笔厚度
            int baseline = 0;               // 基线

            putText(mat, text, Point(faceRect.x + 20, faceRect.y + 100), fontFace, fontScale,
                    Scalar(255, 0, 0, 255),
                    thickness, LINE_AA);
            // 把 mat 我们又放到 bitmap 里面
            mat2Bitmap(env, mat, bitmap);
            LOGI("转换完成");
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_medianBlur(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //中值滤波
    medianBlur(mat, mat, 5);
    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_dilate(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    Mat kernel;
    //最大值滤波
    dilate(mat, mat, kernel);
    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_erode(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    Mat kernel;
    //最小值滤波
    erode(mat, mat, kernel);
    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_gune(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    Mat kernel;
    //最小值滤波
    GaussianBlur(mat, mat, Size(0, 0), 15);
    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_bilateralFilter(JNIEnv *env, jobject thiz,
                                                             jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //高斯双边滤波
    Mat gray;
    cvtColor(mat, gray, COLOR_BGRA2GRAY);
    Mat dst;
    bilateralFilter(gray, dst, 0, 150, 15);
    mat2Bitmap(env, dst, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_pyrMeanShiftFiltering(JNIEnv *env, jobject thiz,
                                                                   jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //均值迁移滤波
    Mat dst;
    cvtColor(mat, dst, COLOR_BGRA2BGR);
    pyrMeanShiftFiltering(dst, dst, 10, 50);
    mat2Bitmap(env, dst, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_mohu(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //模糊
    Mat k(3, 3, CV_32FC1);
    float data[9] = {
            1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
            1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
            1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
    };
    k.push_back_(data);
    filter2D(mat, mat, -1, k);
    mat2Bitmap(env, mat, bitmap);
}