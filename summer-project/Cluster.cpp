#include "Cluster.h"
#include "Line.h"

Cluster::Cluster() : count(0),
directionVectorRejectionSum(cv::Matx33f::zeros()),
originProjectionSum({ 0, 0, 0 }),
originDistSquredSum(0) {}

Cluster::Cluster(const Cluster& l) {
	this->count = l.count;
	this->originDistSquredSum = l.originDistSquredSum;
	this->originProjectionSum = l.originProjectionSum;
	this->directionVectorRejectionSum = l.directionVectorRejectionSum;
}

Cluster::Cluster(const Line &l) {
	this->count = 1;
	this->directionVectorRejectionSum = cv::Matx33d::eye() - l.getV() * l.getV().t();
	this->originProjectionSum = l.getP();
	this->originDistSquredSum = originProjectionSum.dot(originProjectionSum);
}

void Cluster::add(const Cluster &l) { 
	this->count += l.count;
	this->directionVectorRejectionSum += l.directionVectorRejectionSum;
	this->originProjectionSum += l.originProjectionSum;
	this->originDistSquredSum += l.originDistSquredSum;
}

void Cluster::add(const Line &l) {
	add(Cluster(l));
}

void Cluster::remove(const Cluster &l) { 
	this->count -= l.count;
	this->directionVectorRejectionSum -= l.directionVectorRejectionSum;
	this->originProjectionSum -= l.originProjectionSum;
	this->originDistSquredSum -= l.originDistSquredSum;
}

void Cluster::remove(const Line &l) { 
	remove(Cluster(l));
}

double Cluster::cost(cv::Vec3d x) const {
	return (x.dot(this->directionVectorRejectionSum * x - 2 * this->originProjectionSum) + this->originDistSquredSum) / this->size();
}

cv::Vec3d Cluster::getMiddlePoint() const {
	if (count <= 1) {
		printf("Error");
	}
	return (this->directionVectorRejectionSum).inv() * this->originProjectionSum;
}

double Cluster::cost() const {
	if (count <= 1) {
		return 0;
	}
	return this->cost(this->getMiddlePoint());
}


Line Cluster::toLine() const {
	if (count != 1) {
		printf("Argument error: can't convert Cluster to line since Cluster.size() != 1");
	}
	return Line(this->originProjectionSum, (cv::Matx33d::eye() - this->directionVectorRejectionSum)*cv::Vec3d(1, 1, 1));
}

size_t Cluster::size() const {
	return this->count;
}

std::ostream& operator<<(std::ostream& out, const Cluster cluster) {
	out << "#LINES: "<< cluster.size() << " POINT: " << cluster.getMiddlePoint() << ", COST: " << cluster.cost();
	return out;
}

