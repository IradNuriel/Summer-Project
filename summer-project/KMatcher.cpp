#include "KMatcher.h"

KMatcher::KMatcher(){

}
using namespace std;
template<class T, class S>
bool inMap(map<T, S> m, const T& val) {
	return m.find(val) != m.end();
}
void KMatcher::match(std::vector<Image> imgs) const{
typedef pair<int, int> node;
#define x first
#define y second
	int n = imgs.size();
	vector<vector<vector<node>>> g(n); //graph
	//init photos and graph
	for (int i = 0; i < n; i++) { 
		this->matcher.init_img(imgs[i]);
		g[i].resize(imgs[i].key.size());
	}
	// build graph
	map<pair<node, node>, bool> edges;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; i < n; i++) {
			auto ijMtach = this->matcher.match2(imgs[i], imgs[j]);
			for (auto p : ijMtach) {
				node a = {i, p.queryIdx}, b = {j, p.trainIdx};
				g[a.x][a.y].push_back(b);
				g[b.x][b.y].push_back(a);
				edges[{a, b}] = edges[{b, a}] = 1;
			}
		}
	}
	vector<vector<node>> cliques;
	for (int i = 0; i < n; i++) { //try to find triangle start in i (and not using [0,i-1])
		for (int j = 0; j < g[i].size(); j++) {
			map<node, int> neighbours;
			bool foundTri = false;
			vector<node> clique;
			for (node p : g[i][j]) {
				neighbours[p] = 1;
				if (foundTri) continue;
				for (node pnei : g[p.x][p.y]) {
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
			for (node p : g[i][j]) {
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
			if (clique.size() > 2) cliques.push_back(clique);
		}
	}

#undef x
#undef y
}



