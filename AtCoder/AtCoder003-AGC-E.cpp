/*
	https://atcoder.jp/contests/agc003/tasks/agc003_e
	
	If q[i] > q[i+1], q[i] is useless. Make queries increasing.
	Divide the final sequence into blocks of size q[i]-q[i-1].
	
	Let cnt[i] = number of times we count element i.
	We can remove an element i if we do cnt[x] += cnt[i], where x was the position of the
	copied by the position i.
	
	Process and pop blocks from right to left, updating cnt to obtain smaller equivalent situation.
	Updates are sum of prefix, so we can maintain cnt in compressed form
	(put one token on i = add 1 to cnt[1..i]).
	Break things correctly (euclidian div : Q*complete circle + a prefix => two tokens).
	
	Then we restore and print cnt[1...n] (suffix sum on tokens).
	
	Complexity: we can guess that the solution runs fast enough, because it's hard to reach large positions multiple times.

	More precisely, look at distinct positions of tokens. We have N "mandatory positions", the end of each block.
	Ignore these. These ones create N "original tokens".
	Then each original token create (at most) one new token (ignoring the mandatory one). In other words it moves.
	At each move, the position of a token is divided by at least 2.
	So we will process at most O(N log 1e18) distinct positions in the map.
*/

#include <bits/stdc++.h>
#define SZ(v) ((int)((v).size()))
using namespace std;
using ll = long long;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	int nbVal, nbQuery;
	cin >> nbVal >> nbQuery;
	vector<ll> queries {nbVal};
	for (int iQuery = 0; iQuery < nbQuery; ++iQuery) {
		ll x; cin >> x;
		while (!queries.empty() && queries.back() >= x) {
			queries.pop_back();
		}
		queries.push_back(x);
	}

	map<ll, ll> tokens;
	tokens[queries.back()-1]++;

	for (int iQuery = SZ(queries)-1; iQuery > 0; --iQuery) {
		ll before = queries[iQuery-1];
		ll deb = before;
		while (!tokens.empty()) {
			auto it = tokens.end(); --it;
			auto [posTok, nbOcc] = *it;
			if (posTok < deb) break;
			ll nbCompl = (posTok-deb)/before;
			tokens[deb-1] += nbOcc*(nbCompl+1);
			ll posMove = (posTok-deb)%before;
			assert(2*posMove <= posTok); // Position of the token divided by 2
			tokens[posMove] += nbOcc;
			tokens.erase(it);
		}	
	}

	for (ll i = queries.front()-1; i >= 1; --i) {
		tokens[i-1] += tokens[i];
	}
	for (int iVal = 0; iVal < nbVal; ++iVal) {
		cout << tokens[iVal] << "\n";
	}
}
