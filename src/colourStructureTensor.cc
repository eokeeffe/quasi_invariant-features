/*
    % computes lambda1 and lambda2 of the structure tensor of a color image,
    % and computes the local orientation

    % structure tensor = ( Rx^2+Gx^2+Bx^2           Rx Ry + Gx Gy + Bx By )
    %                    ( Rx Ry + Gx Gy + Bx By    Ry^2 + Gy^2 + By^2 )
    % orientation = .5 * arctan ( 2 *  (Rx Ry + Gx Gy + Bx By) /
    %                                            (Rx^2+Gx^2+Bx^2)-(Ry^2+Gy^2+By^2) )
    % anisotropy is a confidence measure of the orientation
*/

#include <colourStructureTensor.h>
#include <gder.h>
#include <operations.h>

std::vector<cv::Mat> colourStructureTensor(cv::Mat image, float sigma, float sigma2)
{
    double eps = std::numeric_limits<double>::epsilon();
    cv::Mat R,G,B;
    split_channels(image,R,G,B);

    cv::Mat Rx, Ry,
    Gx, Gy,
    Bx, By,
    Cxx, Cxy, Cyy,
    D,lambda1,lambda2,anisotropy,orient;

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
    //phase(X,Y,out): atan2(Y,X) element-wise
    cv::phase(Cxy,Cxx-Cyy,orient);
    orient = orient * 0.5;
    anisotropy = D/(Cxx+Cyy);

    std::vector<cv::Mat> results;
    results.push_back(lambda1);
    results.push_back(lambda2);
    results.push_back(orient);
    results.push_back(anisotropy);

    return results;
}

std::vector<cv::Mat> colourStructureTensor2(cv::Mat Cxx, cv::Mat Cxy, cv::Mat Cyy)
{
    /*
    % computes lambda1 and lambda2 of color structure tensor
    % color structure tensor = ( Rx^2+Gx^2+Bx^2           Rx Ry + Gx Gy + Bx By )
    %                          ( Rx Ry + Gx Gy + Bx By    Ry^2 + Gy^2 + By^2 )
    */

    Cxy = 2 * Cxy;
    cv::Mat D;
    cv::sqrt((Cxx-Cyy).mul((Cxx-Cyy))+Cxy.mul(Cxy),D);
    cv::Mat lambda1 = Cxx+Cyy+D;
    cv::Mat lambda2 = Cxx+Cyy-D;
    cv::Mat orient;
    cv::phase(Cxy,Cxx-Cyy,orient);
    orient = orient * 0.5;

    std::vector<cv::Mat> results;
    results.push_back(lambda1);
    results.push_back(lambda2);
    results.push_back(orient);
    return results;
}
