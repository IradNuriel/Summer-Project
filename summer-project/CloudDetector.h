/////////////////////////////////
// file written by Noam Licht, //
// written in 25/08/2020       //
/////////////////////////////////

#ifndef _CloudDetector_H_
#define _CloudDetector_H_

//include algorithm
#include <algorithm>

//include our files
#include "Cluster.h"
#include "Constants.h"
#include "Cloud.h"

//struct edge
struct Edge {
	int u, v;
	double w;
	bool operator<(const Edge& o) {
		return w < o.w;
	}
};


//namespace CloudDetector, getting 3d points and detecting heavy clouds in it
namespace CloudDetector {
	//really important to be sure that X,Y is correct
	static double XYdistance(const cv::Vec3d& a, const cv::Vec3d& b, int X = 0, int Y = 2) { // distance only on the XY plane (when Z is height)
		return (a[X] - b[X])*(a[X] - b[X]) + (a[Y] - b[Y])*(a[Y] - b[Y]);
	}
	static std::vector<Cloud> detectGroups(const std::vector<cv::Vec3d>& points) {
		int n = points.size();
		std::vector<bool> check(n);
		std::vector<Cloud> clouds;
		for (int i = 0; i < n; i++) {

			// if this node is already in a cloud, skip it
			if (check[i]) continue;

			// find nearby free nodes
			std::vector<int> group(1, i);
			for (int j = i + 1; j < n; j++) {
				if (check[j]) continue;
				double ijdist = XYdistance(points[i], points[j]);
				if (ijdist < Constants::MAX_R_FOR_BODY * Constants::MAX_R_FOR_BODY) {
					group.push_back(j);
				}
			}

			// check if the group size 
			if (group.size() >= Constants::MIN_POINTS_FOR_BODY) {
				// add the new cloud to the cloud group
				Cloud cloud;
				for (int ind : group) {
					// mark the points so that they don't get included in another cloud
					check[ind] = true;
					cloud.insert(points[ind]);
				}
				clouds.push_back(cloud);
			}
		}
		return clouds;
	}
};

#endif