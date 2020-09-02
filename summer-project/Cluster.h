/////////////////////////////////
// file written by Irad Nuriel,//
// written in 26/07/2020       //
/////////////////////////////////

#ifndef _CLUSTER_H_
#define _CLUSTER_H_
#include "Line.h"
#include <ostream>
/* 
Class based on Tomer Wolberg code


Cluster is a data-structure for a set of lines L
   that can answer the following queries in O(1):

   Initialize():
	   L = {}

   Add(l):
	   L = L U {l}

   Add(L2):
	   L = L U L2

   Remove(l):
	   L = L \ {l}

   Remove(L2):
	   L = L \ L2

   Cost(p):
	   return Sum(dist^2(L[i], p))

   MiddlePoint():
	   return minarg_p(Cost(p))
   Cost():
	   return Cost(MiddlePoint())*/
class Cluster {
public:
	Cluster();/*default constructor for class Cluster*/
	Cluster(const Cluster& l);//copy c'tor
	Cluster(const Line &l);//constructor of class Cluster, which begin with one line in it
	void add(const Cluster &l);//function add(cluster), combine 2 clusters 
	void add(const Line &l);//function add(line), add line to the cluster
	void remove(const Cluster &l);//function remove(cluster), remove one cluster form another 
	void remove(const Line &l);//function remove(line), remove given line from the cluster 
	double cost(cv::Vec3d x) const;/*function cost,
								return the sum of squared distances of x from the lines in the structure*/
	cv::Vec3d getMiddlePoint() const;/*function getMiddlePoint,
								  get the point that gives the minimum sum of squared distances,
								  from the lines in the cluster.*/
	double cost() const;/*return the cost of the point,
						 that brings the sum of squared distances to the lines in the structures,
						 to the minimum*/
	Line toLine() const;//function toLine, will only work if there is only one line in the cluster.
	size_t size() const;//function size, return the amount of lines in the structure.
	

protected:
	size_t  count;
	cv::Matx33d directionVectorRejectionSum;  // Sum of the rejection matrices of the lines direction vectors (v).
	cv::Vec3d   originProjectionSum; // Sum of the projections of the origin on the lines.               
	double  originDistSquredSum; // Sum of the squared distances of the lines from the origin.        
};

std::ostream& operator<<(std::ostream& out, const Cluster cluster);

#endif
