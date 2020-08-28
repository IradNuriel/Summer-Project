#include "Utilities.h"

namespace Utilities {
	// Generate fileName
	std::string imgFileName(const std::string folderPath, const int cameraID, const int frameID, const std::string suffix) {
		return folderPath + std::to_string(cameraID) + "_" + std::to_string(frameID) + "." + suffix;
	}

	// This should be super fast
	bool fileExists(const std::string fileName) {
		struct stat buffer;
		return (stat(fileName.c_str(), &buffer) == 0);
	}

	// Try and load the given image using the known suffices
	cv::Mat tryLoad(const std::string folderPath, const int cameraID, const int frameID) {
		for (int k = 0; k < Constants::N_SUFFICES; k++) {
			std::string path = imgFileName(folderPath, cameraID, frameID, Constants::SUFFICES[k]);
			if (fileExists(path)) {
				return cv::imread(cv::samples::findFile(path), cv::IMREAD_ANYCOLOR);
			}
		}
		return cv::Mat();
	}

	bool prefixExist(const std::string folderPath, const int cameraID, const int frameID, int& suffix) {
		for (int k = 0; k < Constants::N_SUFFICES; k++) {
			std::string path = imgFileName(folderPath, cameraID, frameID, Constants::SUFFICES[k]);
			if (fileExists(path)) {
				suffix = k;
				return true;
			}
		}
		return false;
		suffix = -1;
	}
}