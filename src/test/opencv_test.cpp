#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
int main()
{
    Mat img;
    img=imread("res/suiginton.png",1);
    namedWindow("suiginton");
    imshow("suiginton", img);
    waitKey(0);
    return 0;
}