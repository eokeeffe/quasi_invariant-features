/**
** Implemented by Evan O'Keeffe
**
*/

#include "main.h"

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
        rx = gder(image,sigma,1,0);
        ry = gder(image,sigma,0,1);
        cv::filter2D(R, R, R.depth() , rx, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
        cv::filter2D(R, R, R.depth() , ry, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);

        gx = gder(image,sigma,1,0);
        gy = gder(image,sigma,0,1);
        cv::filter2D(R, R, R.depth() , gx, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
        cv::filter2D(R, R, R.depth() , gy, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);

        bx = gder(image,sigma,1,0);
        by = gder(image,sigma,0,1);
        cv::filter2D(R, R, R.depth() , bx, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
        cv::filter2D(R, R, R.depth() , by, cv::Point(-1,-1), 0.0, cv::BORDER_DEFAULT);
    }

    cv::Mat M(3,3,CV_64F),W,U,V,S(3,3,CV_64F);

    M.at<float>(0,0) = cv::sum(R.mul(R))[0];
    M.at<float>(0,1) = cv::sum(R.mul(G))[0];
    M.at<float>(0,2) = cv::sum(R.mul(B))[0];
    M.at<float>(1,1) = cv::sum(G.mul(G))[0];
    M.at<float>(1,2) = cv::sum(G.mul(B))[0];
    M.at<float>(2,2) = cv::sum(B.mul(B))[0];

    M.at<float>(1,0) = M.at<float>(0,1);
    M.at<float>(2,0) = M.at<float>(0,2);
    M.at<float>(2,1) = M.at<float>(1,2);

    M = M / (image.rows * image.cols);
    //M compared to Matlab results and kinda working
    //error margin exists due to rounding errors in opencv/c++ versus Matlab
    //heavy customization required to reduce

    SVD::compute(M,W,U,V);
    //Put values of W onto Diagonal of S
    S.at<double>(0,0) = W.at<double>(0);
    S.at<double>(1,1) = W.at<double>(1);
    S.at<double>(2,2) = W.at<double>(2);

    //mboost = U * (diag(1./diag(sqrt(S)))*V');
    cv::sqrt(S,S);
    S = 1./S;
    return U*(S)*V.t();
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
    array_to_merge.push_back(R);
    array_to_merge.push_back(G);
    array_to_merge.push_back(B);
    cv::merge(array_to_merge, colour_boost);

    return colour_boost;
}

int main(int argc, char** argv)
{
    cv::Mat img = imread(std::string(argv[1]));
    img.convertTo(img, CV_64FC3);

    std::cout << "Read Image and converted to double" << std::endl;

    cv::Mat mboost = BoostMatrix(img,1,0.5);

    std::cout << mboost << std::endl;

    std::cout << "Applying Boost Matrix" << std::endl;

    cv::Mat boosted_image = applyBoostMatrix(img,mboost);

    cv::imwrite("boosted_image.jpg",boosted_image);

    return 0;
}
