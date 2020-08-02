#include <iostream>
#include <ctime>
#include "LineBuilder.h"
#include "Cluster.h"
#include "ORBDetector.h"
#include "Matcher.h"

#define NOAM_COMPUTER 0
#define FOLDER1 (NOAM_COMPUTER?"./../../summer-project/set1/":"set1/")

std::string path(int i) {
	return FOLDER1 + std::to_string(i) + ".jpg";
}

Cluster getCluster(LineBuilder& lb, std::vector<std::valarray<double>>& pixel, int offset=2) {
	Cluster clus = Cluster();
	int n = pixel.size();
	for (int i = 0; i < n; i++) {
		clus.add(lb.getLine({ 25.0*(i - offset),118,0 }, pixel[i]));
	}
	return clus;
}

void checkLinePart() {
	LineBuilder lb = LineBuilder();
	std::vector<std::valarray<double>> pixel = { {5566, 1039}, { 4988,1032 }, { 4742,1021 }, { 4255, 1019 },{ 3747,1008 }, { 3544,1016 } };
	std::vector<std::valarray<double>> topcircle = { {3900, 1180}, { 3329,1175 }, { 3056,1177 }, { 2571,1190 }, { 2062,1188 }, { 1880,1206 } };
	std::vector<std::valarray<double>> power = { {4335, 2434}, { 3783,2428 }, { 3529,2437 }, { 3067,2446 }, { 2574,2448 }, { 2393,2454 } };
	std::vector<std::valarray<double>> desk = { {2269,2973},{1540,2977},{1123,2964},{511,2980} };
	std::vector<std::valarray<double>> remoteControl = { {3838,3781},{3093,3782},{2608,3789},{1941,3796},{1258,3803},{911,3767} };
	std::vector<std::valarray<double>> window = { {5784,2626},{5133,2635},{4473,2637},{4091,2667} }; //offset=0
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
	for (Image& img : images) mat.init_img(img);
	times.push_back(high_resolution_clock::now());
	int jump = 1;
	for (int i = 0; i + jump < 5; i++) {
		mat.match2(images[i], images[i+jump], false);
	}
	times.push_back(high_resolution_clock::now());
	for (int i = 0; i + 1 < times.size(); i++) {
		std::cout << duration_cast<std::chrono::milliseconds>(times[i+1]-times[i]).count() << std::endl;
	}
}

int main() {
	//checkLinePart();
	checkORBDetectionPart();
	//checkMatcher();
}
