/*
*   Proper RGB split of the values
*/
#include <operations.h>

void split_channels(cv::Mat &input,cv::Mat &R,cv::Mat &B,cv::Mat &G)
{
    cv::Mat array[3];
    cv::split(input,array);
    R = array[2];
    G = array[1];
    B = array[0];
}

double single_channel_sum(cv::Mat input)
{
    /*
        Returns values sumed in RGB [0,255] ranges
    */
    double sum=0.0;
    int channels = input.channels();
    int nRows = input.rows;
    int nCols = input.cols * channels;

    if (input.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i,j;
    uchar* p;
    for( i = 0; i < nRows; ++i)
    {
        p = input.ptr<uchar>(i);
        std::cout<<(int)p[0]<<","<<(int)p[1]<<","<<(int)p[2]<<std::endl;
        sum+=p[0];
    }
    return sum;
}
