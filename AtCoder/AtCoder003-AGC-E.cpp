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
			tokens[(posTok-deb)%before] += nbOcc;
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
