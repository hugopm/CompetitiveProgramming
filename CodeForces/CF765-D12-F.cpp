// https://codeforces.com/blog/entry/50456
#include <bits/stdc++.h>
#define SZ(v) ((int)((v).size()))
using namespace std;
using ll = long long;

template<typename Spec>
struct Segtree {
	using DT = int;
	int nbElem = 0;
	vector<DT> arr;
	Spec spec;

	Segtree(vector<DT> v) {
		nbElem = v.size();
		arr.resize(2*nbElem);
		for (int i = nbElem; i < 2*nbElem; ++i)
			arr[i] = v[i-nbElem];
		for (int i = nbElem-1; i >= 1; --i)
			arr[i] = spec.comb(arr[2*i], arr[2*i+1]);
	}

	Segtree(int t, DT x) : Segtree(vector<DT>(t, x)) { }

	void upd(int pos, DT val) {
		pos += nbElem; arr[pos] = val;
		while (pos > 1) {
			pos /= 2;
			arr[pos] = spec.comb(arr[2*pos], arr[2*pos+1]);
		}
	}

	void add(int pos, DT delta) {
		upd(pos, arr[pos+nbElem]+delta);
	}

	DT get(int lo, int ri) {
		lo += nbElem;
		ri += nbElem+1;
		DT res = arr[lo]; // maybe 0 ?
		while (lo < ri) {
			if (lo & 1)
				res = spec.comb(res, arr[lo++]);
			if (ri & 1)
				res = spec.comb(res, arr[--ri]);
			lo /= 2; ri /= 2;	
		}
		return res;
	}
};

struct Query {
	int lo, ri, idx;
};

template<typename T>
vector<T> compress(vector<T> vec) {
	vector<pair<T, int>> util(SZ(vec));
	for (int i = 0; i < SZ(vec); ++i) {
		util[i] = {vec[i], i};
	}
	sort(util.begin(), util.end());
	vector<int> res = vec;
	int pos = 0;
	for (int i = 0; i < SZ(vec); ++i) {
		if (i > 0 && util[i].first > util[i-1].first) ++pos;
		res[util[i].second] = pos;
	}
	return res;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	int nbElem, nbQuery;
	cin >> nbElem;
	vector<int> vec(nbElem);
	for (int &x : vec) cin >> x;

	cin >> nbQuery;
	vector<int> res(nbQuery, 1e9);
	vector<Query> queries(nbQuery);

	for (int iQuery = 0; iQuery < nbQuery; ++iQuery) {
		auto &x = queries[iQuery];
		cin >> x.lo >> x.ri;
		--x.lo; --x.ri;
		x.idx = iQuery;
	}

	sort(queries.begin(), queries.end(), [&] (auto a, auto b) { return a.ri < b.ri; });

	for (int pass = 0; pass < 2; ++pass) {
		vector<int> cmp = compress(vec);

		struct doMin { int comb(int a, int b) { return min(a, b); }};
		struct doMax { int comb(int a, int b) { return max(a, b); }};
		Segtree<doMin> minDown(nbElem, 1e9); 
		Segtree<doMax> lastSeen(nbElem, -1);

		auto gls = [&] (int lo, int ri) {
			if (lo > ri) return -1;
			else return lastSeen.get(lo, ri);
		};
		auto updMD = [&] (int ind, int val) {
			if (minDown.arr[minDown.nbElem + ind] > val) {
				minDown.upd(ind, val);
			}
		};

		int nextAdd = 0;
		for (auto req : queries) {
			while (nextAdd <= req.ri) {
				int valAdd = vec[nextAdd];
				int prevInd = gls(cmp[nextAdd], nbElem-1);
				while (prevInd != -1) {
					updMD(prevInd, vec[prevInd] - valAdd);
					int delta = vec[prevInd] - valAdd;
					int bound = min(cmp[prevInd]-1, cmp[nextAdd] + (delta+1)/2); 
					prevInd = gls(cmp[nextAdd], bound);
				}
				lastSeen.upd(cmp[nextAdd], nextAdd);
				++nextAdd;
			}
			res[req.idx] = min(res[req.idx], minDown.get(req.lo, req.ri));
		}
		for (int &x : vec) x=1e9-x;
	}

	for (int iQuery = 0; iQuery < nbQuery; ++iQuery) {
		cout << res[iQuery] << "\n";
	}
}
