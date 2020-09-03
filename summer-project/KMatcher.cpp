/////////////////////////////////
// file written by Noam Licht, //
// written in 02/08/2020       //
/////////////////////////////////
#include "KMatcher.h"


// inhiritence c'tor
KMatcher::KMatcher(){

}


std::vector<cv::Vec3d> KMatcher::match(std::vector<Image>& imgs) const {
typedef std::pair<int, int> Node; // for easier use and clearity
    int n = imgs.size();
	std::vector<std::vector<std::vector<Node>>> g(n); //graph
	//init photos and graph
	for (int i = 0; i < n; i++) { 
		this->initImg(imgs[i]);
		g[i].resize(imgs[i].key.size());
	}
	// build graph
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			auto ijMtach = this->match2(imgs[i], imgs[j],Constants::DEBUG);
			for (auto p : ijMtach) {
				Node a = {i, p.queryIdx}, b = {j, p.trainIdx};
				g[a.first][a.second].push_back(b);
				g[b.first][b.second].push_back(a);
			}
		}
	}
	std::vector<cv::Vec3d> points;
	for (int i = 0; i < n; i++) {  // for each image
		for (int j = 0; j < g[i].size(); j++) { // for each feature
			if (g[i][j].size() > 0) { // if this feature have matches in other images
				//building the cluster with the matches
				std::vector<Node> nodes(g[i][j]);
				nodes.push_back({ i,j });
				Cluster clus;
				for (Node node : nodes) {
					int i = node.first;
					cv::Point2d pixel = imgs[i].key[node.second].pt; // get pxiel position
					Line line = imgs[i].lb.getLine({ pixel.x,pixel.y }); // get line using LineBuilder object
					clus.add(line); //add to cluster
				}
				if (clus.cost() <= Constants::GOOD_MATCH_COST) { //check whether the cost is low enough
					if (Constants::DEBUG) std::cout << clus << std::endl; 
					points.push_back(clus.getMiddlePoint());
				}
			}
		}
	}
	if (Constants::DEBUG) std::cout << "#Points: "<< points.size() << std::endl;
	return points;
}