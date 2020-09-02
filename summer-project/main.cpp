#define _CRT_SECURE_NO_WARNINGS

//normal includes
#include <iostream>
#include <ctime>
#include <fstream>


//our file includes
#include "LineBuilder.h"
#include "Cluster.h"
#include "Matcher.h"
#include "KMatcher.h"
#include "Camera.h"
#include "CloudDetector.h"
#include "Utilities.h"
#include "Calibration.h"


//
#define NOAM_COMPUTER 0
#define FOLDER1 (NOAM_COMPUTER?"./../../summer-project/scene/":"scene/")

// read the cfg file
void readCfg(std::string &inPath, std::string &camData, bool &calibrationNow) {
	std::string buffer;
	std::ifstream cfgFile(Constants::CFG_FILE_NAME);
	std::getline(cfgFile, inPath);	// read inPath
	std::getline(cfgFile, camData);	// read camData
	std::getline(cfgFile, buffer);	// read calibrationNow
	calibrationNow = (buffer.compare("true")==0);
	cfgFile.close();
}

// LineBuilder friend function
void initCameras(const std::vector<Camera>& cameras) {
	LineBuilder::cams = cameras;
}

//prints all pairs of clouds that are less than 2 meters apart. 
void printToCloseClouds(std::vector<Cloud> clouds) {
	for (int i = 0; i < clouds.size(); i++) {
		for (int j = i + 1; j < clouds.size(); j++) {
			cv::Vec3d avg1=clouds[i].average(), avg2=clouds[j].average();
			int distSquared = (avg1 - avg2).dot(avg1 - avg2);
			if (distSquared <= (Constants::MIN_DISTANCE_SQUARED_BETWEEN_OBJECTS)) {
				std::cout << "there are clouds less than 2 meters apart: " << "the clouds:" << std::endl << "cloud 1(mass center): " << clouds[i].average() << std::endl << "cloud 2(mass center): " <<  clouds[j].average() << std::endl;
			}
		}
	}
}



int main(int argc, char *argv[]) {

	// if this is set to true, the program will calculate calibration data from the calibration directory.
	// otherwise, it will read from the .json file already containing the camera data.
	bool calibrationNow = true;

	// path to the .xml file storing the cams' calibration data
	std::string inPath;	 // path
	std::string camData; // file name

	//nCamera, number of different cameras
	int nCameras;
	// read from the .cfg file
	readCfg(inPath, camData, calibrationNow);
	
	// get camera calibration data (from either a set of pictures taken by them or a .xml file)
	std::vector<Camera> cams = Calibration::getCalibration(calibrationNow, inPath, camData, nCameras);

	// initialize LineBuilder cam list
	initCameras(cams);

	// for each frame, match common features captured by different cameras from different angles
	int i = 0;
	bool wasEndReached=false;
	while(!wasEndReached) {
		// for each camera, retrieve the i'th frame
		std::vector<Image> images;
		for (int j = 0; j < cams.size(); j++) {
			// try loading the image with all possible suffices
			cv::Mat imgMatrix = Utilities::tryLoad(FOLDER1, j, i);
			if (imgMatrix.empty()) {
				wasEndReached = true;
				break;
			}
			// insert the loaded image into the image list
			Image img = Image(j, imgMatrix);
			images.push_back(img);
		}
		if (wasEndReached) {
			continue;
		}
		// match the retrieved images
		KMatcher km;
		std::vector<cv::Vec3d> points = km.match(images);
		char namepoint[100], namecenter[100];
		sprintf(namepoint, "points%d.txt", i);
		sprintf(namecenter, "centers%d.txt", i);
		std::ofstream pointout(namepoint);
		std::ofstream centerout(namecenter);
		for (cv::Vec3d point : points) {
			pointout << point << std::endl;
		}
		// process clouds
		std::vector<Cloud> clouds = CloudDetector::detectGroups(points);
		for (Cloud point : clouds) {
			centerout << point.average() << std::endl;
		}

		printToCloseClouds(clouds);


		i++;
	}
}