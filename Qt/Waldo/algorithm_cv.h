#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>


using namespace std;
using namespace cv;

void Surf();
void Flann();

extern string picturePath;

// 图片转二维数组
int** matToMatrix(Mat &inputImage);
// 图片遍历像素
void colorRecude_iter(Mat& inputImage, Mat& outputImage);


struct Record
{
    int red_line;
    int blue_line;
    int white_line;

    int point_x;
    int point_y;
};

// 绘制出ROI
void Draw_cicle_ROI(Mat &srcImage, vector<Vec3f> &circles);

// 返回检测的圆形的个数集合
void HoughCircles_check(Mat &srcImage);

// 检测眼镜特征
void CheckFeatures(map<int, int> &resultMap, Vector<Record> &result);

bool sortBy_Blue ( const Record &r1 , const Record &r2 );

// 处理图片ROI
void DealMatROI(char **Colors, int rowNumber, int colNumber);

void colorRecude_at(Mat& inputImage, Mat& outputImage);
