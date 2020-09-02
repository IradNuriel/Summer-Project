//////////////////////////////////
// file written by all of us,   //
// written in 27/08/2020        //
//////////////////////////////////

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>


//namespace Constants, contains all the constants in the program
namespace Constants {
// For filtering matches using Lowe's ratio test
	const float RATIO_THRESH = 0.75f;

// For determining which matches are "good" matches
	const double GOOD_MATCH_COST = 150; //units: cm^2

// DEBUG printing
	const bool DEBUG = false;

// Number of keypoints
	const int MIN_HESSIAN = 1200;

//Minimum distance between objects squared(cm^2)
	const int MIN_DISTANCE_SQUARED_BETWEEN_OBJECTS = 40000;

//min number of matches between pattern and image
	const int MIN_MATCHES_IN_CALIBRATION_IMAGES = 1;

// Ratio of degrees->radians
	const double DEGREE_TO_RADIANS_RATIO = M_PI/180.0;

// Maximum distance between points in cloud for person recognition
	const double MAX_R_FOR_BODY = 45; //cm

// Minimum number of 3dpoints for person recognition
	const int MIN_POINTS_FOR_BODY = 30;

// Possible image suffices
	const int N_SUFFICES = 3;
	const std::string SUFFICES[3] =  {"jpg", "jpeg", "png"};

// Calibration directory
	const std::string CALIBRATION_DIR = "randomPattern/";

// Program configuration file name
	const std::string CFG_FILE_NAME = "init.cfg";
}
#endif
