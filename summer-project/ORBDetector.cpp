#include "ORBDetector.h"

ORBDetector::ORBDetector() {
	this->minHessian = 1200;
	this->detector = cv::ORB::create(minHessian);
	this->imageSet = std::vector<cv::Mat>();
}

ORBDetector::ORBDetector(const cv::Mat& img) {
	this->minHessian = 1200;
	this->detector = cv::ORB::create(minHessian);
	this->imageSet = std::vector<cv::Mat>();
	this->addImageToSet(img);
}

ORBDetector::ORBDetector(const ORBDetector& other) {
	this->minHessian = other.minHessian;
	this->detector = cv::Ptr<cv::ORB>(other.detector);
	this->imageSet = std::vector<cv::Mat>(other.imageSet);
}

std::vector<cv::KeyPoint> ORBDetector::getFeaturesOfImage(int i) {//using opencv ORB features detection
	std::vector<cv::KeyPoint> keypoints;
	this->detector->detect(this->imageSet[i], keypoints);
	return keypoints;
}

const ORBDetector& ORBDetector::addImageToSet(const cv::Mat img) {//adding image to the data set
	this->imageSet.push_back(img);
	return *this;
}

const ORBDetector& ORBDetector::removeImageFromSet(int i) {//removing image from the data set
	this->imageSet.erase(this->imageSet.begin() + i);
	return *this;
}

void ORBDetector::showImageWithKeyPoints(int i) {//show the i'th image with its keypoints, also save it to a file
	std::vector<cv::KeyPoint> key = this->getFeaturesOfImage(i);
#ifdef _SET1_
	char filename[100];
	sprintf_s(filename, "keypointsinset1/%dkeypoints.jpg", i-2);
#endif
	cv::Mat img_keypoints;
	cv::drawKeypoints(this->imageSet[i], key, img_keypoints,cv::Scalar::all(-1),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
#ifdef _SET1_
	cv::imwrite(filename, img_keypoints);
#endif
	cv::namedWindow("ORB Keypoints", cv::WINDOW_NORMAL);
	imshow("ORB Keypoints", img_keypoints);
#ifndef _SET1_
	cv::waitKey();
#endif

}
