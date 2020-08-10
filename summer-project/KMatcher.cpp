#include "KMatcher.h"

KMatcher::KMatcher(){

}
using namespace std;
template<class T, class S>
bool inMap(map<T, S> m, const T& val) {
	return m.find(val) != m.end();
}
void KMatcher::match(std::vector<Image> imgs) const{
typedef pair<int, int> Node;
	int n = imgs.size();
	vector<vector<vector<Node>>> g(n); //graph
	//init photos and graph
	for (int i = 0; i < n; i++) { 
		this->matcher.init_img(imgs[i]);
		g[i].resize(imgs[i].key.size());
	}
	// build graph
	map<pair<Node, Node>, bool> edges;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; i < n; i++) {
			auto ijMtach = this->matcher.match2(imgs[i], imgs[j]);
			for (auto p : ijMtach) {
				Node a = {i, p.queryIdx}, b = {j, p.trainIdx};
				g[a.first][a.second].push_back(b);
				g[b.first][b.second].push_back(a);
				edges[{a, b}] = edges[{b, a}] = 1;
			}
		}
	}
	vector<vector<Node>> cliques;
	vector<Vec3d> points;
	for (int i = 0; i < n; i++) { //try to find triangle start in i (and not using [0,i-1])
		for (int j = 0; j < g[i].size(); j++) {
			map<Node, int> neighbours;
			bool foundTri = false;
			vector<Node> clique;
			for (Node p : g[i][j]) {
				neighbours[p] = 1;
				if (foundTri) continue;
				for (Node pnei : g[p.first][p.second]) {
					if (inMap(neighbours, pnei)) { //found triangle
						foundTri = true;
						clique.push_back({ i,j });
						clique.push_back(p);
						clique.push_back(pnei);
						neighbours[p] = neighbours[pnei] = -1; //already in
					}
				}
			}
			if (!foundTri) continue;
			for (Node p : g[i][j]) {
				if (neighbours[p] == -1) continue; //already in
				bool inClique = true;
				for (auto c : clique) { //check if all agree
					if (!inMap(edges, { c, p })) {
						inClique = false;
						break;
					}
				}
				if (inClique) {
					clique.push_back(p);
				}
			}
			if (clique.size() > 2) {
				Cluster clus;
				for (Node node : clique) {
					int i = node.first;
					cv::Point2d pixel = imgs[i].key[node.second].pt;
					Line line = imgs[i].lb.getLine(imgs[i].pos, {pixel.x,pixel.y});
					clus.add(line);
				}
				if (clus.cost() <= Constants::GOOD_MATCH_COST) {
					points.push_back(clus.getMiddlePoint());
					cliques.push_back(clique);
				}
			}
		}
	}

}



