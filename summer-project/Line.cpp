/////////////////////////////////
// file written by Irad Nuriel,//
// written in 26/07/2020       //
/////////////////////////////////


#include "Line.h"


Line::Line() {
	setV({ 1, 1, 1 });
	setP({ 0, 0, 0 });
}

Line::Line(const Line& l) {
	setV(l.getV());
	setP(l.getP());
}

Line::Line(cv::Vec3d p, cv::Vec3d v) {
	setV(v);
	setP(p);
}

cv::Vec3d Line::getV() const { 
	return this->v; 
}
cv::Vec3d Line::getP() const { 
	return this->originProjection; 
}

void Line::setV(cv::Vec3d v) {
	this->v = normalize(v);
}

void Line::setP(cv::Vec3d p) {
	this->originProjection = p - p.dot(this->v) * this->v;
}


Line Line::operator=(const Line& other) {
	this->setP(other.getP());
	this->setV(other.getV());
	return *this;
}


cv::Vec3d Line::projectionOf(cv::Vec3d x) const {
	return this->originProjection + x.dot(this->v) * this->v;
}