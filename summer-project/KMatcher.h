/////////////////////////////////
// file written by Noam Licht, //
// written in 02/08/2020       //
/////////////////////////////////
#ifndef _KMATCHER_H_
#define _KMATCHER_H_


//data structures includes
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <fstream>

//opencv stuff includes
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"

//our file includes
#include "Matcher.h"


//pKeyPoint, a pair of keypoints
typedef std::pair<cv::KeyPoint, cv::KeyPoint> pKeyPoint;

//KMatcher class, a normal matcher that can match between multiple images
class KMatcher : public Matcher {
public:
	KMatcher();//default c'tor
	std::vector<cv::Vec3d> match(std::vector<Image>& imgs) const;
	
};

#endif
