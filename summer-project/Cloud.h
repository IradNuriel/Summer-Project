/////////////////////////////////
// file written by Lun Surd,   //
// written in 27/08/2020       //
/////////////////////////////////
#ifndef _Cloud_H_
#define _Cloud_H_

#include "opencv2/core.hpp"
#include <vector>

//class Cloud, represent a heavy cluster of points
class Cloud {
public:
	//normal c'tor
	Cloud(const std::vector<cv::Vec3d> p);
	
	//default c'tor
	Cloud();

	/*insert(params),
	  get a point and insert it to the structure*/ 
	void insert(const cv::Vec3d point);

	/*average(),
	  return the mass center of the points in the structure*/
	cv::Vec3d average();
private:
	//the data.
	std::vector<cv::Vec3d> points;
};

#endif