#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "gder.h"

using namespace std;
using namespace cv;

#ifndef IMAGE_BOOST
#define IMAGE_BOOST

cv::Mat BoostMatrix(cv::Mat image,int order,float sigma);
cv::Mat applyBoostMatrix(cv::Mat image, cv::Mat Mboost);

#endif
