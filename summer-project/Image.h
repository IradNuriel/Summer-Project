/////////////////////////////////
// file written by Noam Licht, //
// written in 02/08/2020       //
/////////////////////////////////
#ifndef _IMAGE_H_
#define _IMAGE_H_
#include <iostream>
#include <vector>
#include <valarray>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "LineBuilder.h"



/*class Image, store image matrix, as well as the keypoinys in it, a line builder, and position*/
class Image {
public:
	Image(const int id, const cv::Mat& img, const cv::Ptr<cv::ORB>& detector = cv::Ptr<cv::ORB>()); // c'tor
	void init(const cv::Ptr<cv::ORB>& detector);
	cv::Mat img;
	std::vector<cv::KeyPoint> key;
	cv::Mat desc;
	LineBuilder lb;
};
#endif
