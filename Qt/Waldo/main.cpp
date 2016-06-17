#include "algorithm_cv.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

string picturePath = "E://MyGit//Qt//build-Waldo-Desktop_Qt_5_6_1_MSVC2015_64bit-Debug//1.png";

void handlePicture(string pictPath)
{
    Mat inputImage = imread(pictPath) , outputImage;

    colorRecude_at(inputImage, outputImage);

    imshow("outputImage", outputImage);

    waitKey(0);
}


int main()
{

    handlePicture(picturePath);
    return 0;


//    Mat inputImage = imread("E://MyGit//Qt//build-Waldo-Desktop_Qt_5_6_1_MSVC2015_64bit-Debug//1.png");
//    Rect rect(413, 1000, 90, 146);
//    Mat srcImage = inputImage(rect);

//    // Mat srcImage = imread("E://MyGit//Qt//build-Waldo-Desktop_Qt_5_6_1_MSVC2015_64bit-Debug//out.png");

//    HoughCircles_check(srcImage);

    return 0;
}
