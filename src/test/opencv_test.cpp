#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <unistd.h>
using namespace cv;
int main()
{
    Mat img;
    img=imread("res/suiginton.png",1);
    namedWindow("test");
    imshow("test",img);
    waitKey(50);
    std::cout<<"waitKey stop"<<std::endl;
    sleep(10);
    
    return 0;
}