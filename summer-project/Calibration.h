#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "Constants.h"


//code based on python example by Rie Ruash, Reut Elboim and Yehonatan Leizerson
void caliber(std::string directorypath, int numOfImages, std::string outputFilePath) {
	cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 35, 0.001);//square size of 35 mm 
	std::vector<cv::Point3f> objp = {{0.0, 0.0, 0.0},
									 {1.0, 0.0, 0.0},
									 {2.0, 0.0, 0.0},
									 {3.0, 0.0, 0.0},
									 {4.0, 0.0, 0.0},
									 {5.0, 0.0, 0.0},
									 {6.0, 0.0, 0.0},
									 {0.0, 1.0, 0.0},
									 {1.0, 1.0, 0.0},
									 {2.0, 1.0, 0.0},
									 {3.0, 1.0, 0.0},
									 {4.0, 1.0, 0.0},
									 {5.0, 1.0, 0.0},
									 {6.0, 1.0, 0.0},
									 {0.0, 2.0, 0.0},
									 {1.0, 2.0, 0.0},
									 {2.0, 2.0, 0.0},
									 {3.0, 2.0, 0.0},
									 {4.0, 2.0, 0.0},
									 {5.0, 2.0, 0.0},
									 {6.0, 2.0, 0.0},
									 {0.0, 3.0, 0.0},
									 {1.0, 3.0, 0.0},
									 {2.0, 3.0, 0.0},
									 {3.0, 3.0, 0.0},
									 {4.0, 3.0, 0.0},
									 {5.0, 3.0, 0.0},
									 {6.0, 3.0, 0.0}};//constant
	std::vector<std::vector<cv::Point3f>> objPoints;//3d points of the object in real space
	std::vector<std::vector<cv::Point2f>> imgPoints;//2d points of the object in the image plane
	cv::Mat gray;
	for (int i = 0; i < numOfImages; i++) {//for any image
		std::string filePath = directorypath + "(" + std::to_string(i) + ").jpeg";
		//convention: calibration images are sorted such that the i'th image is (i).jpeg
		cv::Mat img = cv::imread(filePath);//reading the i'th image
		if (img.empty()) {//if no image
			std::cout << "FUCK" << std::endl;
			continue;
		}
		cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);//gray<--grayscale version of the image
		std::vector<cv::Point2f> corners;//corners
		bool ret = cv::findChessboardCorners(gray, cv::Size(7, 4), corners);//opencv magic to detect the chessboard corners
		if (ret) {//if detected correctly
			objPoints.push_back(objp);//points in real life+=constant
			cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);//refine image points of corners
			imgPoints.push_back(corners);//points in image plane+=corners
			cv::drawChessboardCorners(img, cv::Size(7, 4), corners, ret);//draw the image with corners
			cv::imshow("img", img);
			cv::waitKey(500);
		}
	}
	cv::Mat_<double> cameraMatrix;//camera matrix
	cv::Mat distortionCoeff;
	std::vector<cv::Mat> rvecs, tvecs;
	int flag = 0;
	calibrateCamera(objPoints, imgPoints, gray.size(), cameraMatrix, distortionCoeff, rvecs, tvecs);//opencv magic to get camera calibration

	//save camera matrix to file
	std::ofstream fout(outputFilePath);
	if (!fout) {
		std::cout << "File Not Opened" << std::endl;  return;
	}
	for (int i = 0; i < cameraMatrix.rows; i++) {
		for (int j = 0; j < cameraMatrix.cols; j++) {
			fout << double(cameraMatrix.at<double>(i, j)) << "\t";
		}
		fout << std::endl;
	}
	fout.close();
}



#endif // !_CALIBRATION_H_
