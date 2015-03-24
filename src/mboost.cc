/*
*   Created By Evan O'Keeffe
*/

#include <mboost.h>
#include <gder.h>

cv::Mat BoostMatrix(cv::Mat image,int order,float sigma)
{
    cv::Mat R,G,B,colour_planes[3];
    split(image,colour_planes);  // planes[2] is the red channel

    R = colour_planes[2];
    G = colour_planes[1];
    B = colour_planes[0];

    if(order!=0)
    {
        //apply first gaussian derivative to each channel
        cv::Mat rx,ry,gx,gy,bx,by;
        rx = gder(R,sigma,1,0);
        ry = gder(R,sigma,0,1);
        cv::filter2D(R, R, R.depth() , rx, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
        cv::filter2D(R, R, R.depth() , ry, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);

        gx = gder(G,sigma,1,0);
        gy = gder(G,sigma,0,1);
        cv::filter2D(G, G, G.depth() , gx, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
        cv::filter2D(G, G, G.depth() , gy, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);

        bx = gder(B,sigma,1,0);
        by = gder(B,sigma,0,1);
        cv::filter2D(B, B, B.depth() , bx, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
        cv::filter2D(B, B, B.depth() , by, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
    }

    cv::Mat M(3,3,CV_64F),W,U,V,S=Mat::zeros(3,3,CV_64F);

    M.at<double>(0,0) = cv::sum(R.mul(R))[0];
    M.at<double>(0,1) = cv::sum(R.mul(G))[0];
    M.at<double>(0,2) = cv::sum(R.mul(B))[0];
    M.at<double>(1,1) = cv::sum(G.mul(G))[0];
    M.at<double>(1,2) = cv::sum(G.mul(B))[0];
    M.at<double>(2,2) = cv::sum(B.mul(B))[0];

    M.at<double>(1,0) = M.at<double>(0,1);
    M.at<double>(2,0) = M.at<double>(0,2);
    M.at<double>(2,1) = M.at<double>(1,2);

    M = M / (image.rows * image.cols);
    //M compared to Matlab results and kinda working
    //error margin exists due to rounding errors in opencv/c++ versus Matlab
    //heavy customization maybe required to reduce error margin

    SVD::compute(M,W,U,V);
    //Put values of W onto Diagonal of S
    double S1 = 1.0/sqrt(W.at<double>(0));
    double S2 = 1.0/sqrt(W.at<double>(1));
    double S3 = 1.0/sqrt(W.at<double>(2));

    //make sure inf doesn't show up in the matrix or
    //just about all elements will become inf
    if(std::isfinite(S1)){S.at<double>(0,0) = S1;}
    if(std::isfinite(S2)){S.at<double>(1,1) = S2;}
    if(std::isfinite(S3)){S.at<double>(2,2) = S3;}

    //mboost = U * (diag(1./diag(sqrt(S)))*V');
    cv::Mat SVt = Mat::ones(3,3,CV_64F);
    cv::Mat result = S*V.t();
    SVt.at<double>(0,0) = result.at<double>(0,0);
    SVt.at<double>(1,1) = result.at<double>(1,1);
    SVt.at<double>(2,2) = result.at<double>(2,2);
    //std::cout<<SVt<<std::endl;
    return U * SVt;
}

cv::Mat applyBoostMatrix(cv::Mat image, cv::Mat Mboost)
{
    cv::Mat R,G,B,colour_planes[3],colour_boost;
    split(image,colour_planes);  // planes[2] is the red channel

    R = colour_planes[2];
    G = colour_planes[1];
    B = colour_planes[0];

    cv::filter2D(R, R, R.depth() , Mboost, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
    cv::filter2D(G, G, G.depth() , Mboost, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
    cv::filter2D(B, B, B.depth() , Mboost, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);

    //Put the RGB channels back together
    std::vector<cv::Mat> array_to_merge;
    array_to_merge.push_back(B);
    array_to_merge.push_back(G);
    array_to_merge.push_back(R);
    cv::merge(array_to_merge, colour_boost);

    return colour_boost;
}
