// https://codeforces.com/blog/entry/16468 (DSU solution)
#include <bits/stdc++.h>
#define int long long 
#define SZ(v) ((int)((v).size()))
using namespace std;

const int MAX_NODE = 1e5;
int nbNode;
vector<pair<int, int>> adj[MAX_NODE];
int dp[MAX_NODE];
int goUp[MAX_NODE];

void dfs(int node, int par) {
	vector<int> disp {0, 0};
	for (auto [child, weight] : adj[node]) if (child != par) {
		dfs(child, node);
		disp.push_back(dp[child]+weight);
	}
	sort(disp.rbegin(), disp.rend());
	dp[node] = disp[0]; 
	for (auto [child, weight] : adj[node]) if (child != par) {
		if (dp[child]+weight != disp[0]) {
			goUp[child] = weight + disp[0];
		} else {
			goUp[child] = weight + disp[1];
		}
	}	
}

void dfs2(int node, int par) {
	dp[node] = max(dp[node], goUp[node]);
	for (auto [child, weight] : adj[node]) if (child != par) {
		goUp[child] = max(goUp[child], weight+goUp[node]);
		dfs2(child, node);
	}
}

struct DSU {
	int nbElem;
	vector<int> repr, weight;

	DSU(int ta) : nbElem(ta), repr(ta), weight(ta, 1) {
		iota(repr.begin(), repr.end(), 0);
	}

	int get(int x) {
		if (repr[x] != x)
			repr[x] = get(repr[x]);
		return repr[x];
	}

	bool unite(int a, int b) {
		a = get(a);
		b = get(b);
		repr[b] = a;
		weight[a] += weight[b];
		return true;
	}
};


signed main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> nbNode;
	for (int iEdge = 0; iEdge < nbNode-1; ++iEdge) {
		int u, v, w;
		cin >> u >> v >> w;
		--u; --v;
		adj[u].emplace_back(v, w);
		adj[v].emplace_back(u, w);
	}
	dfs(0,-1); dfs2(0,-1);
	vector<int> order(nbNode);
	iota(order.begin(), order.end(), 0);
	sort(order.begin(), order.end(), [&] (int x, int y) { return dp[x] > dp[y]; });

	int nbQuery;
	cin >> nbQuery;
	for (int iQuery = 0; iQuery < nbQuery; ++iQuery) {
		int width; cin >> width;
		vector<bool> active(nbNode, false);
		int rawLo = 0;
		DSU uf(nbNode);
		int res = 1;
		for (int rawRi = 0; rawRi < nbNode; ++rawRi) {
			int node = order[rawRi];
			active[node] = true;
			while (dp[order[rawLo]]-dp[node] > width) {
				int toDel = order[rawLo];
				--uf.weight[uf.get(toDel)];
				++rawLo;
			}
			for (auto [neighbor, weight] : adj[node]) {
				if (active[neighbor]) {
					uf.unite(node, neighbor);
				}
			}
			res = max(res, uf.weight[uf.get(node)]);
		}
		cout << res << "\n";
	}
}
