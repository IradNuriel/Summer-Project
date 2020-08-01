#include "image.h"

Image::Image(const cv::Mat & img, const cv::Vec3d & pos, const cv::Ptr<cv::ORB>& detector){
	this->img = img;
	this->pos = pos;
	this->lb = LineBuilder({ 1.0*img.size[0], 1.0*img.size[1] });
	if (detector) this->init(detector);
}

void Image::init(const cv::Ptr<cv::ORB>& detector){
	detector->detectAndCompute(this->img, cv::noArray(), this->key, this->desc);
}
