/**
* Implemented by Evan O'Keeffe
*
*/

#include <main.h>

using namespace std;
using namespace cv;

void splitChannels(cv::Mat &input)
{
    cv::Mat R,G,B,array[3];
    cv::split(input,array);
    R = array[2];
    G = array[1];
    B = array[0];

    imwrite("regular.jpg",input);
    imwrite("red.jpg",R);
    imwrite("green.jpg",G);
    imwrite("blue.jpg",B);
}

void enhanceImage(cv::Mat &image)
{
    /*Opencv 3.0*/
    cv::Mat imout;
    //detailEnhance(image,imout);
	//imwrite("enhanced.jpg", imout);
}

int main(int argc, char** argv)
{
    cv::Mat img = imread(std::string(argv[1]));
    double sigma=0.5;
    if(argc>2){sigma = atof(argv[2]);}
    img.convertTo(img, CV_32F);

    cv::Mat ss_var,ss_inv,spqi_var,spqi_inv,sp_ssqi_var,sp_ssqi_inv;
    double eps = std::numeric_limits<double>::epsilon();

    std::cout << "Read Image and converted to floating point" << std::endl;

    cv::Mat mboost = BoostMatrix(img,1,sigma);
    std::cout <<"mboost=" << mboost << std::endl;

    cv::Mat boosted_image = applyBoostMatrix(img,mboost);
    cv::imwrite("boosted_image.jpg",boosted_image);
    std::cout<<"Boosted saved"<<std::endl;

    std::vector<cv::Mat> sph_imgs = spherical_der(img,sigma);
    cv::sqrt(sph_imgs.at(0).mul(sph_imgs.at(0)) + sph_imgs.at(1).mul(sph_imgs.at(1))+
    sph_imgs.at(2).mul(sph_imgs.at(2)) + sph_imgs.at(3).mul(sph_imgs.at(3))+eps,ss_inv);
    cv::sqrt(sph_imgs.at(4).mul(sph_imgs.at(4))+sph_imgs.at(5).mul(sph_imgs.at(5))+eps,ss_var);

    cv::imwrite("spherical_var.jpg",ss_var);
    cv::imwrite("spherical_invar.jpg",ss_inv);
    std::cout<<"Spherical Derivations saved"<<std::endl;

    std::vector<cv::Mat> opp_imgs = opponent_der(img,sigma);
    cv::sqrt(opp_imgs.at(0).mul(opp_imgs.at(0))+opp_imgs.at(1).mul(opp_imgs.at(1))+opp_imgs.at(2).mul(opp_imgs.at(2))+
    opp_imgs.at(3).mul(opp_imgs.at(3))+eps,spqi_inv);
    cv::sqrt(opp_imgs.at(4).mul(opp_imgs.at(4))+opp_imgs.at(5).mul(opp_imgs.at(5))+eps,spqi_var);

    cv::imwrite("opponent_var.jpg",spqi_var);
    cv::imwrite("opponent_invar.jpg",spqi_inv);
    std::cout<<"Opp Derivations saved"<<std::endl;

    cv::Mat img_hsv;
    cv::cvtColor(img,img_hsv,CV_RGB2HSV);

    std::vector<cv::Mat> hsi_imgs = HSI_der(img_hsv,sigma);
    cv::sqrt(hsi_imgs.at(0).mul(hsi_imgs.at(0))+hsi_imgs.at(1).mul(hsi_imgs.at(1))+eps,sp_ssqi_var);
    cv::sqrt(hsi_imgs.at(4).mul(hsi_imgs.at(4))+hsi_imgs.at(5).mul(hsi_imgs.at(5))+
    hsi_imgs.at(2).mul(hsi_imgs.at(2))+hsi_imgs.at(3).mul(hsi_imgs.at(3))+eps,sp_ssqi_inv);

    cv::imwrite("hsi_var.jpg",sp_ssqi_var);
    cv::imwrite("hsi_invar.jpg",sp_ssqi_inv);
    std::cout<<"HSI Derivations saved"<<std::endl;

    cv::Mat harris_reg = colourHarris(img,1,3,0.04,1);
    cv::imwrite("harris.jpg",harris_reg);
    std::cout<<"Colour Harris saved"<<std::endl;

    cv::Mat canny_reg = colourCanny(img,3,0.04,2);
    cv::imwrite("canny.jpg",canny_reg);
    std::cout<<"Colour Canny saved"<<std::endl;

    return 0;
}
