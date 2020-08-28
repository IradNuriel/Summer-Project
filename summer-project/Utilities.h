#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <string>
#include "Constants.h"

namespace Utilities {
	std::string imgFileName(std::string folderPath, int cameraID, int frameID, std::string suffix);
	bool fileExists(std::string fileName);
	cv::Mat tryLoad(const std::string folderPath, const int cameraID, const int frameID);
	bool prefixExist(const std::string folderPath, const int cameraID, const int frameID,int& suffix);
}

#endif