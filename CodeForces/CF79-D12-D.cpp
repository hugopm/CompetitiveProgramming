// https://codeforces.com/blog/entry/1839
#include <bits/stdc++.h>
#define SZ(v) ((int)((v).size()))
using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	int nbElem, nbDef, nbScope;
	cin >> nbElem >> nbDef >> nbScope;	
	vector<bool> orig(nbElem+2);
	for (int iDef = 0; iDef < nbDef; ++iDef) {
		int pos; cin >> pos;
		orig[pos] = true;
	}
	vector<int> scopes;
	for (int iScope = 0; iScope < nbScope; ++iScope) {
		int x; cin >> x;
		scopes.push_back(x);
		scopes.push_back(-x);
	}

	vector<vector<int>> dis(nbElem+1);
	auto bfs = [&] (int source) {
		dis[source].assign(nbElem+1, 1e9);
		vector<int> &dist = dis[source];
		queue<int> qq;
		dist[source] = 0;
		qq.push(source);
		while (!qq.empty()) {
			int node = qq.front();
			qq.pop();
			for (int scope : scopes) {
				int neighbour = node+scope;
				if (0 <= neighbour && neighbour < nbElem+1 && dist[node]+1 < dist[neighbour]) {
					dist[neighbour] = dist[node]+1;
					qq.push(neighbour);
				}
			}
		}
	};
	vector<int> tokens;
	for (int pos = 0; pos < nbElem+1; ++pos) {
		if (orig[pos]^orig[pos+1]) {
			tokens.push_back(pos); 
			bfs(pos);
		}
	}
	int nbToken = SZ(tokens);
	vector<int> dp(1 << nbToken, 1e9);
	dp[0] = 0;
	for (int mask = 1; mask < (1<<nbToken); ++mask) {
		int fi = -1;
		for (int iToken = 0; iToken < nbToken; ++iToken) {
			if (!(mask & (1<<iToken))) continue;
			if (fi == -1) { fi = iToken; continue; }
			int suiv = mask ^ (1<<fi) ^ (1<<iToken);
			int u1 = tokens[fi], u2 = tokens[iToken];
			dp[mask] = min(dp[mask], dp[suiv] + dis[u1][u2]);
		}
	}
	int res = dp[(1<<nbToken)-1];
	cout << (res < 1e9 ? res : -1) << "\n";
}
