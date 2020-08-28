#include "Image.h"

Image::Image(const int id, const cv::Mat & img, const cv::Ptr<cv::ORB>& detector){
	this->img = img;
	this->lb = LineBuilder(id);
	this->pos = this->lb.getPos();
	if (detector) this->init(detector);
}

void Image::init(const cv::Ptr<cv::ORB>& detector){
	detector->detectAndCompute(this->img, cv::noArray(), this->key, this->desc);
}
