#ifndef _CALIBRATION_
#define _CALIBRATION_

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/ccalib/multicalib.hpp"

#include "Camera.h"
#include "Constants.h"
#include "Utilities.h"
#include <vector>
#include <fstream>

namespace Calibration {
	std::vector<Camera> multiCalibrate(int nCamera, std::string inPath, std::string outputFileName, int width, int height, int patternWidth, int patternHeight, bool showProcess = false);
	std::vector<Camera> parseParamsFile(std::string inputFileName);
	void generateImgList(std::string inFile, int nCamera, std::string inPath);
	std::vector<Camera> getCalibration(bool needCalibration, std::string inPath, std::string camData);
	int calcNCamera(std::string inPath);
	std::vector<cv::Mat> getImageVector(std::string inPath, int cameraIdx);
}








#endif // !_MULTI_CALIBRATION
