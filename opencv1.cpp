#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

//opencv小程序
int main()
{


    Mat img = imread("object/car.png");
    Mat imgGray, imgBlur, mask, imgROI, imgBig ,imgHSV ,imgbin ,imgBlurbin ,imgCanny;
    cvtColor(img,imgGray,COLOR_BGR2GRAY);                                               //转灰度图

    double total = static_cast<double>(img.total());
    int nonZero = countNonZero(imgGray);
    double ratio = nonZero / total;


    cout << "图像尺寸: " << img.cols << " x " << img.rows << "  通道数: " <<  img.channels() << endl;
    cout << "原图非零像素数: " << nonZero << endl;
    cout << "原图非零占比:   " << ratio * 100.0 << "%" << endl;


    threshold(imgGray,imgbin,0,255,THRESH_BINARY | THRESH_OTSU);    //二值化
    imshow("二值化图",imgbin);
    imwrite("object/carbin.jpg",imgbin);


    Rect rect(133,72,50,100);
    imgROI = img(rect).clone();                    //ROI裁减
    imshow("ROI剪切图",imgROI);
    imwrite("object/carCorp.jpg",imgROI);


    resize(img,imgBig,Size(0,0),2,2);               //图片缩放
    imshow("缩放图",imgBig);


    cvtColor(img,imgHSV,COLOR_BGR2HSV);
    int hmin = 78, smin = 60, vmin = 93;
    int hmax = 99, smax = 255, vmax = 255;
    Scalar lower(hmin,smin,vmin);
    Scalar upper(hmax,smax,vmax);


    inRange(imgHSV,lower,upper,mask);               //筛选青色
    imshow("青色筛选图",mask);


    GaussianBlur(imgGray,imgBlur,Size(3,3),3,0);                    //高斯模糊
    imshow("高斯模糊图",imgBlur);
    threshold(imgBlur,imgBlurbin,0,255,THRESH_BINARY | THRESH_OTSU);
    imshow("高斯模糊二值图",imgBlurbin);


    Canny(imgBlur,imgCanny,25,75);
    imshow("边缘化图",imgCanny);                        //边缘化
    imwrite("object/carCanny.jpg",imgCanny);


    cout << "高斯模糊处理前非零像素数: " << countNonZero(imgbin) << endl;
    cout << "高斯模糊处理前非零占比:   " << countNonZero(imgbin) / total * 100.0 << "%" << endl;



    cout << "高斯模糊处理后非零像素数: " << countNonZero(imgBlurbin) << endl;
    cout << "高斯模糊处理后非零占比:   " << countNonZero(imgBlurbin) / total * 100.0 << "%" << endl;
    waitKey(0);
}












//以下为练习程序

// int main()
// {
//     string path = "test.jpg";
//     Mat img = imread("object/car.png");
//     if(img.empty()){
//         cout << "图片读取失败" << endl;
//         return -1;
//     }
//     imshow("OpenCV CMake测试窗口", img);
//     waitKey(0);
//     return 0;
// }

//Mat img = imread("/home/alexie/object/test.jpg");


// int main()                                           //转化灰度图，边缘化，高斯模糊
// {
//     Mat img = imread("object/arm.jpg");
//     Mat imgGray , imgBlur , imgCanny;

//     cvtColor(img,imgGray,COLOR_BGR2GRAY);
//     GaussianBlur(imgGray,imgBlur,Size(3,3),1,0);
//     Canny(imgGray,imgCanny,25,75);



//     //imshow("OpenCV CMake测试窗口", img);
//     imshow("OpenCV CMake测试窗口imggray", imgGray);
//     imshow("OpenCV CMake测试窗口imgBlur", imgBlur);
//     imshow("OpenCV CMake测试窗口imgCanny", imgCanny);
//     waitKey(0);
    
// }

//                                                      //转换HSV空间色彩检测
// Mat imgHSV , mask;
// int hmin = 0, smin = 20, vmin = 20;
// int hmax = 22, smax = 255, vmax = 255;

// int main()
// {
//     Mat img = imread("object/light2.png");
//     Mat imgHSV , mask;
//     int hmin = 0, smin = 20, vmin = 20;
//     int hmax = 180, smax = 255, vmax = 255;

//     cvtColor(img,imgHSV,COLOR_BGR2HSV);

//     namedWindow("Trackbars",WINDOW_AUTOSIZE);
//     // resizeWindow("Trackbars", 640, 200);
//     createTrackbar("Hue Min", "Trackbars", &hmin, 180);
//     createTrackbar("Hue Max", "Trackbars", &hmax, 180);
//     createTrackbar("Sat Min", "Trackbars", &smin, 255);
//     createTrackbar("Sat Max", "Trackbars", &smax, 255);
//     createTrackbar("Val Min", "Trackbars", &vmin, 255);
//     createTrackbar("Val Max", "Trackbars", &vmax, 255);
// while (true)
// {
//     Scalar lower(hmin,smin,vmin);
//     Scalar upper(hmax,smax,vmax);
//     inRange(imgHSV,lower,upper,mask);

//     imshow("img",img);
//     // imshow("imgHSV",imgHSV);
//     imshow("mask",mask);
//     waitKey(1);
// }

// }