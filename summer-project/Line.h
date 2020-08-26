#ifndef _LINE_H_
#define _LINE_H_
#include <opencv2/opencv.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <limits>


using namespace cv;
	
/* Line is represented by a point p which is the projection
   of the origin on the line and a unit direction vector v */
class Line {
public:
	Line();//default c'tor of class Line
	Line(const Line& l);//copy c'tor
	Line(Vec3d p, Vec3d v);//c'tor of class Line
	Vec3d getV() const;//function getV, return the direction vector of the line
	Vec3d getP() const;//function getP, return the projection of the origin of the line on the line
	void setV(Vec3d v);/*function setV, get vector as an input,
					   and change the direction vector of the line to the normalized form of the vector*/
	void setP(Vec3d p);/*function setP, get a 3D point as an input, represented by a vecter, 
					   and change the origin of th eline to its projection on the line.*/
	Vec3d projectionOf(Vec3d x) const;/*function projectionOf,
									  get a 3D point as an input represented by a vector,
									  and return its projection on the line.*/
	Line operator=(const Line& other);
protected:
	Vec3d originProjection; //Projection of the origin on the line
	Vec3d v;           //Unit vector of the line's direction
};
#endif