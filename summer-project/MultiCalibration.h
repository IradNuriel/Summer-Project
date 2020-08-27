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


void multiCalibrate(int nCamera,std::string inputFileName,std::string outputFileName,int width,int height,bool showProcess=false) {
	cv::randpattern::RandomPatternGenerator generator(width, height);
	generator.generatePattern();
	cv::Mat pattern = generator.getPattern();
	cv::randpattern::RandomPatternCornerFinder finder(width, height, Constants::MIN_HESSIAN);
	finder.loadPattern(pattern);

	for (int cameraIdx = 0; cameraIdx < nCamera; cameraIdx++) {
		finder.computeObjectImagePoints(getImageVector(inputFileName,cameraIdx));
		std::vector<cv::Mat> objectPoints = finder.getObjectPoints();
		std::vector<cv::Mat> imagePoints = finder.getImagePoints();

	}
	cv::multicalib::MultiCameraCalibration multiCalib(cv::multicalib::MultiCameraCalibration::PINHOLE, nCamera, inputFileName, width, height, showProcess, Constants::MIN_HESSIAN);
	multiCalib.run();
	multiCalib.writeParameters(outputFileName);
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
