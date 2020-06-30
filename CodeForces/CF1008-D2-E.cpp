// https://codeforces.com/blog/entry/60572 (second solution)
#include <bits/stdc++.h>
#define chmin(x,v) x = min(x,(v))
#define chmax(x,v) x = max(x,(v))
#define SZ(v) ((int)((v).size()))
using namespace std;
using ll = long long;
using Area = long double;

#ifdef DEBUG
int r = 0;
int ask(ll x, ll y) {
	cout << "quer " << ++r << "\n";
	cout << x << " " << y << endl << flush;
	ll a = 20302309, b = 19032403;
	if (x == a && y == b) return 0;
	if (x > a || y > b) return 3;
	if (x < a) return 1;
	if (y < b) return 2;
	assert(false);
}
#else
int ask(ll x, ll y) {
	cout << x << " " << y << endl;
	int ret; cin >> ret; 
	return ret;
}
#endif

vector<pair<ll, ll>> shape;
ll minY = 1;

Area calc(ll untilX, ll untilY) {
	Area res = 0;
	for (int i = 1; i < SZ(shape); ++i) {
		ll x1 = shape[i-1].first, x2 = min(untilX, shape[i].first-1);
		Area dx = max(0LL, x2-x1+1);
		ll y1 = minY, y2 = min(untilY, shape[i-1].second);
		Area dy = max(0LL, y2-y1+1);
		res += dx*dy;
	}
	return res;
}

int main() {
#ifdef DEBUG
	ll n = 1e18;
#else
	ll n; cin >> n;
#endif
	shape = {{1, n}, {n+1, 0}};

	while (true) {
		Area tot = calc(n,n);
		ll chX; {
			ll lo = 1, ri = n;
			while (lo < ri) {
				ll mid = (lo+ri)/2;
				if (3*calc(mid, n) >= tot) ri = mid;
				else lo = mid+1;
			}
			chX = lo;
		}
		ll chY; {
			ll lo = 1, ri = n;
			while (lo < ri) {
				ll mid = (lo+ri)/2;
				if (3*calc(n, mid) >= tot) ri = mid;
				else lo = mid+1;
			}
			chY = lo;
		}
		int ret = ask(chX, chY);
		assert(chY >= minY);
		if (ret == 0) break;
		if (ret == 1) {
			int i = 0;
			while (shape[i+1].first <= chX) ++i;
			shape.erase(shape.begin(), shape.begin()+i);
			chmax(shape.front().first, chX+1);
		}
		if (ret == 2) {
			minY = chY+1;
		}
		if (ret == 3) {
			shape.push_back({chX, chY-1});
		}
		sort(shape.begin(), shape.end());
		for (int i = 1; i < SZ(shape); ++i) {
			chmin(shape[i].second, shape[i-1].second);
		}
		vector<vector<pair<ll,ll>>::iterator> toDel;
		for (int i = 1; i < SZ(shape); ++i) {
			if (shape[i].second == shape[i-1].second) {
				toDel.push_back(shape.begin()+i);
			}
		}
		reverse(toDel.begin(), toDel.end());
		for (auto x : toDel) shape.erase(x);
	}
}
