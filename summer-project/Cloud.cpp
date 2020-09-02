/////////////////////////////////
// file written by Lun Surd,   //
// written in 27/08/2020       //
/////////////////////////////////
#include "Cloud.h"

//normal c'tor
Cloud::Cloud(const std::vector<cv::Vec3d> p) {
	this->points.resize(p.size());
	this->points.insert(this->points.begin(), p.begin(), p.end());
}

//default c'tor
Cloud::Cloud() {
	this->points = std::vector<cv::Vec3d>();
}

/*insert(params),
  get a point and insert it to the structure*/
void Cloud::insert(const cv::Vec3d point){
	this->points.push_back(point);
}


/*average(),
  return the mass center of the points in the structure*/
cv::Vec3d Cloud::average() {
	cv::Vec3d res;
	for (auto point : this->points) res += point;
	return res / int(this->points.size());
}