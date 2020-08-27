#include "Cloud.h"

Cloud::Cloud(const std::vector<cv::Vec3d> p) {
	this->points = p;
}

Cloud::insert(const Vec3d point)
{
	points.push_back(point);
}

Vec3d Cloud::average() {
	Vec3d res;
	for (auto point : this->points) res += point;
	return res / int(this->points.size());
}