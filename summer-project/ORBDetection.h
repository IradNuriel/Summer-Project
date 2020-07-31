#ifndef _ORBDETECTION_H_
#define _ORBDETECTION_H_
#include <iostream>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"


class ORBDetection{
public:
	ORBDetection();//defsult c'tor
	ORBDetection(const cv::Mat& img);//c'tor
	ORBDetection(const ORBDetection& other);//copy c'tor
	std::vector<cv::KeyPoint> getFeaturesOfImage(int i);//function getFeaturesOfImageI, return the features found in image number i.
	const ORBDetection& addImageToSet(const cv::Mat img);//function addImageToSet, add img to the data set
	const ORBDetection& removeImageFromSet(int i);//function removeImageFromSetIn, remove the image in place i from the data set
	void showImageWithKeyPoints(int i);//show the i'th image in the data set with its keypoints.
private:
	std::vector<cv::Mat> imageSet;//the data set
	int minHessian;//number of keypoints
	cv::Ptr<cv::ORB> detector;//pointer to an ORB detector object
};

/*
	CommandLineParser parser(argc, argv, "{@input | box.png | input image}");
	Mat src = imread(samples::findFile(parser.get<String>("@input")), IMREAD_GRAYSCALE);
	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		return -1;
	}
	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	Ptr<ORB> detector = ORB::create(minHessian);
	std::vector<KeyPoint> keypoints;
	detector->detect(src, keypoints);
	//-- Draw keypoints
	Mat img_keypoints;
	drawKeypoints(src, keypoints, img_keypoints);
	//-- Show detected (drawn) keypoints
	imshow("SURF Keypoints", img_keypoints);
	waitKey();
	return 0;

*/
#endif