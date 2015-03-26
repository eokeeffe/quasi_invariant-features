#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#ifndef IMAGE_BOOST_H
#define IMAGE_BOOST_H

cv::Mat BoostMatrix(cv::Mat image,int order,double sigma);
cv::Mat applyBoostMatrix(cv::Mat &image, cv::Mat& Mboost);

#endif
