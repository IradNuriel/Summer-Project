#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <string>

#include "Constants.h"

namespace Utilities {
	bool fileExists(std::string fileName);
	cv::Mat tryLoad(const std::string folderName, const int cameraID, const int frameID);
}

#endif