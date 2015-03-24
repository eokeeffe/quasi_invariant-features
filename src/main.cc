/**
* Implemented by Evan O'Keeffe
*
*/

#include <main.h>

int main(int argc, char** argv)
{
    cv::Mat img = imread(std::string(argv[1]));
    img.convertTo(img, CV_64FC3);

    std::cout << "Read Image and converted to double" << std::endl;

    cv::Mat mboost = BoostMatrix(img,1,0.5);

    //std::cout << mboost << std::endl;

    //std::cout << "Applying Boost Matrix" << std::endl;

    cv::Mat boosted_image = applyBoostMatrix(img,mboost);
    std::vector<cv::Mat> sph_imgs = spherical_der(img,1.0);
    std::vector<cv::Mat> opp_imgs = opponent_der(img,1.0);
    std::vector<cv::Mat> hsi_imgs = HSI_der(img,1.0);

    cv::imwrite("boosted_image.jpg",boosted_image);

    cv::imwrite("spherical_var.jpg",sph_imgs.at(0));
    cv::imwrite("spherical_invar.jpg",sph_imgs.at(1));

    cv::imwrite("opponent_var.jpg",opp_imgs.at(0));
    cv::imwrite("opponent_invar.jpg",opp_imgs.at(1));

    cv::imwrite("hsi_var.jpg",hsi_imgs.at(0));
    cv::imwrite("hsi_invar.jpg",hsi_imgs.at(1));

    return 0;
}
