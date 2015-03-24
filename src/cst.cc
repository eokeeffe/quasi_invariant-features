/*
    % computes lambda1 and lambda2 of the structure tensor of a color image,
    % and computes the local orientation

    % structure tensor = ( Rx^2+Gx^2+Bx^2           Rx Ry + Gx Gy + Bx By )
    %                    ( Rx Ry + Gx Gy + Bx By    Ry^2 + Gy^2 + By^2 )
    % orientation = .5 * arctan ( 2 *  (Rx Ry + Gx Gy + Bx By) /
    %                                            (Rx^2+Gx^2+Bx^2)-(Ry^2+Gy^2+By^2) )
    % anisotropy is a confidence measure of the orientation
*/

#include <colorStructureTensor.h>
#include <gder.h>


void colorStructureTensor(cv::Mat image, float sigma1, float sigma2)
{
    cv::Mat R,G,B,colour_planes[3];
    split(image,colour_planes);  // planes[2] is the red channel

    R = colour_planes[2];
    G = colour_planes[1];
    B = colour_planes[0];

    cv::Mat Rx, Ry,
    Gx, Gy,
    Bx, By,
    Cxx, Cxy, Cyy
    D,lambda1,lambda2,anisotropy;

    Rx = gder(R,sigma,1,0);
    Ry = gder(R,sigma,0,1);
    R  = gder(R,sigma,0,0);

    Gx = gder(G,sigma,1,0);
    Gy = gder(G,sigma,0,1);
    G  = gder(G,sigma,0,0);

    Bx = gder(B,sigma,1,0);
    By = gder(B,sigma,0,1);
    B  = gder(B,sigma,0,0);

    Cxy = 2 * (Rx.mul(Ry)+Gx.mul(Gy)+Bx.mul(By));
    Cxx = Rx.mul(Rx)+Gx.mul(Gx)+Bx.mul(Bx);
    Cyy = Ry.mul(Ry)+Gy.mul(Gy)+By.mul(By);

    Cxx = gder(Cxx,sigma2,0,0);
    Cxy = gder(Cxy,sigma2,0,0);
    Cyy = gder(Cyy,sigma2,0,0);

    cv::sqrt((Cxx-Cyy).mul((Cxx-Cyy))+Cxy.mul(Cxy)+eps,D);
    lambda1=Cxx+Cyy+D;
    lambda2=Cxx+Cyy-D;
    orient = 0.5*atan2(Cxy,Cxx-Cyy);
    anisotropy=D./(Cxx+Cyy);

    std::vector<cv::Mat> results;
    results.push_back(lambda1);
    results.push_back(lambda2);
    results.push_back();
    results.push_back(anisotropy);
}
