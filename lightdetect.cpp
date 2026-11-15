#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

// 在 HSV 图上按 min/max 范围取掩膜
static void inRangeHSV(const Mat& hsv, int hmin, int hmax,
                       int smin, int smax, int vmin, int vmax, Mat& mask)
{
    Mat m;
    inRange(hsv, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), m);
    if (mask.empty()) mask = m;
    else             mask |= m;   // 多段合并
}

int main()
{
    // 1. 读取图片
    Mat img = imread("object/light.jpg");
    if (img.empty()) {
        cout << "图片读取失败，检查路径" << endl;
        return -1;
    }

    // 2. BGR -> HSV
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    // 3. 阈值分割（OpenCV H 范围 0~179，S/V 0~255）
    //    红色：H 在 [0,10] 和 [170,179] 两段
    //    蓝色：H 在 [100,130]
    Mat maskR, maskB, mask;
    inRangeHSV(hsv,   0,  10, 120, 255, 80, 255, maskR);
    inRangeHSV(hsv, 170, 179, 120, 255, 80, 255, maskR);
    inRangeHSV(hsv, 100, 130, 120, 255, 80, 255, maskB);

    mask = maskR | maskB;

    // 4. 形态学去噪 + 连通
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(mask, mask, MORPH_OPEN,  kernel);  // 去小噪点
    morphologyEx(mask, mask, MORPH_CLOSE, kernel, Point(-1,-1), 2); // 补断裂

    imshow("mask", mask);

    // 5. 找轮廓
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat result = img.clone();
    int count = 0;

    for (auto& c : contours)
    {
        double area = contourArea(c);
        if (area < 200) continue;              // 面积过滤

        Rect r = boundingRect(c);

        // 长宽比过滤：灯条是细长的（竖条 h/w 大，横条 w/h 大）
        double ratio = (double)r.height / r.width;
        double inv   = (double)r.width / r.height;
        if (!(ratio > 1.5 || inv > 1.5)) continue;

        rectangle(result, r, Scalar(0, 255, 0), 2);   // 绿色框
        ++count;

        cout << "灯条 " << count << ": x=" << r.x << " y=" << r.y
             << " w=" << r.width << " h=" << r.height
             << " area=" << area << endl;
    }

    cout << "共检测到 " << count << " 个灯条" << endl;

    // 6. 保存并显示
    imwrite("result.jpg", result);
    imshow("result", result);
    waitKey(0);
    return 0;
}
