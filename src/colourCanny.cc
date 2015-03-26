/*
    % color canny: computes the edges (given a photometric model) of a color image
    %
    % SYNOPSIS:
    %    [out]=color_canny(input_im, sigma1, sigma2, method)
    %
    % INPUT :
    %   input_im      : color input image (NxMx3)
    %	sigma1        : size of the Gaussian standard deviation in computing the derivatives
    %	sigma2        : size of the Gaussian averaging of the color tensor
    %   method        : the color canny is computed based on:
    %                                           0 - color gradient
    %                                           1 - shadow-shading quasi-invariant
    %                                           2 - specular quasi-invariant
    %                                           3 - specular-shadow-shading quasi-invariant
    % OUTPUT:
    %   out                                 :  canny edges

    % LITERATURE:
    % J. van de Weijer, Th. Gevers, A.W.M Smeulders
    % " Robust Photometric Invariant Features from the Color Tensor"
    % IEEE Trans. Image Processing,
    % vol. 15 (1), January 2006.
*/

#include <colourCanny.h>
#include <colourStructureTensor.h>
#include <spherical_der.h>
#include <opponent_der.h>
#include <HSI_der.h>
#include <gder.h>

cv::Mat colourCanny(cv::Mat in, double sigma1, double sigma2,int method)
{
    double eps = std::numeric_limits<double>::epsilon();
    cv::Mat lambda1,
        out=cv::Mat::zeros(in.rows,in.cols,CV_32F),
        orientation;

    if(method==0)
    {
        std::vector<cv::Mat> cst = colourStructureTensor(in,sigma1,sigma2);
        lambda1 = cst.at(0);
        orientation = cst.at(2);
    }
    if(method==1)
    {
        //quasi-invariants
        std::vector<cv::Mat> sders = spherical_der(in, sigma1);
        cv::Mat Lxy = sders.at(0).mul(sders.at(1)+sders.at(2)).mul(sders.at(3));
        cv::Mat Lxx = sders.at(0).mul(sders.at(0)+sders.at(2)).mul(sders.at(2));
        cv::Mat Lyy = sders.at(1).mul(sders.at(1)+sders.at(3)).mul(sders.at(3));
        Lxx = gder(Lxx,sigma2,0,0);
        Lxy = gder(Lxy,sigma2,0,0);
        Lyy = gder(Lyy,sigma2,0,0);
        std::vector<cv::Mat> cst2 = colourStructureTensor2(Lxx,Lxy,Lyy);
        lambda1 = cst2.at(0);
        orientation = cst2.at(2);
    }
    if(method==2)
    {
        //specular-invariants
        std::vector<cv::Mat> oders = opponent_der(in, sigma1);
        cv::Mat Lxy=(oders.at(0).mul(oders.at(0)+oders.at(2)).mul(oders.at(3)));
        cv::Mat Lxx=(oders.at(0).mul(oders.at(0)+oders.at(2)).mul(oders.at(2)));
        cv::Mat Lyy=(oders.at(1).mul(oders.at(1)+oders.at(3)).mul(oders.at(3)));
        Lxx= gder(Lxx,sigma2,0,0);
        Lxy= gder(Lxy,sigma2,0,0);
        Lyy= gder(Lyy,sigma2,0,0);
        std::vector<cv::Mat> cst2 = colourStructureTensor2(Lxx,Lxy,Lyy);
        lambda1 = cst2.at(0);
        orientation = cst2.at(2);
    }
    if(method==3)
    {
        //specular-shadow-shading invariant
        std::vector<cv::Mat> hsiders = HSI_der(in, sigma1);
        cv::Mat Lxy = hsiders.at(0).mul(hsiders.at(1));
        cv::Mat Lxx = hsiders.at(0).mul(hsiders.at(0));
        cv::Mat Lyy = hsiders.at(1).mul(hsiders.at(1));
        Lxx = gder(Lxx,sigma2,0,0);
        Lxy = gder(Lxy,sigma2,0,0);
        Lyy = gder(Lyy,sigma2,0,0);
        std::vector<cv::Mat> cst2 = colourStructureTensor2(Lxx,Lxy,Lyy);
        lambda1 = cst2.at(0);
        orientation = cst2.at(2);
    }

    cv::sqrt(lambda1+eps,lambda1);

    int width  = out.cols;
    int height = out.rows;
    int dx=0,dy=0;

    for(int jj= 2; jj < width-1; jj++)
    {
        for(int ii = 2; ii < height-1; ii++)
        {
            float orient = orientation.at<float>(ii+(jj-1)*height);
            float grad   = lambda1.at<float>(ii+(jj-1)*height);
            if(grad>0)
            {
                if(abs(orient)<=0.3927)
                {
                    dx=1; dy=0;
                }
                if( (orient<1.1781 ) & (orient>0.3927) )
                {
                    dx=1; dy=1;
                }
                if( (orient>-1.1781 ) & (orient<-0.3927) )
                {
                    dx=1; dy=-1;
                }
                if( abs(orient)>=1.1781)
                {
                    dx=0; dy=-1;
                }
    		    if( ( grad >= lambda1.at<float>( ii+dy+(jj+dx-1)*height) ) && ( grad >= lambda1.at<float>( ii-dy+(jj-dx-1)*height) ))
                {
                    out.at<float>(ii+jj*height)=grad;
                }
            }
        }
    }

    return out;
}
