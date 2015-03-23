/*
*    Creating Gaussian Derivatives for images
*/

#include "gder.h"

cv::Mat fill_border(cv::Mat in, int bw)
{
    int hh = in.rows;
    int ww = in.cols;
    int dd = in.depth();

    if(dd == 1)
    {

        cv::Mat out = cv::Mat::zeros(hh+bw*2,ww+bw*2,CV_32F);
        /*
        out(1:bw,1:bw) = cv::Mat::ones(bw,bw).mul(in(1,1));
        out(bw+hh+1:2*bw+hh,1:bw) = cv::Mat::ones(bw,bw).mul(in(hh,1));
        out(1:bw,bw+1+ww:2*bw+ww) = cv::Mat::ones(bw,bw).mul(in(1,ww));
        out(bw+hh+1:2*bw+hh,bw+1+ww:2*bw+ww) = cv::Mat::ones(bw,bw).mul(in(hh,ww));
        out( bw+1:bw+hh,bw+1:bw+ww ) = in;
        out(1:bw,bw+1:bw+ww) = cv::Mat::ones(bw,1).mul(in(1,:));
        out(bw+hh+1:2*bw+hh,bw+1:bw+ww) = cv::Mat::ones(bw,1).mul(in(hh,:));
        out(bw+1:bw+hh,1:bw) = in(:,1).mul(cv::Mat::ones(1,bw));
        out(bw+1:bw+hh,bw+ww+1:2*bw+ww) = in(:,ww).mul(cv::Mat::ones(1,bw));
        */
        return out;
    }
    else
    {
        cv::Mat out = cv::Mat::zeros(hh+bw*2,ww+bw*2,CV_32F);
        /*
        for(int ii=1:ii<dd;i++)
        {
        	out(1:bw,1:bw,ii) = cv::Mat::ones(bw,bw).mul(in(1,1,ii));
    		out(bw+hh+1:2*bw+hh,1:bw,ii) = cv::Mat::ones(bw,bw).mul(in(hh,1,ii));
    		out(1:bw,bw+1+ww:2*bw+ww,ii) = cv::Mat::ones(bw,bw).mul(in(1,ww,ii));
    		out(bw+hh+1:2*bw+hh,bw+1+ww:2*bw+ww,ii) = cv::Mat::ones(bw,bw).mul(in(hh,ww,ii));
    		out( bw+1:bw+hh,bw+1:bw+ww,ii ) = in(:,:,ii);
    		out(1:bw,bw+1:bw+ww,ii) = cv::Mat::ones(bw,1).mul(in(1,:,ii));
    		out(bw+hh+1:2*bw+hh,bw+1:bw+ww,ii) = cv::Mat::ones(bw,1).mul(in(hh,:,ii));
    		out(bw+1:bw+hh,1:bw,ii) = in(:,1,ii).mul(cv::Mat::ones(1,bw));
    		out(bw+1:bw+hh,bw+ww+1:2*bw+ww,ii) = in(:,ww,ii).mul(cv::Mat::ones(1,bw));
        }
        */
        return out;
    }
}

cv::Mat gder(cv::Mat f, float sigma, int iorder, int jorder)
{
    float break_off_sigma = 3.0;
    float filtersize = floor(break_off_sigma*sigma+0.5);

    //f = fill_border(f,filtersize);

    int counter=0;
    for(int i=-filtersize;i<=filtersize;i++){counter++;}

    cv::Mat x(1,counter,CV_32F),Gauss(1,counter,CV_32F);
    //verified working against results from Matlab
    for(int i=-filtersize,counter=0;i<=filtersize;i++,counter++)
    {
        x.at<float>(counter) = i;
    }
    //verfied working against results from Matlab
    for(int i=0;i<x.total();i++)
    {
        Gauss.at<float>(i) = (1/(sqrt(2*M_PI)*sigma)*
            exp((x.at<float>(i)*x.at<float>(i))/(-2*sigma*sigma)));
    }

    cv::Mat Gx,Gy,H;
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
            Gx = Gx / (cv::sum(cv::sum(x.mul(Gx))))[0];
            break;
        }
        case 2:
        {
            Gx = (( x.mul(x)/pow(sigma,4) ) - (1/pow(sigma,2) )).mul(Gauss);
            Gx = Gx-cv::sum(Gx)[0]/x.cols;

            cv::Mat x2 = x.mul(x);
            cv::multiply(x2,Gx,Gx,1,-1);
            double sum=0.0;
            for(int i=0;i<Gx.total();i++){sum+=Gx.at<float>(i);}
            Gx = Gx/sum;
            break;
        }
    }
    cv::filter2D(f,H,f.depth(),Gx,cv::Point(-1,-1),0.0,cv::BORDER_DEFAULT);

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
            Gy = Gy / cv::sum(x.mul(Gy))[0];
            break;
        }
        case 2:
        {
            Gy = ((x.mul(x) / pow(sigma,4)) - (1/pow(sigma,2)) ).mul(Gauss);
            Gy = Gy-cv::sum(Gy)[0]/x.cols;

            cv::Mat x2 = x.mul(x);
            cv::multiply(x2,Gy,Gy,1,-1);
            float sum=0.0;
            for(int i=0;i<Gy.total();i++){sum+=Gy.at<float>(i);}
            Gy = Gy/sum;
            break;
        }
    }
    cv::filter2D(f,H,f.depth(),Gy.t(),cv::Point(-1,-1),0.0,cv::BORDER_DEFAULT);

    return H;
}
