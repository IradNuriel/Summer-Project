#ifndef _Cloud_H_
#define _Cloud_H_

#include "opencv2/core.hpp"
#include <vector>

class Cloud {
public:
	Cloud(const std::vector<Vec3d> p);
private:
	std::vector<Vec3d> points;
};

#endif