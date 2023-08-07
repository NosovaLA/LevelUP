#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int main (){

    Mat img = cv::imread("C:/photo.jpg");
    if(img.empty()){
        std::cout << "not img";
        return -1;
    }
    namedWindow("Testing OpenCV", WINDOW_AUTOSIZE);

    Mat img_blur10x10, img_Gblur5x5, img_threshold,
        img_gray, img_hsv, img_canny;

    blur(img,img_blur10x10, Size(10,10));
    GaussianBlur(img, img_Gblur5x5, Size(5,5), 0);
    threshold(img, img_threshold, 125, 255, 3);
    cvtColor(img, img_gray, COLOR_RGB2GRAY, 0);
    cvtColor(img, img_hsv, COLOR_RGB2HSV, 0);
    Canny(img, img_canny, 0, 100, 3);

    imshow("Origin", img);
    imshow("Blur", img_blur10x10);
    imshow("GaussianBlur", img_Gblur5x5);
    imshow("Threshold", img_threshold);
    imshow("Gray", img_gray);
    imshow("HSV", img_hsv);
    imshow("Canny", img_canny);

    waitKey(0);
    destroyAllWindows();

    return 0;
}