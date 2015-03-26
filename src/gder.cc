/*
*    Creating Gaussian Derivatives for images
*/

#include <gder.h>
#include <operations.h>

cv::Mat fill_border(cv::Mat in, int bw)
{
    int hh = in.rows;
    int ww = in.cols;
    int dd = in.depth();

    if(dd == 1)
    {

    }
    else
    {

    }

    return cv::Mat::eye(hh,ww,CV_32F);
}

cv::Mat gder(cv::Mat f, double sigma, int iorder, int jorder)
{
    int counter=0;
    double break_off_sigma = 3.;
    double filtersize = floor(break_off_sigma*sigma+0.5);
    //get the apeture size of the gauss kernel
    for(int i=-filtersize;i<=filtersize;i++){counter++;}

    cv::Mat Gauss,Gx,Gy,H;

    cv::Mat x(counter,1,CV_32F);
    //verified working against results from Matlab
    for(int i=-filtersize,counter=0;i<=filtersize;i++,counter++)
    {
        x.at<float>(counter) = i;
    }

    //checked the differences, I don't know why it's there ?
    //f = fill_border(f,filtersize);

    //create standard gauss kernel
    Gauss = cv::getGaussianKernel(counter,sigma, CV_32F);

    switch(iorder)
    {
        case 0:
        {
            Gx = Gauss/cv::sum(Gauss)[0];
            break;
        }
        case 1:
        {
            Gx = -( x / (sigma*sigma) ).mul(Gauss);
            Gx = Gx / cv::sum(x.mul(Gx))[0];
            break;
        }
        case 2:
        {
            Gx = (( x.mul(x)/pow(sigma,4) ) - (1/pow(sigma,2) )).mul(Gauss);
            Gx = Gx-cv::sum(Gx)[0]/x.cols;
            Gx = Gx/cv::sum(0.5*x.mul(x).mul(Gx))[0];
            break;
        }
    }
    //cv::filter2D(f,H,f.depth(),Gx,cv::Point(-1,-1),0.0,cv::BORDER_DEFAULT);

    switch(jorder)
    {
        case 0:
        {
            Gy = Gauss/cv::sum(Gauss)[0];
            break;
        }
        case 1:
        {
            Gy = -(x/(sigma*sigma)).mul(Gauss);
            Gy = Gy/cv::sum(x.mul(Gy))[0];
            break;
        }
        case 2:
        {
            Gy = ((x.mul(x) / pow(sigma,4)) - (1/pow(sigma,2)) ).mul(Gauss);
            Gy = Gy-cv::sum(Gy)[0]/x.cols;
            Gy = Gy/cv::sum(0.5*x.mul(x).mul(Gy))[0];
            break;
        }
    }
    //cv::filter2D(f,H,f.depth(),Gy.t(),cv::Point(-1,-1),0.0,cv::BORDER_DEFAULT);

    cv::sepFilter2D(f,H,f.depth(),Gx,Gy.t(),cv::Point(-1,-1),sigma,cv::BORDER_DEFAULT);

    //H=H(filtersize+1:size(H,1)-filtersize,filtersize+1:size(H,2)-filtersize);
    return H(cv::Range(filtersize+1,H.rows-filtersize),cv::Range(filtersize+1,H.cols-filtersize));
}
