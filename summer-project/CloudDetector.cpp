#include "CloudDetector.h"

double CloudDetector::XYdistance(const Vec3d & a, const Vec3d & b, int X, int Y){
	return (a[X] - b[X])*(a[X] - b[X]) + (a[Y] - b[Y])*(a[Y] - b[Y]);
}

std::vector<std::vector<int>> CloudDetector::detectGroups(const std::vector<Vec3d>& points) {
	int n = points.size();
	////////////past ideas
	/*
	std::vector<std::vector<int>> g(n);
	std::vector<Edge> edges;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			double ijdist = XYdistance(points[i], points[j]);
			if (ijdist <= Constants::MAX_R_FOR_BODY) {
				g[i].push_back(j);
				g[j].push_back(i);
				edges.push_back({i, j, ijdist});
			}
		}
	}
	std::sort(edges.begin(), edges.end());
	*/
	std::vector<bool> check(n);
	std::vector<std::vector<int>> groups;
	for (int i = 0; i < n; i++) {
		if (check[i]) continue;
		std::vector<int> group(1, i);
		for (int j = i + 1; j < n; j++) {
			if (check[j]) continue;
			double ijdist = XYdistance(points[i], points[j]);
			if (ijdist < Constants::MAX_R_FOR_BODY) {
				group.push_back(j);
			}
		}
		if (group.size() >= Constants::MIN_POINTS_FOR_BODY) {
			for (int ind : group) check[ind] = true;
			groups.push_back(group);
		}
	}
	return groups;
}
