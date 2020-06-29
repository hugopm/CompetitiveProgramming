#include <bits/stdc++.h>
#define chmin(x, v) x=min(x,(v))
#define SZ(v) ((int)((v).size()))
using namespace std;
using ll = long long;

template<int D, typename T>
struct Vec : public vector<Vec<D - 1, T>> {
  static_assert(D >= 1, "Vector dimension must be greater than zero!");
  template<typename... Args>
  Vec(int n = 0, Args... args) : vector<Vec<D - 1, T>>(n, Vec<D - 1, T>(args...)) {
  }
};
template<typename T>
struct Vec<1, T> : public vector<T> {
  Vec(int n = 0, const T& val = T()) : vector<T>(n, val) {
  }
};

void solve() {
	int nbElem, nbTaken, divi;
	cin >> nbElem >> nbTaken >> divi;
	vector<int> arr(nbElem);
	for (int &x : arr) cin >> x;
	sort(arr.begin(), arr.end());

	vector<int> real;
	vector<int> seen(divi, 0);
	const int maxDelta = 3*divi; 
	for (int iElem = nbTaken-1; iElem >= 0; --iElem) {
		int &a = seen[arr[iElem]%divi];
		if (a < maxDelta) {
			++a;
			real.push_back(arr[iElem]);
		}
	}
	int takReal = SZ(real);
	seen.assign(divi, 0);
	for (int iElem = nbTaken; iElem < nbElem; ++iElem) {
		int &a = seen[arr[iElem]%divi];
		if (a < maxDelta) {
			++a;
			real.push_back(arr[iElem]);
		}
	}
	ll res = accumulate(arr.begin(), arr.begin()+nbTaken, 0LL);
	arr.clear(); nbElem = 0;
	int nbReal = SZ(real); 

	Vec<3, ll> dp(nbReal+1, maxDelta, divi, 1e16);
	dp[0][0][0] = 0;

	for (int iReal = 0; iReal < nbReal; ++iReal) {
		const int rd = real[iReal] % divi;
		for (int delta = 0; delta < maxDelta; ++delta) {
			for (int residu = 0; residu < divi; ++residu) {
				ll cur = dp[iReal][delta][residu];

				if (iReal < takReal) {
					int addResi = residu+rd;
				     	if (addResi >= divi) addResi -= divi;	
					chmin(dp[iReal+1][delta][residu], cur);
					if (delta+1 < maxDelta)
						chmin(dp[iReal+1][delta+1][addResi], cur - real[iReal]);
				} else {
					int minResi = residu-rd;
				     	if (minResi < 0) minResi += divi;	
					chmin(dp[iReal+1][delta][residu], cur);
					if (delta > 0)
						chmin(dp[iReal+1][delta-1][minResi], cur + real[iReal]);
				}

			}
		}
	}

	res += dp[nbReal][0][res%divi];
	if (res >= 1e15) res = -1;
	cout << res << "\n";
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	int nbTests;
	cin >> nbTests;
	for (int iTest = 0; iTest < nbTests; ++iTest) {
		solve();
	}
}
