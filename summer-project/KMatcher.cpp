#include "KMatcher.h"

KMatcher::KMatcher(){

}
template<class T, class S>
bool isInMap(std::map<T, S> m, const T& val) {
	return m.find(val) != m.end();
}

std::vector<cv::Vec3d> KMatcher::match(std::vector<Image>& imgs) const {
typedef std::pair<int, int> Node;
    int n = imgs.size();
	std::vector<std::vector<std::vector<Node>>> g(n); //graph
	//init photos and graph
	for (int i = 0; i < n; i++) { 
		this->initImg(imgs[i]);
		g[i].resize(imgs[i].key.size());
	}
	// build graph
	std::map<std::pair<Node, Node>, bool> edges;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			auto ijMtach = this->match2(imgs[i], imgs[j]);
			for (auto p : ijMtach) {
				Node a = {i, p.queryIdx}, b = {j, p.trainIdx};
				g[a.first][a.second].push_back(b);
				g[b.first][b.second].push_back(a);
				edges[{a, b}] = edges[{b, a}] = 1;
			}
		}
	}
	std::vector<std::vector<Node>> cliques;
	std::vector<cv::Vec3d> points;
	for (int i = 0; i < n; i++) { 
		for (int j = 0; j < g[i].size(); j++) { 
			if (g[i][j].size() >= 1) { 
				std::vector<Node> nodes(g[i][j]);
				nodes.push_back({ i,j });
				Cluster clus;
				for (Node node : nodes) {
					int i = node.first;
					cv::Point2d pixel = imgs[i].key[node.second].pt;
					Line line = imgs[i].lb.getLine({ pixel.x,pixel.y });
					clus.add(line);
				}
				if (clus.cost() <= Constants::GOOD_MATCH_COST) {
					if (Constants::DEBUG) std::cout << clus << std::endl;
					points.push_back(clus.getMiddlePoint());
				}
			}
		}
	}
	if (Constants::DEBUG) std::cout << "#Points: "<< points.size() << std::endl;
	return points;
}