#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

namespace Constants {
// For filtering matches using Lowe's ratio test
	const float RATIO_THRESH = 0.75f;

// For determining which matches are "good" matches
	const double GOOD_MATCH_COST = 15; //units: cm^2

// DEBUG printing
	const bool DEBUG = true;

// Number of keypoints
	const int MIN_HESSIAN = 1200;

// Ratio of degrees->radians
	const double DEGREE_TO_RADIANS_RATIO = M_PI/180.0;

// Maximum distance between points in cloud for person recognition
	const double MAX_R_FOR_BODY = 45; //cm

// Minimum number of 3dpoints for person recognition
	const int MIN_POINTS_FOR_BODY = 30;

// Possible image suffices
	const int N_SUFFICES = 3;
	const std::string SUFFICES[3] =  {"jpg", "jpeg", "png"};
}
#endif
