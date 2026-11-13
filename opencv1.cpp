#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// int main()
// {
//     string path = "test.jpg";
//     Mat img = imread(path);
//     if(img.empty()){
//         cout << "图片读取失败" << endl;
//         return -1;
//     }
//     imshow("OpenCV CMake测试窗口", img);
//     waitKey(0);
//     return 0;
// }



int main()
{
    string path = "test.jpg";
    Mat img = imread(path);
    Mat imgGray;
    cvtColor(img,imgGray,COLOR_BGR2GRAY);
    imshow("OpenCV CMake测试窗口", img);
    imshow("OpenCV CMake测试窗口imggray", imgGray);
    waitKey(0);
    
}



