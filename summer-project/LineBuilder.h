#ifndef _LINE_BUILDER_H_
#define _LINE_BUILDER_H_

#include "Line.h"

// use PI
#define _USE_MATH_DEFINES
#include <math.h>

// use value arrays
#include <valarray>
using namespace std;

// this entire class was translated from python, orig. code by Licht
class LineBuilder
{
public:
	LineBuilder(valarray<int> res = { 6000, 4000 }, valarray<double> a = { 66.276, 46.861 }); // c'tor
	Line get_line(Vec3d pos = Vec3d(), valarray<int> pixel = {0,0} ) const;
protected:
	valarray<int> resolution; // the camera's resolution (horizontal, vertical)
	valarray<double> angle; // the camera's angle of view (horisontal, vertical)
};

#endif