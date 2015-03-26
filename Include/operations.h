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

#ifndef OPERATIONS_H
#define OPERATIONS_H

void split_channels(cv::Mat &input,cv::Mat &R,cv::Mat &B,cv::Mat &G);
double single_channel_sum(cv::Mat input);

#endif
