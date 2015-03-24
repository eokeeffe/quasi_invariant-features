/*function [x_max,y_max,mask,num_max]=getmaxpoints(in,nPoints)
% returns the nPoints maxima of an image
%
% SYNOPSIS:
%  [x_max,y_max,mask,num_max]=getmaxpoints(in,nPoints,sigma)
%
% PARAMETERS:
%   in      : input image
%   nPoints : number maxima to return

mask=zeros(size(in));
x_max=zeros(nPoints,1);
y_max=zeros(nPoints,1);
[size_y size_x]=size(in);
ii=1;
num_max=0;

copyIn=dilation33(dilation33(in));
copyIn=copyIn.*(in==copyIn);

while(ii<=nPoints)
 [max_y ypos] =max(copyIn);
 [max_xy xpos]=max(max_y);
 x_max(ii)=xpos(1);
 y_max(ii)=ypos(xpos(1));

 copyIn(y_max(ii),x_max(ii))=0;
 mask(y_max(ii),x_max(ii))=1;

 if(sum(copyIn(:))==0) num_max=ii;ii=nPoints+1;end
 ii=ii+1;
end
if(num_max==0) num_max=nPoints;end
*/

maxpoints getmaxpoints(cv::Mat in,int nPoints,double sigma)
{
    cv::Mat mask(in.rows,in.cols,CV_64F),
        x_max(nPoints,1,CV_64F),y_max(nPoints,1,CV_64F);
    int num_max=0;

    //MORPH_RECT,MORPH_CROSS,MORPH_ELLIPSE
    int dilation_size = 0;
    cv::Mat copyIn = getStructuringElement(MORPH_RECT,
        cv::Size(2*dilation_size + 1, 2*dilation_size+1),
        cv::Point(dilation_size,dilation_size));

    for(int i=0;i<copyIn.size()[0];i++)
    {
        if(in.at<double>(i) != copyIn.at<double>(i))
        {copyIn.at<double>(i) = 0.0;}
    }

    double minVal,maxVal;
    cv::Point minLoc,maxLoc;
    for(int ii=0;ii<=nPoints;ii++)
    {
        minMaxLoc( copyIn, &minVal, &maxVal, &minLoc, &maxLoc );
    }


    return maxpoints();
}
