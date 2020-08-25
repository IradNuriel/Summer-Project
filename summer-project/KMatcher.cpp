#include "KMatcher.h"

void printAllPairsWithTooLowDistance(std::vector<std::pair<cv::Vec3d,cv::Vec3d>> coordsPairsList,int minDistance){
	for (std::pair<cv::Vec3d, cv::Vec3d> coordPair : coordsPairsList) {
		int distSquared = coordPair.first.dot(coordPair.first) + coordPair.second.dot(coordPair.second);
		if (distSquared <= (minDistance*minDistance)) {
			std::cout << "the pair: " << std::endl << "{coord 1: " << coordPair.first << std::endl << "coord 2: " << coordPair.second << "}" << std::endl;
		}
	}
}

KMatcher::KMatcher(){

}
template<class T, class S>
bool isInMap(std::map<T, S> m, const T& val) {
	return m.find(val) != m.end();
}
void KMatcher::match(std::vector<Image>& imgs) const{
typedef std::pair<int, int> Node;
    int n = imgs.size();
	std::ofstream outfile;
	outfile.open("D:\\points.txt");
	outfile.flush();
	std::vector<std::vector<std::vector<Node>>> g(n); //graph
	//init photos and graph
	for (int i = 0; i < n; i++) { 
		this->initImg(imgs[i]);
		g[i].resize(imgs[i].key.size());
	}
	// build graph
	std::map<std::pair<Node, Node>, bool> edges;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; i < n; i++) {
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
		for (int j = 0; j < g[i].size(); j++) { //try to find triangle include (i,j)
			/*map<Node, int> neighbours;
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
					if (!isInMap(edges, { c, p })) {
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
					clus.add(line);				}
				if (clus.cost() <= Constants::GOOD_MATCH_COST) {
					cout << clus << endl;
					points.push_back(clus.getMiddlePoint());
					cliques.push_back(clique);
				}
			}*/
			if (g[i][j].size() > 1) { //dunb solution beacuse clique-based solution (^) works terrible
				std::vector<Node> nodes(g[i][j]);
				nodes.push_back({ i,j });
				Cluster clus;
				for (Node node : nodes) {
					int i = node.first;
					cv::Point2d pixel = imgs[i].key[node.second].pt;
					Line line = imgs[i].lb.getLine(imgs[i].pos, { pixel.x,pixel.y });
					clus.add(line);
				}
				if (clus.cost() <= Constants::GOOD_MATCH_COST) {
					std::cout << clus << std::endl;
					
					outfile << clus.getMiddlePoint() << std::endl;
					points.push_back(clus.getMiddlePoint());
				}
			}
		}
	}
	outfile.close();
	std::cout << "#Points: "<< points.size() << std::endl;
}