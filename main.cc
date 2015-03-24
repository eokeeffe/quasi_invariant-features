/**
* Implemented by Evan O'Keeffe
*
*/

#include "main.h"

int main(int argc, char** argv)
{
    cv::Mat img = imread(std::string(argv[1]));
    img.convertTo(img, CV_64FC3);

    std::cout << "Read Image and converted to double" << std::endl;

    cv::Mat mboost = BoostMatrix(img,1,0.5);

    std::cout << mboost << std::endl;

    //std::cout << "Applying Boost Matrix" << std::endl;

    cv::Mat boosted_image = applyBoostMatrix(img,mboost);

    cv::imwrite("boosted_image.jpg",boosted_image);

    return 0;
}
