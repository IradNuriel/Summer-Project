#include "LineBuilder.h"
#include "Line.h"

// c'tor
LineBuilder::LineBuilder(std::valarray<int> res, std::valarray<double> a) {
	resolution.resize(2);
	std::copy(std::begin(res), std::end(res), std::begin(resolution));
	angle = a / 180.0 * M_PI;
}

Line LineBuilder::getLine(Vec3d pos, std::valarray<int> pixel) const {
	// casting int array into double array (to be used later)
	std::valarray<double> pixeld = std::valarray<double>(2);
	std::copy(std::begin(pixel), std::end(pixel), std::begin(pixeld));
	
	pixeld = pixeld - (resolution / 2); //normalizing pixel pos relative to center
	pixeld[1] = -pixeld[1]; // coordinates (>x, ^y, .z)
	std::valarray<double> temp = (pixeld * tan(angle / 2.0)) / (resolution / 2.0); // calculating x,y length relative to z (=1)
	Vec3d v = { temp[0], temp[1], 1 }; //declaring the 3d line vector based on the calculation above
	return Line(v, pos);
}
