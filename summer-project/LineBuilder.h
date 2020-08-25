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
	void setPos(Vec3d pos);
	Line getLine(std::vector<double> pixel = {0,0} ) const;
	std::vector<Line> genLines(std::vector<std::vector<cv::Vec2f>> pixelList);
protected:
	std::vector<double> resolution; // the camera's resolution (horizontal, vertical)
	std::vector<double> angle; // the camera's angle of view (horisontal, vertical)
	Vec3d pos;
	cv::Mat_<float> invCameraMatrix;
	std::vector<cv::Mat_<float>> rotation;
	std::vector<cv::Mat_<float>> translation;
	cv::Mat_<float> initCameraPos = cv::Mat::zeros(cv::Size(1,3),CV_32F);
	cv::Mat_<float> initCameraRot=cv::Mat::eye(3,3,CV_32F);

};

#endif
