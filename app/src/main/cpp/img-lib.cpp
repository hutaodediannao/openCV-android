#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include <string>
#include <android/log.h>
#include <opencv2/highgui/highgui_c.h>
#include <math.h>
#include <vector>

using namespace cv;
using namespace std;

#ifndef LOG_TAG
#define LOG_TAG "JNI_TAG"
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

            putText(mat, text, Point(faceRect.x + 20, faceRect.y + 150), fontFace, fontScale,
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
    pyrMeanShiftFiltering(dst, dst, 15, 50);
    mat2Bitmap(env, dst, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_mohu(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //模糊
    Mat myKnernel = (Mat_<double>(3, 3) <<
                                        1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
            1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
            1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f);
    filter2D(mat, mat, -1, myKnernel);
    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_ruiHua(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //模糊
    Mat k(3, 3, CV_32FC1);
    Mat myKnernel = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    filter2D(mat, mat, -1, myKnernel);
    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_BlurActivity_tiDu(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //梯度
    Mat robert_x = (Mat_<double>(3, 3) << -1, 0, 0, 1);
    Mat robert_y = (Mat_<double>(3, 3) << 0, 1, -1, 0);
    filter2D(mat, mat, -1, robert_x);
    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_MorphologyActivity_open(JNIEnv *env, jobject thiz, jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    morphologyEx(mat, mat, MORPH_OPEN, element);

    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_MorphologyActivity_close(JNIEnv *env, jobject thiz,
                                                         jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    morphologyEx(mat, mat, MORPH_CLOSE, element);

    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_MorphologyActivity_topHat(JNIEnv *env, jobject thiz,
                                                          jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    morphologyEx(mat, mat, MORPH_TOPHAT, element);

    mat2Bitmap(env, mat, bitmap);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_MorphologyActivity_blackHat(JNIEnv *env, jobject thiz,
                                                            jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    morphologyEx(mat, mat, MORPH_BLACKHAT, element);

    mat2Bitmap(env, mat, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_ThresholdActivity_threshold(JNIEnv *env, jobject thiz,
                                                            jobject bitmap, jint type) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    Mat gray;
    cvtColor(mat, gray, COLOR_BGRA2GRAY);
    switch (type) {
        case 1:
            threshold(gray, gray, 125, 255, THRESH_BINARY);
            break;
        case 2:
            threshold(gray, gray, 125, 255, THRESH_BINARY_INV);
            break;
        case 3:
            threshold(gray, gray, 125, 255, THRESH_TRUNC);
            break;
        case 4:
            threshold(gray, gray, 125, 255, THRESH_TOZERO);
            break;
        case 5:
            threshold(gray, gray, 125, 255, THRESH_TOZERO_INV);
            break;
        case 6:
            threshold(gray, gray, 0, 255, THRESH_BINARY | THRESH_OTSU);
            break;
        case 7:
            threshold(gray, gray, 0, 255, THRESH_TOZERO_INV | THRESH_TRIANGLE);
            break;
        default:
            break;
    }
    mat2Bitmap(env, gray, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_TiDuActivity_sobel(JNIEnv *env, jobject thiz,
                                                                    jobject bitmap) {
    //X方向梯度
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    Mat gradx;
    Sobel(mat, gradx, CV_32F, 1, 0);
    convertScaleAbs(gradx, gradx);

    //Y方向梯度
    bitmap2Mat(env, bitmap, mat);
    Mat grady;
    Sobel(mat, grady, CV_32F, 0, 1);
    convertScaleAbs(grady, grady);

    Mat dst;
    addWeighted(gradx, 0.5, grady, 0.5, 0, dst);

    mat2Bitmap(env, dst, bitmap);
    gradx.release();
    grady.release();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_TiDuActivity_scharr(JNIEnv *env, jobject thiz,
                                                                     jobject bitmap) {
    //X方向梯度
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    Mat gradx;
    Scharr(mat, gradx, CV_32F, 1, 0);
    convertScaleAbs(gradx, gradx);

    //Y方向梯度
    bitmap2Mat(env, bitmap, mat);
    Mat grady;
    Scharr(mat, grady, CV_32F, 0, 1);
    convertScaleAbs(grady, grady);

    Mat dst;
    addWeighted(gradx, 0.5, grady, 0.5, 0, dst);

    mat2Bitmap(env, dst, bitmap);
    gradx.release();
    grady.release();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_LaplacianActivity_laplacian(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    Mat dst;
    Laplacian(mat, dst, CV_32F, 3, 1, 0);
    convertScaleAbs(dst, dst);

    mat2Bitmap(env, dst, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_CannyActivity_canny(JNIEnv *env, jobject thiz,
                                                                     jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    //高斯模糊处理原图
    GaussianBlur(mat, mat, Size(3, 3), 0);

    //灰度化
    Mat gray;
    cvtColor(mat, gray, COLOR_BGR2GRAY);

    Mat edges;
    Canny(mat, edges, 50, 150, 3, true);

    Mat dst;
    bitwise_and(mat, mat, dst, edges);

    mat2Bitmap(env, dst, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_HoughLinesActivity_hougLines(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);

    Mat gray;
    cvtColor(mat, gray, COLOR_BGRA2GRAY);

    //二值化处理
    threshold(gray, gray, 150, 255,
              THRESH_BINARY);//#自定义，把平均值高于的都截断，（mean=127,127的值都变成255，127以下保持不变）

    //边缘检测
    Mat edges;
    Canny(gray, edges, 50, 150, 3, true);

    //霍夫直线检测
    Mat lines;
    HoughLines(edges, lines, 1, M_PI / 180.0, 100, 50, 10);

    Mat out = Mat::zeros(mat.size(), mat.type());

    for (int i = 0; i < lines.rows; ++i) {
        Mat lMat = lines.row(i);

        int x0 = lMat.data[0];
        int y0 = lMat.data[1];
        int x1 = lMat.data[2];
        int y1 = lMat.data[3];

        line(out, Point(x0, y0), Point(x1, y1), Scalar(0, 0, 255), 5);
    }

    out.copyTo(gray);

    mat2Bitmap(env, gray, bitmap);

    out.release();
    lines.release();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_HoughCirclesActivity_houghCircles(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jobject bitmap) {
    Mat src;
    bitmap2Mat(env, bitmap, src);

    Mat gray;
    cvtColor(src, gray, COLOR_BGRA2GRAY);

//    pyrMeanShiftFiltering(gray, gray, 15, 80);

    GaussianBlur(gray, gray, Size(3, 3), 0);

    Mat dst;
    dst.create(src.size(), src.type());

    Mat circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 20, 100, 30, 10, 200);
    for (int i = 0; i < circles.rows; ++i) {
        int x = circles.col(i).data[0];
        int y = circles.col(i).data[1];
        int r = circles.col(i).data[2];
        LOGI("%d\r,  %d\r, %d\n ", x, y, r);
        circle(dst, Point(x, y), r, Scalar(255, 0, 0), 5, 8, 0);
    }

    mat2Bitmap(env, dst, bitmap);
    circles.release();
    dst.release();
    gray.release();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_ContourActivity_drawContours(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jobject bitmap) {

    Mat src;
    bitmap2Mat(env, bitmap, src);

    //二值化
    Mat gray;
    Mat binary;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

    //轮廓发现
    vector<Mat> mats;
    Mat hierarchy;
    findContours(binary, mats, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    //绘制轮廓
    Mat dst(src.size(), src.type());
    for (int i = 0; i < mats.size(); ++i) {
        drawContours(dst, mats, i, Scalar(255, 0, 0), 5);
    }

    //开始装换并释放内存
    mat2Bitmap(env, dst, bitmap);
    gray.release();
    binary.release();
    hierarchy.release();
    dst.release();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_RotateRectActivity_measureContours(JNIEnv *env,
                                                                                    jobject thiz,
                                                                                    jobject bitmap) {
    Mat src;
    Mat gray;
    Mat binary;

    //二值化
    bitmap2Mat(env, bitmap, src);
    cvtColor(src, gray, COLOR_BGR2GRAY);
//    threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
    threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

    //轮廓发现
    vector<Mat> contours;
    Mat hierarchy;
//    findContours(binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    //测绘轮廓
    Mat dst(src.size(), src.type());
    for (int i = 0; i < contours.size(); ++i) {
        Rect rect = boundingRect(contours.at(i));
        double w = rect.width;
        double h = rect.height;
        double rate = min(w, h) / max(w, h);
        RotatedRect minRect = minAreaRect(contours.at(i));
        w = minRect.size.width;
        h = minRect.size.height;
        rate = min(w, h) / max(w, h);

        double area = contourArea(contours.at(i), false);
        double arclen = arcLength(contours.at(i), true);
        drawContours(dst, contours, i, Scalar(0, 255, 0), 3);
    }
    mat2Bitmap(env, dst, bitmap);

    gray.release();
    binary.release();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myopencvndkapp_featureDetection_TemplateActivity_matchTemplate(JNIEnv *env,
                                                                                jobject thiz,
                                                                                jobject bitmap,
                                                                                jobject temp) {

    Mat source;
    Mat src;
    Mat tmp;
    bitmap2Mat(env, bitmap, source);
    cvtColor(source, src, COLOR_RGB2GRAY);
    bitmap2Mat(env, temp, tmp);
    cvtColor(tmp, tmp, COLOR_RGB2GRAY);

    int height = src.rows - tmp.rows + 1;
    int width = src.cols - tmp.cols + 1;
    Mat result(height, width, CV_32FC1);

    //模板匹配
    int method = TM_CCOEFF_NORMED;
    matchTemplate(src, tmp, result, method);
    Point matchloc;
    Point minloc;
    Point maxloc;
    minMaxLoc(src, NULL, NULL, &minloc, &maxloc);
    if (method == TM_SQDIFF || method == TM_SQDIFF_NORMED) {
        matchloc = minloc;
    } else {
        matchloc = maxloc;
    }
    rectangle(source, matchloc, Point(matchloc.x + tmp.cols, matchloc.y + tmp.rows), Scalar(255, 0, 0),3);
    mat2Bitmap(env, source, bitmap);

    tmp.release();
    result.release();
}