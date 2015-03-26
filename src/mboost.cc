/*
*   Created By Evan O'Keeffe
*/

#include <mboost.h>
#include <gder.h>
#include <operations.h>

using namespace std;
using namespace cv;

cv::Mat BoostMatrix(cv::Mat image,int order,double sigma)
{
    /*
        % computes boost-matrix, Mboost, from a single image
        %
        % image:
        %   1. class(image) = double   input is a single color image (w*h*3)
        % order:
        %       order of differentiation. For standard color boosting the first
        %       order derivatives are used (order=1)
        % sigma:
        %       sigma of Gaussian derivative
    */
    cv::Mat R(image.rows,image.cols,CV_32F),
    G(image.rows,image.cols,CV_32F),
    B(image.rows,image.cols,CV_32F);

    split_channels(image,R,G,B);

    if(order!=0)
    {
        //apply first gaussian derivative to each channel
        cv::Mat Rx,Ry,Gx,Gy,Bx,By,Temp;

        //vconcat(A, B, V);
        Rx = gder(R,sigma,1,0);
        Ry = gder(R,sigma,0,1);
        //cv::sepFilter2D(R,R,R.depth(),Rx,Ry,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);
        //hconcat(Rx, Ry, Temp);
        cv::filter2D(R,R,R.depth(),Rx,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);
        cv::filter2D(R,R,R.depth(),Ry,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);

        Gx = gder(G,sigma,1,0);
        Gy = gder(G,sigma,0,1);
        //cv::sepFilter2D(G,G,G.depth(),Gx,Gy,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);
        //hconcat(Gx, Gy, Temp);
        cv::filter2D(G,G,G.depth(),Gx,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);
        cv::filter2D(G,G,G.depth(),Gy,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);

        Bx = gder(B,sigma,1,0);
        By = gder(B,sigma,0,1);
        //cv::sepFilter2D(B,B,B.depth(),Bx,By,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);
        //hconcat(Bx, By, Temp);
        cv::filter2D(B,B,B.depth(),Bx,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);
        cv::filter2D(B,B,B.depth(),By,cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);
    }

    cv::Mat M=Mat::zeros(3,3,CV_32F),W=Mat::zeros(3,3,CV_32F),
        U=Mat::zeros(3,3,CV_32F),Vt=Mat::zeros(3,3,CV_32F),
        S=Mat::zeros(3,3,CV_32F),SVt=Mat::zeros(3,3,CV_32F);

    M.at<double>(0,0) = cv::sum(R.mul(R))[0];
    M.at<double>(0,1) = cv::sum(R.mul(G))[0];
    M.at<double>(0,2) = cv::sum(R.mul(B))[0];

    M.at<double>(1,1) = cv::sum(G.mul(G))[0];
    M.at<double>(1,2) = cv::sum(G.mul(B))[0];
    M.at<double>(2,2) = cv::sum(B.mul(B))[0];

    M.at<double>(1,0) = M.at<double>(0,1);
    M.at<double>(2,0) = M.at<double>(0,2);
    M.at<double>(2,1) = M.at<double>(1,2);

    M = M/(image.rows*image.cols);

    cv::SVD::compute(M,W,U,Vt,cv::SVD::FULL_UV);
    //place values of W into diagonal of S
    //mboost = U * (diag(1./diag(sqrt(S)))*V');
    cv::sqrt(Mat::diag(W),S);
    S = 1./S;
    return U * S * Vt;
}

cv::Mat applyBoostMatrix(cv::Mat &image, cv::Mat &Mboost)
{
    cv::Mat R(image.rows,image.cols,CV_32F),
    G(image.rows,image.cols,CV_32F),
    B(image.rows,image.cols,CV_32F),
    colour_planes[3],colour_boost(image.rows,image.cols,CV_32F);

    split_channels(image,R,G,B);

    cv::filter2D(R, R, R.depth() , Mboost, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
    cv::filter2D(G, G, G.depth() , Mboost, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
    cv::filter2D(B, B, B.depth() , Mboost, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);

    //Put the RGB channels back together
    std::vector<cv::Mat> array_to_merge;
    array_to_merge.push_back(R);
    array_to_merge.push_back(G);
    array_to_merge.push_back(B);
    cv::merge(array_to_merge, colour_boost);

    //cv::filter2D(colour_boost, colour_boost, colour_boost.depth() , Mboost, cv::Point(-1,-1), 1.0, cv::BORDER_DEFAULT);

    return colour_boost;
}
