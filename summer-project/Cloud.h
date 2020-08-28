#ifndef _Cloud_H_
#define _Cloud_H_

#include "opencv2/core.hpp"
#include <vector>

class Cloud {
public:
	Cloud(const std::vector<cv::Vec3d> p);
	Cloud();
	void insert(const cv::Vec3d point);
	cv::Vec3d average();
private:
	std::vector<cv::Vec3d> points;
};

#endif