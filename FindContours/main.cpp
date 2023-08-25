#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int main (){

    Mat image = cv::imread("C:/Qt_projects/projects/opencv_test/IMG.jpg");
    if(image.empty()){
        std::cout << "not image";
        return -1;
    }
    namedWindow("Find contours", WINDOW_AUTOSIZE);

    Mat img_gray, img_thresh, img_blur;

    blur(image, img_blur, Size(3,3));
    cvtColor(img_blur, img_gray, COLOR_BGR2GRAY);
    threshold(img_gray, img_thresh, 70, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<Point> approxCurve;
    findContours(img_thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    Mat image_copy = image.clone();
    drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);

    for(size_t i=0; i<contours.size(); ++i){
        Rect rect = boundingRect(contours[i]);
        Point center = Point(rect.x+rect.width/2-50 , rect.y+rect.height-50);

        if (contours[i].size()){
            double epsilon = 0.01*arcLength(contours[i], true);
            approxPolyDP(contours[i], approxCurve, epsilon, true);
            if (approxCurve.size()){
                if (approxCurve.size() == 3)
                {
                    putText(image_copy, "triangle", center, FONT_ITALIC, 1, {255,255,255});
                }
                else if(approxCurve.size() == 4)
                {
                    if (rect.width/100 == rect.height/100)
                    {
                        putText(image_copy, "square", center, FONT_ITALIC, 1, {255,255,255});
                    }
                    else
                    {
                        putText(image_copy, "rectangle", center, FONT_ITALIC, 1, {255,255,255});
                    }
                }
                else if(approxCurve.size() > 8)
                {
                    putText(image_copy, "circle", center, FONT_ITALIC, 1, {255,255,255});
                }
            }
        }
    }

    imshow("None approximation", image_copy);
    waitKey(0);
    destroyAllWindows();

    return 0;
}
