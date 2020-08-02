#ifndef _MATCHER_H_
#define _MATCHER_H_
#include <iostream>
#include <vector>
#include <utility>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "Cluster.h"
#include "Image.h"


typedef std::pair<cv::KeyPoint, cv::KeyPoint> pKeyPoint;
class Matcher {
public:
	Matcher();//defsult c'tor
	Matcher(const Matcher& other);//copy c'tor
	const void init_img(Image& img);
	const std::vector<pKeyPoint> match2(const cv::Mat& img1, const cv::Mat& img2, bool draw_match=false);//function addImageToSet, add img to the data set
	const void match2(Image& img1, Image& img2, bool draw_match=false);
	const void draw(const cv::Mat& img1, const cv::Mat& img2, const std::vector<pKeyPoint>& kvec);
	const void draw(const Image& img1, const Image& img2, const std::vector<cv::DMatch>& match, std::string title="Match");
private:
	const int minHessian=1200;//number of keypoints
	cv::Ptr<cv::ORB> detector;//pointer to an ORB detector object
	cv::Ptr<cv::DescriptorMatcher> matcher;
};

#endif
