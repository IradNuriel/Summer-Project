#include "LineBuilder.h"
#include "Line.h"

// c'tor
LineBuilder::LineBuilder(std::vector<double> res, std::vector<double> a) {
	resolution.resize(2);
	std::copy(std::begin(res), std::end(res), std::begin(resolution));
	// degrees -> radians
	this->angle.resize(2);
	this->angle[0] = a[0] * Constants::DEGREE_TO_RADIANS_RATIO;
	this->angle[1] = a[1] * Constants::DEGREE_TO_RADIANS_RATIO;
	
}

void LineBuilder::setPos(Vec3d pos){
	this->pos = pos;
}

Line LineBuilder::getLine(std::vector<double> pixel) const {
	// casting int array into double array (to be used later)
	std::vector<double> pixeld = std::vector<double>(2);
	std::copy(std::begin(pixel), std::end(pixel), std::begin(pixeld));
	
	//pixeld = pixeld - (resolution / 2.0); //normalizing pixel pos relative to center
	pixeld[0] = pixeld[0] - (this->resolution[0] / 2.0);
	pixeld[1] = pixeld[1] - (this->resolution[1] / 2.0);
	pixeld[1] = -pixeld[1]; // coordinates (>x, ^y, .z)

	std::vector<double> temp(2);
	temp[0]= (pixeld[0] * tan(this->angle[0] / 2.0)) / (this->resolution[0] / 2.0); // calculating x length relative to z (=1)
	temp[1] = (pixeld[1] * tan(this->angle[1] / 2.0)) / (this->resolution[1] / 2.0); // calculating y length relative to z (=1)

	Vec3d v = { temp[0], temp[1], 1 }; //declaring the 3d line vector based on the calculation above
	
	return Line(this->pos, v);

}


/*
std::vector<Line> LineBuilder::genLines(std::vector<std::vector<cv::Vec2f>> pixelList) {
	std::vector<std::vector<cv::Vec3f>> lines;
	for (std::vector<cv::Vec2f> imagePixels : pixelList) {
		std::vector<cv::Vec3f> imageLines;
		for (cv::Vec2f point : imagePixels) {
			cv::Mat_<float> line=this->invCameraMatrix*(cv::Mat(cv::Vec3f(point[0],point[1],1)));
			imageLines.push_back(cv::Vec3f(line.at<float>(0,0),line.at<float>(0,1),line.at<float>(0,2))/(line.dot(line)));
		}
		lines.push_back(imageLines);
	}
	std::vector<cv::Mat_<float>> cameraPoses;
	cameraPoses.push_back(this->initCameraPos);
	std::vector<std::vector<Line>> newLines;
	newLines.push_back(std::vector<Line>());
	for (cv::Vec3f imageLine : lines[0]) {
		newLines[0].push_back(Line(this->initCameraPos, imageLine));
	}
	cv::Mat_<float> rotationAll = this->initCameraRot;
	bool f = false;
	for (std::vector<cv::Vec3f> imageLines : lines) {
		if (f) {
			cv::Mat_<float> lastCameraPos = cameraPoses[cameraPoses.size() - 1];
			cv::Mat_<float> currentCameraPos = this->rotation.dot(lastCameraPos) + this->translation;
			cameraPoses.push_back(currentCameraPos);
			rotationAll = this->rotation * rotationAll;
			std::vector<cv::Vec3f> currentDirections;
			for (cv::Vec3f line : imageLines) {
				cv::Mat_<float> mult = rotationAll * cv::Mat_<float>(line);
				currentDirections.push_back(cv::Vec3f(mult.at<float>(0,0),mult.at<float>(0,1),mult.at<float>(0,2)));
			}
			newLines.push_back(std::vector<Line>());
			for (cv::Vec3f direction : currentDirections) {
				newLines[newLines.size() - 1].push_back(Line(currentCameraPos,direction));
			}
		}
		f = true;
	}
	std::vector<Line> res;
	for (std::vector<Line> imageLines : newLines) {
		res.insert(res.end(), imageLines.begin(), imageLines.end());
	}
	return res;
}*/