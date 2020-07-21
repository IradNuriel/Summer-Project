#include "LineBuilder.h"

// c'tor
LineBuilder::LineBuilder(valarray<int> res, valarray<double> a)
{
	resolution = res;
	angle = a / 180.0 * M_PI;
}

Line LineBuilder::get_line(Vec3d pos, valarray<int> pixel) const
{
	// casting int arrays into double arrays (to be used later)
	valarray<double> pixeld = valarray<double>(2);
	copy(begin(pixel), end(pixel), begin(pixeld));
	valarray<double> resd = valarray<double>(2);
	copy(begin(resolution), end(resolution), begin(resd));

	// IDK what this does (ask Licht)
	pixeld = pixeld / (resd / 2.0);
	pixeld[1] = -pixeld[1];
	valarray<double> temp = (pixeld * tan(angle / 2.0)) / (resd / 2.0);
	Vec3d v = { temp[0], temp[1], 1 };
	return Line(v, pos);
}
