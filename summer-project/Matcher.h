#ifndef _MATCHER_H_
#define _MATCHER_H_
#include <iostream>
#include <vector>
#include <utility>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "Constants.h"
#include "Cluster.h"
#include "Image.h"

typedef std::pair<cv::KeyPoint, cv::KeyPoint> pKeyPoint;
class Matcher {
public:
	Matcher();//defult c'tor
	Matcher(const Matcher& other);//copy c'tor
	void initImg(Image& img) const; //function initImage, take an image object and initialize it with this->detector.
	std::vector<pKeyPoint> match2(const cv::Mat& img1, const cv::Mat& img2, bool drawMatch=false) const; /*function match2(const Mat&,const Mat&,bool), takes two image matrices as an input,
														and return matching features that has been filtered by the Lowe's ratio test.*/
	std::vector<cv::DMatch> match2(Image& img1, Image& img2, bool drawMatch=false) const; /*function match2(Image&,Image&,bool),
												takes two image objects as an input,
												and return matching features that has been filtered by the Lowe's ratio test*/
	void draw(const cv::Mat& img1, const cv::Mat& img2, const std::vector<pKeyPoint>& kvec) const; /*function draw(const Mat&,const Mat&,const vector<pKeyPoint>),
													takes two image matrices as an input, and draw the matches between them*/
	void draw(const Image& img1, const Image& img2, const std::vector<cv::DMatch>& match, std::string title="Match") const; /*function draw(const Image&,const Image&,const vector<pKeyPoint>,string="Match"),
																  takes two images as an input, and draw the matches between them*/
protected:
	const int minHessian=Constants::MIN_HESSIAN;//number of keypoints
	cv::Ptr<cv::ORB> detector;//pointer to an ORB detector object
	cv::Ptr<cv::DescriptorMatcher> matcher; //pointer to a DescriptorMatcher object
};

#endif
