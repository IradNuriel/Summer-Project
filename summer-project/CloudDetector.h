#ifndef _CloudDetector_H_
#define _CloudDetector_H_
#include "Cluster.h"
#include "Constants.h"
#include <algorithm>

struct Edge {
	int u, v;
	double w;
	bool operator<(const Edge& o) {
		return w < o.w;
	}
};
class CloudDetector {
public:
	//really important to be sure that X,Y is correct
	static double XYdistance(const Vec3d& a, const Vec3d& b, int X = 0, int Y = 2); //distance only on the XY plane (when Z is higth)
	static std::vector<std::vector<int>> detectGroups(const std::vector<Vec3d>& points);
};

#endif
