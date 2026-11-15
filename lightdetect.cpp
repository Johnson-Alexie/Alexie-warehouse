#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;


int main()
{
    Mat img = imread("object/light2.png");
    Mat imgHSV, mask  ;

    cvtColor(img,imgHSV,COLOR_BGR2HSV);
    int hmin = 100, smin = 20, vmin = 0;
    int hmax = 140, smax = 255, vmax = 255;
    Scalar lower(hmin,smin,vmin);
    Scalar upper(hmax,smax,vmax);

    inRange(imgHSV,lower,upper,mask);                                       //筛选蓝色灯条
    imshow("蓝色筛选图",mask);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
// 开运算：去小白点噪声
    morphologyEx(mask,mask, MORPH_OPEN, kernel);
// 迭代 2 次
    morphologyEx(mask, mask, MORPH_CLOSE, kernel, Point(-1,-1), 2);
//反色，防止找不到轮廓
    bitwise_not(mask, mask);
    imshow("蓝色筛选优化图",mask);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);       //寻找轮廓

    Mat result  = img.clone();

    for(int i = 0;i < contours.size();i++)
    {
        double area = contourArea(contours[i]);
        if (area < 200) 
        {
                continue;
        }                                                   //面积过滤

        Rect r = boundingRect(contours[i]);

        rectangle(result, r, Scalar(0, 255, 0),2);          //圈出灯条
    }

    imshow("result", result);
    imwrite("object/lightdetect.jpg",result);

    waitKey(0);
}













