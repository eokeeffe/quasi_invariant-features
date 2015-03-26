
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

#ifndef OPPONENT_DERIVATION_H
#define OPPONENT_DERIVATION_H

std::vector<cv::Mat> opponent_der(cv::Mat input,double sigma);

#endif
