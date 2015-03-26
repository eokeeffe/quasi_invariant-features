/*
*
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <limits>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#ifndef COLOUR_CANNY_H
#define COLOUR_CANNY_H

cv::Mat colourCanny(cv::Mat in, double sigma1, double sigma2, int method);

#endif
