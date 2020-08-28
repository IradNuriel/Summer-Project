#include "Cloud.h"

Cloud::Cloud(const std::vector<cv::Vec3d> p) {
	this->points.resize(p.size());
	this->points.insert(this->points.begin(), p.begin(), p.end());
}


Cloud::Cloud() {
	this->points = std::vector<cv::Vec3d>();
}


void Cloud::insert(const cv::Vec3d point){
	points.push_back(point);
}

cv::Vec3d Cloud::average() {
	cv::Vec3d res;
	for (auto point : this->points) res += point;
	return res / int(this->points.size());
}