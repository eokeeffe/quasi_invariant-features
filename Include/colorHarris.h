/*
*
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;


#ifndef COLOR_HARRIS
#define COLOR_HARRIS

cv::Mat colorHarris(cv::Mat in, double sigma_g, double sigma_a, double k, int method);

#endif
