/*
*   ColorStructureTensors using OpenCV
*   Evan O'Keeffe
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cfloat>
#include <cmath>
#include <limits>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#ifndef COLOUR_STRUCTURE_TENSOR_H
#define COLOUR_STRUCTURE_TENSOR_H

std::vector<cv::Mat> colourStructureTensor(cv::Mat image, float sigma, float sigma2);
std::vector<cv::Mat> colourStructureTensor2(cv::Mat Cxx, cv::Mat Cxy, cv::Mat Cyy);

#endif
