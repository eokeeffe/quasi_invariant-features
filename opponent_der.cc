/*
    % opponent derivatives: used to compute the specular quasi-invariants (SPQI)
    %
    % SYNOPSIS:
    %  [O1_x, O1_y, O2_x, O2_y, O3_x, O3_y] = opponent_der(input_im, sigma)
    %
    % INPUT :
    %   input_im : color input image (NxMx3)
    %	sigma    : size of the Gaussian standard deviation in computing the derivatives
    %
    % OUTPUT:
    %   O1_x, O1_y, O2_x, O2_y  :  opponent derivatives in the specular INVARIANT direction.
    %   O3_x, O3_y              :  opponent derivatives in the specular VARIANT direction.

    % LITERATURE:
    % J. van de Weijer, Th. Gevers, J-M Geusebroek
    % "Edge and Corner Detection by Photometric Quasi-Invariants"
    % IEEE Trans. Pattern Analysis and Machine Intelligence,
    % vol. 27 (4), April 2005.
    Created by Evan O'Keeffe
*/

#include "opponent_der.h"
#include "gder.h"

std::vector<cv::Mat> opponent_der(cv::Mat input,double sigma)
{
    double eps = std::numeric_limits<double>::epsilon();
    cv::Mat R,G,B,colour_planes[3];
    split(input,colour_planes);  // planes[2] is the red channel
    R = colour_planes[2];
    G = colour_planes[1];
    B = colour_planes[0];

    cv::Mat Rx,Ry,Gx,Gy,Bx,By,
    O1_x,O1_y,
    O2_x,O2_y,
    O3_x,O3_y,
    ss_inv,ss_var;

    //computation of spatial derivatives
    Rx=gder(R,sigma,1,0);
    Ry=gder(R,sigma,0,1);

    Gx=gder(G,sigma,1,0);
    Gy=gder(G,sigma,0,1);

    Bx=gder(B,sigma,1,0);
    By=gder(B,sigma,0,1);

    // computation of derivatives in opponent color space
    O1_x = (Rx-Gx)/sqrt(2);
    O1_y = (Ry-Gy)/sqrt(2);
    O2_x = (Rx+Gx-2*Bx)/sqrt(6);
    O2_y = (Ry+Gy-2*By)/sqrt(6);
    O3_x = (Rx+Gx+Bx)/sqrt(3);
    O3_y = (Ry+Gy+By)/sqrt(3);

    sqrt(O1_x.mul(O1_x)+O1_y.mul(O1_y)+O2_x.mul(O2_x)+O2_y.mul(O2_y)+eps,ss_inv);
    sqrt(O3_x.mul(O3_x)+O3_y.mul(O3_y)+eps,ss_var);

    std::vector<cv::Mat> ss;
    ss.push_back(ss_inv);
    ss.push_back(ss_var);
    return ss;
}
