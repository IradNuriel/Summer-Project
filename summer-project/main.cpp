#include <iostream>
#include "LineBuilder.h"
#include "Cluster.h"
#include "ORBDetection.h"

Cluster getCluster(LineBuilder& lb, std::vector<std::valarray<int>>& pixel, int offset=2) {
	Cluster clus = Cluster();
	int n = pixel.size();
	for (int i = 0; i < n; i++) {
		clus.add(lb.getLine({ 25.0*(i - offset),118,0 }, pixel[i]));
	}
	return clus;
}

void checkLinePart() {
	LineBuilder lb = LineBuilder();
	std::vector<std::valarray<int>> pixel = { {5566, 1039}, { 4988,1032 }, { 4742,1021 }, { 4255, 1019 },{ 3747,1008 }, { 3544,1016 } };
	std::vector<std::valarray<int>> topcircle = { {3900, 1180}, { 3329,1175 }, { 3056,1177 }, { 2571,1190 }, { 2062,1188 }, { 1880,1206 } };
	std::vector<std::valarray<int>> power = { {4335, 2434}, { 3783,2428 }, { 3529,2437 }, { 3067,2446 }, { 2574,2448 }, { 2393,2454 } };
	std::vector<std::valarray<int>> desk = { {2269,2973},{1540,2977},{1123,2964},{511,2980} };
	std::vector<std::valarray<int>> remoteControl = { {3838,3781},{3093,3782},{2608,3789},{1941,3796},{1258,3803},{911,3767} };
	std::vector<std::valarray<int>> window = { {5784,2626},{5133,2635},{4473,2637},{4091,2667} }; //offset=0
	std::vector<Line> lines;
	Cluster clus = getCluster(lb, topcircle);
	std::cout << clus << std::endl;
	clus = getCluster(lb, power);
	std::cout << clus << std::endl;
}

void checkORBDetectionPart() {
	ORBDetector orbDetector;
#ifdef _SET1_
	for (int i = -2; i <= 3; i++) {
		char filename[20];
		sprintf_s(filename, "set1/%d.jpg", i);
		cv::Mat src = cv::imread(cv::samples::findFile(filename), IMREAD_ANYCOLOR);
		orbDetector.addImageToSet(src);
	}
	for (int i = 0; i < 6; i++) {
		orbDetector.getFeaturesOfImage(i);
		orbDetector.showImageWithKeyPoints(i);
	}
	for (int i = 5; i >= 0; i--) {
		orbDetector.removeImageFromSet(i);
	}
#else
	cv::Mat src = cv::imread(cv::samples::findFile("set1/1.jpg"), IMREAD_ANYCOLOR);
	orbDetector.addImageToSet(src);
	orbDetector.showImageWithKeyPoints(0);
	orbDetector.removeImageFromSet(0);
#endif
}


int main() {
	checkLinePart();
	checkORBDetectionPart();
}

