#include "algorithm_cv.h"

using namespace std;
using namespace cv;

void Surf()
{
    // 【1】载入素材图
    Mat srcImage1 = imread("E://MyGit//Qt//build-Waldo-Desktop_Qt_5_6_1_MSVC2015_64bit-Debug//1_1.png", 1);
    Mat srcImage2 = imread("E://MyGit//Qt//build-Waldo-Desktop_Qt_5_6_1_MSVC2015_64bit-Debug//2_2.png", 1);

    if (! srcImage1.data || ! srcImage2.data)
    {
        cout << "读取图片失败" << endl;
        return;
    }

    // 【2】用SURF算子检测关键点
    int minHseeian = 700;	// SURF算法中的hessian阈值
    SurfFeatureDetector detector(minHseeian);		// 定义一个特征验证类对象
    vector<KeyPoint> keypoint1, keypoint2;

    // 【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
    detector.detect(srcImage1, keypoint1);
    detector.detect(srcImage2, keypoint2);

    // 【4】计算描述符（特征向量）
    SurfDescriptorExtractor extractor;
    Mat descriptors1, descriptors2;
    extractor.compute(srcImage1, keypoint1, descriptors1);
    extractor.compute(srcImage2, keypoint2, descriptors2);

    // 【5】使用BruteForce进行匹配
    //      实例化一个匹配器
    BruteForceMatcher< L2<float> > matcher;
    vector< DMatch > matches;
    //      匹配两幅图中的描述子
    matcher.match(descriptors1, descriptors2, matches);

    // 【6】绘制从两个图像中匹配的关键点
    Mat imgMatches;
    drawMatches(srcImage1, keypoint1, srcImage2, keypoint2, matches, imgMatches);	//进行绘制

    // 【7】显示效果图
    imshow("fit", imgMatches);

    waitKey(0);
}


void Flann()
{
    Mat trainImage = imread("E://MyGit//Qt//build-Waldo-Desktop_Qt_5_6_1_MSVC2015_64bit-Debug//2_2.png", 1), trainImage_gray;
    Mat testImage = imread("E://MyGit//Qt//build-Waldo-Desktop_Qt_5_6_1_MSVC2015_64bit-Debug//3.png", 1), testImage_gray;

    if (! trainImage.data || ! testImage.data)
    {
        cout << "Wrong !" << endl;
        return;
    }

    cvtColor(trainImage, trainImage_gray, CV_BGR2GRAY);
    cvtColor(testImage, testImage_gray, CV_BGR2GRAY);


    vector<KeyPoint> train_KeyPoint, test_KeyPoint;
    Mat trainDescriptor, testDescriptor;
    int minHseeian_train = 10;	// SURF算法中的hessian阈值
    SurfFeatureDetector featureDetector_train(minHseeian_train);		// 定义一个特征验证类对象
    featureDetector_train.detect(trainImage, train_KeyPoint);
    int minHseeian_test = 10;	// SURF算法中的hessian阈值
    SurfFeatureDetector featureDetector_test(minHseeian_test);		// 定义一个特征验证类对象
    featureDetector_test.detect(testImage, test_KeyPoint);
    SurfDescriptorExtractor featureDxtractor;
    featureDxtractor.compute(trainImage, train_KeyPoint, trainDescriptor);
    featureDxtractor.compute(testImage, test_KeyPoint, testDescriptor);

    FlannBasedMatcher matcher;
    vector<Mat> train_desc_collection(1, trainDescriptor);
    matcher.add(train_desc_collection);
    matcher.train();


    vector<vector<DMatch> > matches;
    matcher.knnMatch(testDescriptor, matches, 2);

    vector<DMatch> goodMatches;
    for (unsigned int i = 0; i < matches.size(); ++i)
    {
        if (matches[i][0].distance < 0.6 * matches[i][1].distance)
        {
            goodMatches.push_back(matches[i][0]);
        }
    }

    Mat dstImage;
    drawMatches(trainImage, train_KeyPoint, testImage, test_KeyPoint, goodMatches, dstImage);	//进行绘制

    imshow("image", dstImage);

    waitKey(0);
}

//string picturePath;

// 图片转二维数组
int** matToMatrix(Mat &inputImage)
{
    // 参数准备
    int rowNumber = inputImage.rows;
    int colNumber = inputImage.cols * inputImage.channels();

//    std::cout << "channels == " << inputImage.channels() << std::endl;
//    std::cout << "cols == " << inputImage.cols << std::endl;

    // 双重循环，遍历所有的像素值
    int **a;
    a = new int*[rowNumber];

    for (int i = 0; i < rowNumber; ++i)
    {
        a[i] = new int[colNumber];
    }

    for (int i = 0; i < rowNumber; ++i)
    {
        uchar* data = inputImage.ptr<uchar>(i);
        for (int j = 0; j < colNumber; ++j)
        {
            a[i][j] = (data[j] == 255 ? 1 : 0);
        }
    }

//    int bar_width = 3;

//    for (int i = 0; i < rowNumber; ++i)
//    {
//        for (int j = 0; j < colNumber - (bar_width - 1); ++j)
//        {
//            std::cout << a[i][j] << ',';
//        }
//        std::cout << '\n' << endl;
//    }

    return a;
}

// 图片遍历像素
void colorRecude_iter(Mat& inputImage, Mat& outputImage)
{
    outputImage = inputImage.clone();
    for( Mat_<Vec3b>::iterator iter  = outputImage.begin<Vec3b>(); iter != outputImage.end<Vec3b>(); ++iter)
    {
        if (220 <= (*iter)[0] && (*iter)[0] <= 240)
        {
            if (40 <= (*iter)[1] && (*iter)[1] <= 60)
            {
                if (20 <= (*iter)[2] && (*iter)[2] <= 40)
                {
                    (*iter)[0] = 0;
                    (*iter)[1] = 0;
                    (*iter)[2] = 0;
                }
            }

        }
    }
}


void Draw_circle_ROI(Mat &srcImage, vector<Vec3f> &circles)
{
    for (size_t i = 0; i < circles.size(); ++i)
    {
        // 参数定义
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        if (radius >= 8)
        {
            break;
        }
        // 绘制圆心
        circle(srcImage, center, 1, Scalar(0, 0, 255), -1, 8, 0);

        //绘制圆轮廓
        circle(srcImage, center, radius, Scalar(0, 255, 0), 1, 8, 0);

    }

    imshow("result", srcImage);

    waitKey(0);
}


// 返回检测的圆形的个数集合
void HoughCircles_check(Mat &srcImage)
{
    Mat midImage;

    cvtColor(srcImage, midImage, COLOR_BGR2GRAY);
    imshow("mid", midImage);
    GaussianBlur(midImage, midImage, Size(3, 3), 1, 1);
    vector<Vec3f> circles;
    HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, 1, 2, 20, 20, 0, 0);
    Draw_circle_ROI(srcImage, circles);
}


// 检测眼镜特征
void CheckFeatures(map<int, int> &resultMap, Vector<Record> &result)
{
    map<int, int> resultMap_Point;
    for (map<int, int>::iterator iter = resultMap.begin(); iter != resultMap.end(); ++iter)
    {
        // cout << iter->first << " - " << iter->second << endl;
        for (int ix = 0; ix < result.size(); ++ix)
        {
            if (result[ix].point_x == iter->first * 100)
            {
                resultMap_Point[result[ix].point_x] = result[ix].point_y;
                break;
            }
        }
    }

    for (map<int, int>::iterator iter = resultMap_Point.begin(); iter != resultMap_Point.end(); ++iter)
    {
        cout << iter->first << " - " << iter->second << endl;
        Mat inputImage = imread(picturePath);
        cout << "picturePath" << picturePath << endl;
        Rect rect(iter->second, iter->first, 90, 146);
        Mat srcImage = inputImage(rect);

        HoughCircles_check(srcImage);
    }
}



bool sortBy_Blue ( const Record &r1 , const Record &r2 )
{
    return (r1.blue_line > r2.blue_line) ;
}


// 处理图片ROI
void DealMatROI(char **Colors, int rowNumber, int colNumber)
{
    // 根据Colors处理特征
    int ROI_width = 90;
    int ROI_height = 145;
    Vector<Record> records, result;
    map<int, int> resultMap;
    for (int i = 0; i < rowNumber - (ROI_height - 1); ++i)
    {
        cout << i << endl;
        for (int j = 0; j < colNumber - (ROI_width - 1); ++j)
        {
            Record _record;
            _record.red_line = 0;
            _record.blue_line = 0;
            _record.white_line = 0;

            for (int _i = 0; _i < ROI_height; ++_i)
            {
                int m = i + _i, n;
                int count_line_red = 0;
                int count_line_blue = 0;
                int count_line_white = 0;
                for (int _j = 0; _j < ROI_width; ++_j)
                {
                    n = j + _j;
                    count_line_red += (Colors[m][n] == 'R');
                    count_line_blue += (Colors[m][n] == 'B');
                    count_line_white += (Colors[m][n] == 'W');
                }

                _record.red_line += (count_line_red >= 12);
                _record.blue_line += (count_line_blue >= 8);
                _record.white_line += (count_line_white >= 10);
            }


            if (_record.red_line >= 28 && _record.blue_line >= 27 && _record.white_line >= 16)
            {
                _record.point_x = i;
                _record.point_y = j;

                records.push_back(_record);
            }
        }
    }

    // cout << "Red_White_Blue: " << records.size() << endl;
    stable_sort(records.begin(), records.end(), sortBy_Blue);

    int sum = 0;

    for (int ix = 0; ix < records.size(); ++ix)
    {
        int x = records[ix].point_x, y = records[ix].point_y;
        int count_line_RedWhite_Blue = 0;
        char before_color = '_';
        for (int line_y = 0; line_y < ROI_width; ++line_y)
        {
            // 检测是否是满足红和白颜色顺序，是否满足蓝色在下方
            int start_red = -1, start_white = -1, start_blue = -1, red_white = 0, white_red = 0;
            for (int line_x = 0; line_x < ROI_height; ++line_x)
            {
                if (Colors[line_x + x][line_y + y] == 'R')
                {
                    start_red = (start_red < 0 ? line_x : start_red);
                    if (before_color = 'W')
                    {
                        white_red++;
                    }
                    before_color = 'R';
                }

                if (Colors[line_x + x][line_y + y] == 'W')
                {
                    start_white = (start_white < 0 ? line_x : start_white);
                    if (before_color = 'R')
                    {
                        red_white++;
                    }
                    before_color = 'W';
                }

                if (Colors[line_x + x][line_y + y] == 'B' && start_red >= 0)
                {
                    start_blue = (start_blue < 0 ? line_x : start_blue);
                    break;
                }
            }

            // 是否红和白色
            bool is_RedWhite = false;
            if (red_white >= 4 && white_red >= 10)
            {
                is_RedWhite = true;
            }

            // 是否蓝色在下方
            bool is_RedWhite_Blue = true;
            for (int line_x = start_blue; line_x < ROI_height - 1; ++line_x)
            {
                if (Colors[line_x + x][line_y + y] == 'R')
                {
                    is_RedWhite_Blue = false;
                    break;
                }
            }

            if (is_RedWhite && is_RedWhite_Blue)
            {
                count_line_RedWhite_Blue++;
            }
        }

//        if (records[ix].point_x == 900 && records[ix].point_y == 727)
//        {
//            cout << endl;
//            cout << records[ix].point_x << " - " << records[ix].point_y << " - " << records[ix].red_line << " - " << records[ix].blue_line << " - " << records[ix].white_line << endl;
//            cout << "point_x - point_y: count_line_RedWhite_Blue == " << count_line_RedWhite_Blue << endl;
//            cout << endl;
//        }

        if (count_line_RedWhite_Blue >= 4)
        {
            ++sum;
            result.push_back(records[ix]);
            cout << "ROI's pos: " << records[ix].point_x << " - " << records[ix].point_y << endl;
            int k = records[ix].point_x/ 100 + (records[ix].point_x % 100 >= 60);
            resultMap[k]++;
        }
    }

    cout << "sum == " << sum << endl;
    cout << "map == " << resultMap.size() << '\n' << endl;

    CheckFeatures(resultMap, result);
}


void colorRecude_at(Mat& inputImage, Mat& outputImage)
{

    outputImage = inputImage.clone();
    int rowNumber = outputImage.rows;    // 行
    int colNumber = outputImage.cols;    // 列

    ofstream out("Colors.txt");
    out << rowNumber << '\t' << colNumber << endl;
    char **Colors;
    Colors = new char*[rowNumber];
    for (int i = 0; i < rowNumber; ++i)
    {
        Colors[i] = new char[colNumber];
    }

    for (int i = 0; i < rowNumber; ++i)
    {
        for (int j = 0; j < colNumber; ++j)
        {
            // 检测 红色区域
            if (150 <= outputImage.at<Vec3b>(i, j)[2] && outputImage.at<Vec3b>(i , j)[2] <= 255)
            {
                if (10 <= outputImage.at<Vec3b>(i , j)[1] && outputImage.at<Vec3b>(i , j)[1] <= 90)
                {
                    if (30 <= outputImage.at<Vec3b>(i , j)[0] && outputImage.at<Vec3b>(i , j)[0] <= 100)
                    {
                        outputImage.at<Vec3b>(i , j)[0] = 0;     // blue
                        outputImage.at<Vec3b>(i , j)[1] = 0;     // green
                        outputImage.at<Vec3b>(i , j)[2] = 0;     // red

                        Colors[i][j] = 'R';
                    }
                }
            }

            // 检测蓝色区域
            if (0 <= outputImage.at<Vec3b>(i , j)[2] && outputImage.at<Vec3b>(i , j)[2] <= 20)
            {
                if (140 <= outputImage.at<Vec3b>(i , j)[1] && outputImage.at<Vec3b>(i , j)[1] <= 180)
                {
                    if (160 <= outputImage.at<Vec3b>(i , j)[0] && outputImage.at<Vec3b>(i , j)[0] <= 255)
                    {
                        outputImage.at<Vec3b>(i , j)[0] = 0;     // blue
                        outputImage.at<Vec3b>(i , j)[1] = 0;     // green
                        outputImage.at<Vec3b>(i , j)[2] = 0;     // red

                        Colors[i][j] = 'B';
                    }
                }
            }

            // 检测白色区域
            if (230 <= outputImage.at<Vec3b>(i , j)[2] && outputImage.at<Vec3b>(i , j)[2] <= 255)
            {
                if (200 <= outputImage.at<Vec3b>(i , j)[1] && outputImage.at<Vec3b>(i , j)[1] <= 255)
                {
                    if (200 <= outputImage.at<Vec3b>(i , j)[0] && outputImage.at<Vec3b>(i , j)[0] <= 255)
                    {
                        outputImage.at<Vec3b>(i , j)[0] = 0;     // blue
                        outputImage.at<Vec3b>(i , j)[1] = 0;     // green
                        outputImage.at<Vec3b>(i , j)[2] = 0;     // red

                        Colors[i][j] = 'W';
                    }
                }
            }
        }
    }

    for (int i = 0; i < rowNumber; ++i)
    {
        for (int j = 0; j < colNumber; ++j)
        {
            out << ( (Colors[i][j] == 'R' || Colors[i][j] == 'B' || Colors[i][j] == 'W') ? Colors[i][j] : '_' );
            out << ( (j == colNumber - 1) ? '\n' : ' ' );
        }
    }
    out.close();

    DealMatROI(Colors, rowNumber, colNumber);  // 处理图片
}

