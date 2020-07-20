#include "Line.h"


Line::Line() {
	setV({ 1, 1, 1 });
	setP({ 0, 0, 0 });
}

Line::Line(const Line& l) {
	setV(l.getV());
	setP(l.getP());
}

Line::Line(Vec3d p, Vec3d v) {
	setV(v);
	setP(p);
}

Vec3d Line::getV() const { 
	return this->v; 
}
Vec3d Line::getP() const { 
	return this->originProjection; 
}

void Line::setV(Vec3d v) {
	this->v = normalize(v);
}

void Line::setP(Vec3d p) {
	this->originProjection = p - p.dot(this->v) * this->v;
}

Vec3d Line::projectionOf(Vec3d x) const {
	return this->originProjection + x.dot(this->v) * this->v;
}