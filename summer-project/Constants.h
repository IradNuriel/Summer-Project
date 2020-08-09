#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
namespace Constants {
	// For filtering matches using Lowe's ratio test
	const float RATIO_THRESH = 0.75f;

		// For determining which matches are "good" matches
	const double GOOD_MATCH_COST = 0.1;


// temporary, while we're only using two cameras (this will not be a constant later on)
	const int N_KEYPOINTS = 2;

// Number of keypoints
	const int MIN_HESSIAN = 1200;
}
#endif