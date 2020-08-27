#ifndef _LINE_BUILDER_H_
#define _LINE_BUILDER_H_

#include "Line.h"
#include "Camera.h"
#include "Constants.h"
#include <vector>
// use PI

// this entire class was translated from python, orig. code by Licht
class LineBuilder {
public:
	LineBuilder(std::vector<double> res = {1200,800}, std::vector<double> a = {66.276,46.861});
	LineBuilder(Camera camera); // c'tor
	const LineBuilder& operator=(const LineBuilder& other);
	Line getLine(std::vector<double> pixel = {0,0} ) const;
	void setPos(const Vec3f& pos);
	//std::vector<Line> genLines(std::vector<std::vector<cv::Vec2f>> pixelList);
	friend void initCameras(const &std::_Vector_iterator<Camera> cameras)
protected:
	void setPos();
	void setAngle(const std::vector<double>&);
	void setRes(const std::vector<double>&);

	std::vector<double> resolution; // the camera's resolution (horizontal, vertical)
	std::vector<double> angle; // the camera's angle of view (horisontal, vertical)
	Vec3d pos;
	Camera& camera=Camera::no_camera();
private:
	static std::vector<Camera> cams;
};

#endif
