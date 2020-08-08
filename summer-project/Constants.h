#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// For filtering matches using Lowe's ratio test
#define RATIO_THRESH 0.75f;

// For determining which matches are "good" matches
#define GOOD_MATCH_COST 0.1;

// temporary, while we're only using two cameras (this will not be a constant later on)
#define N_KEYPOINTS 2;

// Number of keypoints
#define MIN_HESSIAN 1200;

#endif