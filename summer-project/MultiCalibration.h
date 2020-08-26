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
	finder.computeObjectImagePoints(getImageVector(inputFileName));
	std::vector<cv::Mat> objectPoints = finder.getObjectPoints();
	std::vector<cv::Mat> imagePoints = finder.getImagePoints();
	cv::multicalib::MultiCameraCalibration multiCalib(cv::multicalib::MultiCameraCalibration::PINHOLE, nCamera, inputFileName, width, height, showProcess, Constants::MIN_HESSIAN);
	multiCalib.run();
	multiCalib.writeParameters(outputFileName);
}


std::vector<cv::Mat> getImageVector(std::string inPath) {
	//dont know how to work with those kind of files, TODO: ask someone in the lab in thusday
}


#endif // !_MULTI_CALIBRATION
