/*
    Colour Structure Tensor
    For OpenCV
*/

#include <>

void colourStructureTensor(cv::Mat image, float sigma1, float sigma2)
{
    cv::Mat R,G,B,colour_planes[3];
    split(image,colour_planes);  // planes[2] is the red channel

    R = colour_planes[2];
    G = colour_planes[1];
    B = colour_planes[0];

    cv::Mat Rx, Ry, Gx, Gy, Bx, By, Cxx, Cxy, Cyy;

    sepFilter2D (R, Rx, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);
    sepFilter2D (R, Ry, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);
    sepFilter2D (R, R, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);

    sepFilter2D (G, Gx, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);
    sepFilter2D (G, Gy, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);
    sepFilter2D (G, G, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);

    sepFilter2D (B, Bx, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);
    sepFilter2D (B, By, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);
    sepFilter2D (B, B, -1, dygauss_kernelx (sigma), dygauss_kernely (sigma), Point (-1, -1), 0, BORDER_REPLICATE);

    Cxy = 2*(Rx*Ry+Gx*Gy+Bx*By);
    Cxx = Rx*Rx+Gx*Gx+Bx*Bx;
    Cyy = Ry*Ry+Gy*Gy+By*By;

    cv::Mat D = sqrt(((Cxx-Cyy)*(Cxx-Cyy) + (Cxy*Cxy) + EPS);
    cv::Mat lambda1 = Cxx+Cyy+D;
    cv::Mat lambda2 = Cxx+Cyy-D;
    orient = 0.5 * atan2(Cxy,Cxx-Cyy);
    anisotropy = D/(Cxx+Cyy);
}
