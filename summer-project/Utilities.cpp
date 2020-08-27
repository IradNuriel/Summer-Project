#include "Utilities.h"

namespace Utilities {
	// generate fileName
	//std::string imgFileName(int cameraID, int frameID, std::string suffix) {
	//	return FOLDER1 + std::to_string(cameraID) + "_" + std::to_string(frameID) + "." + suffix;
	//}

	// this should be super fast
	bool fileExists(std::string fileName) {
		struct stat buffer;
		return (stat(fileName.c_str(), &buffer) == 0);
	}

	// try and load the given image with the known suffices
	cv::Mat tryLoad(const std::string folderName, const int cameraID, const int frameID) {
		for (int k = 0; k < Constants::N_SUFFICES; k++) {
			std::string path = folderName + std::to_string(cameraID) + "_" +std::to_string(frameID)+ "." + Constants::SUFFICES[k];
			if (fileExists(path)) {
				return cv::imread(cv::samples::findFile(path), cv::IMREAD_ANYCOLOR);
			}
		}
		std::string	message = ("File not Found, cameraIdx=" + std::to_string(cameraID) + ", frameIdx=" + std::to_string(frameID));
		throw new std::exception(message.c_str());
	}
}