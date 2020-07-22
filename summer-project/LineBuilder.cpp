#include "LineBuilder.h"
#include "Line.h"

// c'tor
LineBuilder::LineBuilder(std::valarray<int> res, std::valarray<double> a) {
	resolution = res;
	angle = a / 180.0 * M_PI;
}

Line LineBuilder::getLine(Vec3d pos, std::valarray<int> pixel) const {
	// casting int arrays into double arrays (to be used later)
	std::valarray<double> pixeld = std::valarray<double>(2);
	copy(begin(pixel), end(pixel), begin(pixeld));
	std::valarray<double> resd = std::valarray<double>(2);
	copy(begin(resolution), end(resolution), begin(resd));

	// IDK what this does (ask Licht)
	pixeld = pixeld / (resd / 2.0);
	pixeld[1] = -pixeld[1];
	std::valarray<double> temp = (pixeld * tan(angle / 2.0)) / (resd / 2.0);
	Vec3d v = { temp[0], temp[1], 1 };
	return Line(v, pos);
}
