#ifndef _KMATCHER_H_
#define _KMATCHER_H_
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "Matcher.h"

typedef std::pair<cv::KeyPoint, cv::KeyPoint> pKeyPoint;
class KMatcher : public Matcher {
public:
	KMatcher();//default c'tor
	std::vector<cv::Vec3d> match(std::vector<Image>& imgs) const;
	
};

#endif
