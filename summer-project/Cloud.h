#ifndef _Cloud_H_
#define _Cloud_H_

#include "opencv2/core.hpp"
#include <vector>

class Cloud {
public:
	Cloud(const std::vector<Vec3d> p);
	insert(const Vec3d point);
private:
	std::vector<Vec3d> points;
};

#endif