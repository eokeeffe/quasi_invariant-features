/*
    % SYNOPSIS:
    %  [harris_energy] = ColorHarris(in,sigma_g,sigma_a,k,method)
    %
    % PARAMETERS IN:
    %   in      : input color image
    %	sigma_g : size of the Gaussian standard deviation in computing the first-derivatives
    %	sigma_a : size of the Gaussian standard deviation to define local neighborhood ( typically larger than sigma_g )
    %   k       : multiplication constant from original Harris article.
    %   method  : method=0 performs luminance based Harros
    %           : method=1 performs multi-channel Harris.
    % PARAMETERS OUT:
    %   harris_energy:  the Harris corner energy funtion.
    % LITERATURE:
    %   J. van de Weijer, Th. Gevers and A.W.M. Smeulders, Robust
    %   Photometric Invariant Features from the Color Tensor, IEEE Trans. Image Processing, vol. 15 (1), January 2006.
*/

#include <colorHarris.h>

cv::Mat colorHarris(cv::Mat in, double sigma_g, double sigma_a, double k, int method)
{
    /*
    *   sigma_g=1
    *   sigma_a=3
    *   k=0.04
    */

    cv::Mat R,G,B,colour_planes[3];
    split(image,colour_planes);  // planes[2] is the red channel

    R = colour_planes[2];
    G = colour_planes[1];
    B = colour_planes[0];

    cv::Mat Lx,Ly;
    cv::Mat Lx2,Ly2,LxLy,Lx_2,Ly_2,Lx_3,Ly_3;

    if(method==0)
    {//luminance based compute derivatives
        Lx = gder(B,sigma_g,1,0);
        Ly = gder(B,sigma_g,0,1);
    }
    if(method==1)
    {//if colour based compute derivatives
        Lx  =gDer(R,sigma_g,1,0);
        Ly  =gDer(R,sigma_g,0,1);
        Lx_2=gDer(G,sigma_g,1,0);
        Ly_2=gDer(G,sigma_g,0,1);
        Lx_3=gDer(B,sigma_g,1,0);
        Ly_3=gDer(B,sigma_g,0,1);
    }

    Lx2 = Lx.mul(Lx);
    Ly2 = Ly.mul(Ly);
    LxLy = Lx.mul(Ly);

    if(method==1)
    {//in case of colour image
        Lx2 =   Lx2 + Lx_2.mul(Lx_2) + Lx_3.mul(Lx_3);
        Ly2 =   Ly2 + Ly_2.mul(Lx_2) + Ly_3.mul(Lx_3);
        LxLy=   LxLy+ Lx_2.mul(Ly_2)+Lx_3.mul(Ly_3);
    }

    // computation of the local tensor on scale sigma_a
    Lx2 =   gDer(Lx2,sigma_a,0,0);
    Ly2 =   gDer(Ly2,sigma_a,0,0);
    LxLy=   gDer(LxLy,sigma_a,0,0);

    //Lx2.*Ly2-LxLy.^2-k*(Lx2+Ly2).^2;
    return Lx2.mul(Ly2)-LxLy.mul(LxLy) - k*((Lx2+Ly2).mul(Lx2+ly2));
}
