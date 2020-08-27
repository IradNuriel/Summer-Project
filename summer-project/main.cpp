#include <iostream>
#include <ctime>
#include <fstream>

#include "LineBuilder.h"
#include "Cluster.h"
#include "ORBDetector.h"
#include "Matcher.h"
#include "KMatcher.h"
#include "Camera.h"
#include "CloudDetector.h"
#include "Utilities.h"
#include "MultiCalibration.h"

#define NOAM_COMPUTER 0
#define FOLDER1 (NOAM_COMPUTER?"./../../summer-project/set1/":"set1/")

// read the cfg file
void readCfg(std::string &camData, bool &calibrationNow)
{
	std::string buffer;
	std::ifstream cfgFile(Constants::CFG_FILE_NAME);
	std::getline(cfgFile, camData);
	std::getline(cfgFile, buffer);
	calibrationNow = buffer.compare("true");
	cfgFile.close();
}

// LineBuilder friend function
void initCameras(const std::vector<Camera>& cameras) {
	LineBuilder::cams = cameras;
}

int main(int argc, char *argv[]) {

	// if this is set to true, the program will calculate calibration data from the calibration directory.
	// otherwise, it will read from the .json file already containing the camera data.
	bool calibrationNow = true;

	// path to the .json file storing the cams' calibration data
	std::string camData;

	// read from the .cfg file
	readCfg(camData, calibrationNow);
	
	// get camera calibration data (from either a set of pictures taken by them or a .json file)
	std::vector<Camera> cams = MultiCalibration::getCalibration(calibrationNow, Constants::CALIBRATION_DIR, camData);

	// initialize LineBuilder cam list
	initCameras(cams);

	// for each frame, match common features captured by different cameras from different angles
	int i = 0;
	try {
		while(true) {
			// for each camera, retrieve the i'th frame
			std::vector<Image> images;
			for (int j = 0; j < cams.size(); j++) {
				// try loading the image with all possible suffices
				cv::Mat imgMatrix = Utilities::tryLoad(FOLDER1, i, j);
	
				// insert the loaded image into the image list
				Image img = Image(j, imgMatrix);
				images.push_back(img);
			}

			// match the retrieved images
			KMatcher km;
			km.match(images);

			i++;
		}
	}
	catch (std::exception e) {
		std::cout << "finished running, finished at frame:" << i << std::endl;
	}
}