/*
    Convert R,G,B values to luminance
*/

cv::Mat RGB2luminance(cv::Mat image,double sigma)
{
    cv::Mat R,G,B,colour_planes[3];
    split(image,colour_planes);  // planes[2] is the red channel
    R = colour_planes[2];
    G = colour_planes[1];
    B = colour_planes[0];

    //l=0.299*double(f(:,:,1))+0.587*double(f(:,:,1))+0.114*double(f(:,:,3));
    return 0.299 * R + 0.587 * R + 0.114 * B;
}
