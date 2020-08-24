#ifndef _KMATCHER_H_
#define _KMATCHER_H_
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "Matcher.h"


typedef std::pair<cv::KeyPoint, cv::KeyPoint> pKeyPoint;
class KMatcher : public Matcher{
public:
	KMatcher();//defult c'tor
	void match(std::vector<Image>& imgs) const;

};
void printAllPairsWithTooLowDistance(std::vector<std::pair<cv::Vec3d, cv::Vec3d>> coordsPairsList, int minDistance = 2);

#endif
