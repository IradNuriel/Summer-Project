#include <iostream>
#include <ctime>
#include <fstream>
#include "LineBuilder.h"
#include "Cluster.h"
#include "ORBDetector.h"
#include "Matcher.h"
#include "KMatcher.h"
//#include "Camera.h"
#include "CloudDetector.h"


#define NOAM_COMPUTER 1
#define FOLDER1 (NOAM_COMPUTER?"./../../summer-project/set1/":"set1/")



std::string path(int i) {
	return FOLDER1 + std::to_string(i) + ".jpg";
}

Cluster getCluster(LineBuilder& lb, std::vector<std::vector<double>>& pixel, int offset=2) {
	Cluster clus = Cluster();
	int n = pixel.size();
	for (int i = 0; i < n; i++) {
		lb.setPos({25.0*(i - offset), 118, 0});
		clus.add(lb.getLine(pixel[i]));
	}
	return clus;
}

void checkLinePart() {
	LineBuilder lb = LineBuilder();
	std::vector<std::vector<double>> pixel = { {5566, 1039}, { 4988,1032 }, { 4742,1021 }, { 4255, 1019 },{ 3747,1008 }, { 3544,1016 } };
	std::vector<std::vector<double>> topcircle = { {3900, 1180}, { 3329,1175 }, { 3056,1177 }, { 2571,1190 }, { 2062,1188 }, { 1880,1206 } };
	std::vector<std::vector<double>> power = { {4335, 2434}, { 3783,2428 }, { 3529,2437 }, { 3067,2446 }, { 2574,2448 }, { 2393,2454 } };
	std::vector<std::vector<double>> desk = { {2269,2973},{1540,2977},{1123,2964},{511,2980} };
	std::vector<std::vector<double>> remoteControl = { {3838,3781},{3093,3782},{2608,3789},{1941,3796},{1258,3803},{911,3767} };
	std::vector<std::vector<double>> window = { {5784,2626},{5133,2635},{4473,2637},{4091,2667} }; //offset=0
	std::vector<Line> lines;
	Cluster clus = getCluster(lb, topcircle);
	std::cout << clus << std::endl;
	clus = getCluster(lb, power);
	std::cout << clus << std::endl;
}

void checkORBDetectionPart() {
	ORBDetector orbDetector;
	cv::Mat src = cv::imread(cv::samples::findFile(path(1)), IMREAD_ANYCOLOR);
	orbDetector.addImageToSet(src);
	orbDetector.showImageWithKeyPoints(0);
	orbDetector.removeImageFromSet(0);
}

void checkMatcher() {
	using namespace std::chrono;
	std::vector<time_point<steady_clock>> times;
	times.push_back(high_resolution_clock::now());
	std::vector<Image> images;
	for (int i = -2; i <= 3; i++) {
		cv::Mat src = cv::imread(cv::samples::findFile(path(i)));
		images.push_back(Image(src, { 25.0*(i - 2),118,0 }));
	}
	times.push_back(high_resolution_clock::now());
	Matcher mat;
	for (Image& img : images) mat.initImg(img);
	times.push_back(high_resolution_clock::now());
	int jump = 1;
	for (int i = 0; i + jump < 5; i++) {
		mat.match2(images[i], images[i+jump], true);
	}
	times.push_back(high_resolution_clock::now());
	for (int i = 0; i + 1 < times.size(); i++) {
		std::cout << duration_cast<std::chrono::milliseconds>(times[i+1]-times[i]).count() << std::endl;
	}
}

void checkKMatcher() {
	std::vector<Image> images;
	for (int i = -2; i <= 3; i++) {
		cv::Mat src = cv::imread(cv::samples::findFile(path(i)));
		images.push_back(Image(src, { 25.0*(i - 2),118,0 }));
	}
	KMatcher matcher;
	std::ofstream pointsOut("points.txt");
	std::vector<cv::Vec3d> points = matcher.match(images);
	for (const cv::Vec3d& point : points) pointsOut << point << std::endl;

	std::vector<std::vector<cv::Vec3d>> groups = CloudDetector::detectGroups(points);
	std::cout << "#GROUPS: " << groups.size() << std::endl;
	std::ofstream avgPointsOut("avgpoints.txt");
	for (const std::vector<cv::Vec3d>& group : groups) avgPointsOut << CloudDetector::avrage(group) << std::endl;
}


/*
bool checkCalibration(std::string calibrationDir) {
	//caliber("chessboardcalibration/", 30, "riePhoneCameraCalibration.txt");
	try {
		Camera riesPhoneCamera(30, calibrationDir, 7, 4);
		std::cout << riesPhoneCamera;
	}
	catch(const std::exception& e) {
		std::cout << "Calibration check failed!" << e.what();
		return false;
	}
	return true;
}

void storeCalibration(Camera* cams, std::string outputFile) {
	
}

// read a calibration directory, store the calibration data in .json, and return a camera array
Camera* newCalibration(std::string calibrationDir, std::string outputFile) {
	// create camera array
	Camera* cams;
	// ...
	
	// store camera array in .json file
	storeCalibration(cams, outputFile);
	
	// return camera array
	return cams;
}

// read calibration data from a .json file, and return a camera array
Camera* readCalibration(std::string inputFile) {
	// convert .json to a camera array
}*/

int main() {
	//checkMatcher();
	checkKMatcher();
	/*bool calibrationNow = true; // if this is set to false, the program will read calibration data
	std::string calibrationDir = "chessboardcalibration/";
	int n_photos;
	
	std::string camData = "camData.json";
	
	// calibration
	Camera* cams;
	if (calibrationNow)
		cams = newCalibration(calibrationDir, camData);
	else if (checkCalibration(calibrationDir)){
		cams = readCalibration(camData);
	}
	else
		return; // error
	
	// matching points
	*/
}
