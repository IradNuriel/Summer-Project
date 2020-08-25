#ifndef _LINE_BUILDER_H_
#define _LINE_BUILDER_H_

#include "Line.h"
#include "Constants.h"
#include <vector>
// use PI


// this entire class was translated from python, orig. code by Licht
class LineBuilder {
public:
	LineBuilder(std::vector<double> res = { 6000, 4000 }, std::vector<double> a = { 66.276, 46.861 }); // c'tor
	Line getLine(Vec3d pos = Vec3d(), std::vector<double> pixel = {0,0} ) const;
	std::vector<Line> genLines(cv::Mat_<float> invCameraMatrix, std::vector<std::vector<cv::Vec2f>>, cv::Mat_<float> rotation, cv::Mat_<float> translation, cv::Mat_<float> initCameraRot = cv::Mat::eye(3, 3, CV_32F), cv::Mat_<float> initCameraPos = cv::Mat::zeros(cv::Size(1, 3), CV_32F));
protected:
	std::vector<double> resolution; // the camera's resolution (horizontal, vertical)
	std::vector<double> angle; // the camera's angle of view (horisontal, vertical)
};

#endif
