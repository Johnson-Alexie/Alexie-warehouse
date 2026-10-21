#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    Mat img = imread("test.jpg");
    if(img.empty()){
        std::cout << "图片读取失败" << std::endl;
        return -1;
    }
    imshow("OpenCV CMake测试窗口", img);
    waitKey(0);
    return 0;
}
