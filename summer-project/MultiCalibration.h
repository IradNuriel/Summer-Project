#ifndef _MULTI_CALIBRATION
#define _MULTI_CALIBRATION

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/ccalib/multicalib.hpp"
#include "Constants.h"
#include <vector>
#include <fstream>




void multiCalibrate(int nCamera,std::string inputFileName,std::string inPath,std::string outputFileName,int width,int height,int patternWidth,int patternHeight,bool showProcess=false) {
	cv::Mat pattern = cv::imread(inPath+"/randPattern.jpg");
	cv::randpattern::RandomPatternCornerFinder finder(width, height, Constants::MIN_HESSIAN);
	finder.loadPattern(pattern);
	std::vector<cv::Mat_<float>> cameraMatrixs;
	std::vector<std::vector<cv::Mat>> rvecss;//camera rotation
	std::vector<std::vector<cv::Mat>> tvecss;//camera translation
	std::vector<cv::Mat> distortionCoeffs;
	std::vector<cv::Mat_<float>> meanTranss;
	for (int cameraIdx = 0; cameraIdx < nCamera; cameraIdx++) {
		std::vector<cv::Mat> imgVec = getImageVector(inPath, cameraIdx);
		finder.computeObjectImagePoints(imgVec);
		std::vector<cv::Mat> objPoints = finder.getObjectPoints();
		std::vector<cv::Mat> imgPoints = finder.getImagePoints();
		cv::Mat_<float> cameraMatrix;//camera matrix
		std::vector<cv::Mat> rvecs;//camera rotation
		std::vector<cv::Mat> tvecs;//camera translation
		cv::Mat distortionCoeff;
		cv::calibrateCamera(objPoints, imgPoints, imgVec[0].size(), cameraMatrix, distortionCoeff, rvecs, tvecs);//opencv magic to get camera calibration
		cameraMatrixs.push_back(cameraMatrix);
		rvecss.push_back(rvecs);
		tvecss.push_back(tvecs);
		distortionCoeffs.push_back(distortionCoeff);
	}
	cv::multicalib::MultiCameraCalibration multiCalib(cv::multicalib::MultiCameraCalibration::PINHOLE, nCamera, inputFileName, patternWidth, patternHeight, showProcess, Constants::MIN_HESSIAN);
	multiCalib.run();
	

}



cv::Mat getPattern(int width,int height) {
	cv::randpattern::RandomPatternGenerator generator(width, height);
	generator.generatePattern();
	cv::Mat pattern = generator.getPattern();
	return pattern;
}

std::vector<cv::Mat> getImageVector(std::string inPath,int cameraIdx) {
	//dont know how to work with those kind of files, TODO: ask someone in the lab in thusday
	std::vector<cv::Mat> imgVec;
	for (int timeStemp = 0; fileExists(inPath + std::to_string(cameraIdx) + "_" + std::to_string(timeStemp)) + ".jpeg"; timeStemp++) {
		cv::Mat img = cv::imread(inPath + std::to_string(cameraIdx) + "_" + std::to_string(timeStemp));
		if (img.empty()) {
			std::cout << "FUCK" << std::endl;
			continue;
		}
		imgVec.push_back(img);
	}
	return imgVec;

}




// this should be super fast
bool fileExists(std::string fileName) {
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}







#endif // !_MULTI_CALIBRATION
