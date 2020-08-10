#ifndef _LINE_BUILDER_H_
#define _LINE_BUILDER_H_

#include "Line.h"
#include "Constants.h"
// use PI

// use value arrays
#include <valarray>

// this entire class was translated from python, orig. code by Licht
class LineBuilder {
public:
	LineBuilder(std::valarray<double> res = { 6000, 4000 }, std::valarray<double> a = { 66.276, 46.861 }); // c'tor
	Line getLine(Vec3d pos = Vec3d(), std::valarray<double> pixel = {0,0} ) const;
protected:
	std::valarray<double> resolution; // the camera's resolution (horizontal, vertical)
	std::valarray<double> angle; // the camera's angle of view (horisontal, vertical)
};

#endif