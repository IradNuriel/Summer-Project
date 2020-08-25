#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
#define _USE_MATH_DEFINES
#include <math.h>

namespace Constants {
	// For filtering matches using Lowe's ratio test
	const float RATIO_THRESH = 0.75f;

	// For determining which matches are "good" matches
	const double GOOD_MATCH_COST = 15; //units: cm^2

//DEBUG printing
	const bool DEBUG = true;


// Number of keypoints
	const int MIN_HESSIAN = 1200;
//ratio of degrees->radians
	const double DEGREE_TO_RADIANS_RATIO = M_PI/180.0;

//maximum distance between points in cloud for person recognition
	const double MAX_R_FOR_BODY = 45; //cm
//minimum number of 3dpoints for person recognition
	const int MIN_POINTS_FOR_BODY = 30;

}
#endif
