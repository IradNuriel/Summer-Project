#include <iostream>
#include "LineBuilder.h"
#include "Cluster.h"
#include "ORBDetector.h"
#include "Matcher.h"

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
	cv::Mat src = cv::imread(cv::samples::findFile("set1/1.jpg"), IMREAD_ANYCOLOR);
	orbDetector.addImageToSet(src);
	orbDetector.showImageWithKeyPoints(0);
	orbDetector.removeImageFromSet(0);
}

void checkMatcher() {
	std::vector<Image> images;
	for (int i = -2; i <= 3; i++) {
		char filename[1000];
		sprintf_s(filename, "set1/%d.JPG", i); 
		cv::Mat src = cv::imread(cv::samples::findFile(filename));
		if (src.empty()) {
			std::cout << "FUCK! " << i << std::endl;
			return;
		}
		images.push_back(Image(src, { 25.0*(i - 2),118,0 }));
	}
	Matcher mat;
	for (Image& img : images) mat.init_img(img);
	for (int i = 0; i < 4; i++) {
		mat.match2(images[i], images[i+1], true);
	}
	
	/*LineBuilder lb({ 1200,800 });
	Vec3d pos1 = {-50,118,0}, pos2 = {-25,118,0};
	std::vector<pKeyPoint> newKvec;
	for (pKeyPoint& p : Kvec) {
		cv::Point2d p1 = p.first.pt, p2 = p.second.pt;
		Line l1 = lb.getLine(pos1, { p1.x,p1.y });
		Line l2 = lb.getLine(pos2, { p2.x,p2.y });
		Cluster clus; 
		clus.add(l1);
		clus.add(l2);
		if (clus.cost() > 0.1 ) std::cout << clus << std::endl;
		else newKvec.push_back(p);
	}
	mat.draw(images[0].img, images[1].img, newKvec);*/
}

int main() {
	//checkLinePart();
	//checkORBDetectionPart();
	checkMatcher();
}

