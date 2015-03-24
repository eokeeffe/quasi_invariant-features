/*
    % hue-saturation-intensity derivatives: used to compute the specular shadow-shading quasi-invariants (SP-SSQI)
    %
    % SYNOPSIS:
    %   [[h_x, h_y, s_x, s_y, i_x, i_y, saturation] = HSI_der( input_im, sigma)
    %
    % INPUT :
    %   input_im        : color input image (NxMx3)
    %	sigma           : size of the Gaussian standard deviation in computing the derivatives
    %
    % OUTPUT:
    %   h_x, h_y                     :  derivatives in the speclar-shadow-shading INVARIANT direction.
    %                                   (note that h_x= saturation * hue_x)
    %   s_x, s_y, i_x, i_y           :  spherical derivatives in the specular-shadow-shading VARIANT directions.

    % LITERATURE:
    % J. van de Weijer, Th. Gevers, J-M Geusebroek
    % "Edge and Corner Detection by Photometric Quasi-Invariants"
    % IEEE Trans. Pattern Analysis and Machine Intelligence,
    % vol. 27 (4), April 2005.

    Created by Evan O'Keeffe
*/
#include "HSI_der.h"
#include "gder.h"

std::vector<cv::Mat> HSI_der(cv::Mat input,double sigma)
{
    double eps = std::numeric_limits<double>::epsilon();
    cv::Mat R,G,B,colour_planes[3];
    split(input,colour_planes);  // planes[2] is the red channel
    R = colour_planes[2];
    G = colour_planes[1];
    B = colour_planes[0];

    cv::Mat Rx,Ry,Gx,Gy,Bx,By,
    saturation,
    h_x,s_x,i_x,
    h_y,s_y,i_y,
    ss_inv,ss_var;

    //compute spatial derivatives
    Rx = gder(R,sigma,1,0);
    Ry = gder(R,sigma,0,1);
    R  = gder(R,sigma,0,0);

    Gx = gder(G,sigma,1,0);
    Gy = gder(G,sigma,0,1);
    G  = gder(G,sigma,0,0);

    Bx = gder(B,sigma,1,0);
    By = gder(B,sigma,0,1);
    B  = gder(B,sigma,0,0);

    //hsi derivatives- hue saturation intensity
    cv::sqrt(2*(R.mul(R)+G.mul(G)+B.mul(B)-R.mul(G)-R.mul(B)-G.mul(B)+eps),saturation);
    h_x = (R.mul((Bx-Gx))+G.mul((Rx-Bx))+B.mul((Gx-Rx)))/saturation;
    s_x = (R.mul((2*Rx-Gx-Bx))+G.mul((2*Gx-Rx-Bx))+B.mul((2*Bx-Rx-Gx)))/(sqrt(3)*saturation);
    i_x = 1/sqrt(3)*(Rx+Gx+Bx);

    h_y = (R.mul((By-Gy))+G.mul((Ry-By))+B.mul((Gy-Ry)))/saturation;
    s_y = (R.mul((2*Ry-Gy-By))+G.mul((2*Gy-Ry-By))+B.mul((2*By-Ry-Gy)))/(sqrt(3)*saturation);
    i_y = 1/sqrt(3)*(Ry+Gy+By);

    saturation=saturation/sqrt(3);

    cv::sqrt(h_x.mul(h_x)+h_y.mul(h_y)+eps,ss_inv);
    cv::sqrt(i_x.mul(i_x)+i_y.mul(i_y)+s_x.mul(s_x)+s_y.mul(s_y)+eps,ss_var);

    std::vector<cv::Mat> ss;
    ss.push_back(ss_inv);
    ss.push_back(ss_var);
    return ss;
}
