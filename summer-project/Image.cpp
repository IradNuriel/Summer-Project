#include "Image.h"

//c'tor
Image::Image(const int id, const cv::Mat & img, const cv::Ptr<cv::ORB>& detector){
	this->img = img;
	this->lb = LineBuilder(id);
	if (detector) this->init(detector);
}

//init image features
void Image::init(const cv::Ptr<cv::ORB>& detector){
	detector->detectAndCompute(this->img, cv::noArray(), this->key, this->desc);
}
