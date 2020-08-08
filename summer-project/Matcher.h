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
#include "Constants.h"

typedef std::pair<cv::KeyPoint, cv::KeyPoint> pKeyPoint;//typedef pKeyPoint, a pair of keypoints.

/*class Matcher, store a DescriptorMatcher and detector.*/
class Matcher {
public:
	Matcher();//defsult c'tor
	Matcher(const Matcher& other);//copy c'tor
	const void initImage(Image& img);//function initImage, take an image object and initialize it with this->detector.
	const std::vector<pKeyPoint> match2(const cv::Mat& img1, const cv::Mat& img2, bool drawMatch=false);/*function match2(const Mat&,const Mat&,bool), takes two image matrices as an input,
																										 and return matching features that has been filtered by the Lowe's ratio test.*/
	const void match2(Image& img1, Image& img2, bool drawMatch=false);/*function match2(Image&,Image&,bool),
																	   takes two image objects as an input,
																	   and return matching features that has been filtered by the Lowe's ratio test*/
	const void draw(const cv::Mat& img1, const cv::Mat& img2, const std::vector<pKeyPoint>& kvec);/*function draw(const Mat&,const Mat&,const vector<pKeyPoint>),
																								  takes two image matrices as an input, and draw the matches between them*/
	const void draw(const Image& img1, const Image& img2, const std::vector<cv::DMatch>& match, std::string title="Match");/*function draw(const Image&,const Image&,const vector<pKeyPoint>,string="Match"),
																								  takes two images as an input, and draw the matches between them*/
private:
	const int minHessian = MIN_HESSIAN;//number of keypoints
	cv::Ptr<cv::ORB> detector;//pointer to an ORB detector object
	cv::Ptr<cv::DescriptorMatcher> matcher;
};

#endif
