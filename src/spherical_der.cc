/*
    % spherical derivatives: used to compute the shadow-shading quasi-invariants (SSQI)
    %
    % SYNOPSIS:
    %   [ theta_x, theta_y, phi_x, phi_y, r_x, r_y, intensityL2] = spherical_der(input_im, sigma)
    %
    % INPUT :
    %   input_im     : color input image (NxMx3)
    %	sigma        : size of the Gaussian standard deviation in computing the derivatives
    %
    % OUTPUT:
    %   theta_x, theta_y, phi_x, phi_y      :  spherical derivatives in the shadow-shading INVARIANT direction.
    %                                          (note that the Jacobian is taken into account theta_x = r*sin(phi)*THETA_x
    %                                           and phi_x = r* PHI_x, where THETA_x and PHI_x are the true spatial
    %                                           derivatives of THETA and PHI)
    %   r_x, r_y                            :  spherical derivatives in the shadow-shading VARIANT direction.

    % LITERATURE:
    % J. van de Weijer, Th. Gevers, J-M Geusebroek
    % "Edge and Corner Detection by Photometric Quasi-Invariants"
    % IEEE Trans. Pattern Analysis and Machine Intelligence,
    % vol. 27 (4), April 2005.

    Created by Evan O'Keeffe
*/

#include <spherical_der.h>
#include <gder.h>
#include <operations.h>

std::vector<cv::Mat> spherical_der(cv::Mat input, double sigma)
{
    double eps = std::numeric_limits<double>::epsilon();

    //split the colour channels
    cv::Mat R(input.rows,input.cols,CV_64FC1),
            G(input.rows,input.cols,CV_64FC1),
            B(input.rows,input.cols,CV_64FC1),
            colour_planes[3];
    cv::Mat Rx,Ry,
    Gx,Gy,
    Bx,By,
    intensityL2,I2,
    theta_x,phi_x,r_x,
    theta_y,phi_y,r_y;

    split_channels(input,R,G,B);

    //computation of spatial derivatives
    Rx=gder(R,sigma,1,0);
    Ry=gder(R,sigma,0,1);
    R =gder(R,sigma,0,0);

    Gx=gder(G,sigma,1,0);
    Gy=gder(G,sigma,0,1);
    G =gder(G,sigma,0,0);

    Bx=gder(B,sigma,1,0);
    By=gder(B,sigma,0,1);
    B =gder(B,sigma,0,0);

    cv::sqrt(R.mul(R)+G.mul(G)+B.mul(B)+eps,intensityL2);
    cv::sqrt(R.mul(R)+G.mul(G)+eps,I2);

    theta_x = (R.mul(Gx)-G.mul(Rx))/I2;
    phi_x   = (G.mul(B.mul(Gx)-G.mul(Bx))+R.mul(B.mul(Rx)-R.mul(Bx)))/(intensityL2.mul(I2));
    r_x     = (R.mul(Rx)+G.mul(Gx)+B.mul(Bx))/intensityL2;

    std::vector<cv::Mat> ss;
    ss.push_back(theta_x);
    ss.push_back(theta_y);
    ss.push_back(phi_x);
    ss.push_back(phi_y);
    ss.push_back(r_x);
    ss.push_back(r_y);
    ss.push_back(intensityL2);
    return ss;
}
