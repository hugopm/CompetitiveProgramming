/*
	https://codeforces.com/contest/840/problem/C
	
	Note G(x) = {p prime | exponent of p in x is odd}
	We know that x*y is a square iff G(x) = G(y).
	Consider G(x) as the group of x.
	We want to order elements such that there are no consecutive elements of same group.

	Consider the progressive construction of the permutation. The natural order is to set
	values from left to right (positions order). It doesn't work, too many parameters to remember.

	Think about *interactions*. For the interaction of an element of group G, it DEPENDS only on
	two possibilties : group G, or another group. Hence, we should place all elements of a group AT
	ONCE, and then forget about their groups.
	We can confuse the elements of the same group, and multiply later by size! of each group.

	Hence for each group we want to place all tokens, caring only about relative order with already placed
	elements. In other words, consider already placed elements and slots between them : _X_X_X_X_. Place tokens
	in slots, the order inside the slots is not important.

	The issue with our filling order is that two tokens of the group 1 could be neighboring, then separated by a token
	of group 2. So we need more parameters about the current partial ordering.

	Fortuantely, we can see that pairs are independent, so we only care about the NUMBER of such pairs : number
	of red slots (red = same group, green = different group). Very classical observation in combinatorics.

	So our situation is DP[i][j] = number of ordering of the i first groups with j red slots.
	Transitions? Naively, it's also possible slots fillings (a lot!). But since we have only one parameter in situation
	(red slots), we only care about the variation of this parameter caused by the transition. So, we want to find
	simple characteristics of a transition describing how the number of red slots evolve.

	Observe the evolution of number of red slots if we put X elements in a slot
	- in a green slot : 0 +0 1 2 3 4 ...
	- in a red slot   : 0 -1 0 1 2 3 ...

	It's _almost_ identity. If it was identity, sum of delta = size. So, let's substract identity
	- in a green slot : 0 -1 -1 -1 -1 ...
	- in a red slot   : 0 -2 -2 -2 -2 ...

	Note t[G] and t[R] the number of red/green slots where we put at least one element. Then
	Sum of delta = size - t[G] - 2*t[R]. We found our parameters, so iterate over them.

	For each couple of parameters (t[G], t[R]), we must find the number of corresponding slots fillings.
	We must choose the red slots with at least one element, green slots with at least one element, then split
	elements among t[G]+t[R] slots with at least one element in each. Since all tokens of a group are considered
	equal, we can align them : O_O_O_O_O and place t[G]+t[R]-1 barriers. So split coefficient is :
	binom[size-1][t[G]+t[R]-1]

	The complexity seems to be O(N^4). But if we make a slightly more precise estimation, it's <=
	sum of N*size^2 = N*(sum of size^2) <= N^3 !
*/
#include <bits/stdc++.h>
#define SZ(v) ((int)((v).size()))
using namespace std;
using ll = long long;

template<const int MOD>
struct ModInt
{
	//int x;
	long long x;
	ModInt() : x(0) { }
	ModInt(long long u) : x(u) { if (x >= MOD || x < 0) { x %= MOD; if (x < 0) x += MOD; } } 
	//ModInt(long long u) : x(u) { }
	ModInt(const ModInt &m) : x(m.x) { }
	ModInt& operator=(const ModInt &m) { x = m.x; return *this; }
	friend bool operator==(const ModInt& a, const ModInt& b) { return a.x == b.x; }
	friend bool operator!=(const ModInt& a, const ModInt& b) { return a.x != b.x; }
	friend bool operator<(const ModInt& a, const ModInt& b) { return a.x < b.x; }

	ModInt& operator+=(const ModInt& m) { x += m.x; if (x >= MOD) x -= MOD; return *this; }
	ModInt& operator-=(const ModInt& m) { x -= m.x; if (x < 0) x += MOD; return *this; }
	ModInt& operator*=(const ModInt& m) { x = (1ll*x*m.x)%MOD; return *this; }	

	friend ModInt operator-(const ModInt &a) { ModInt res(a); if (res.x) res.x = MOD-res.x; return res; }
	friend ModInt operator+(const ModInt& a, const ModInt &b) { return ModInt(a) += ModInt(b); }
	friend ModInt operator-(const ModInt& a, const ModInt &b) { return ModInt(a) -= ModInt(b); }
	friend ModInt operator*(const ModInt& a, const ModInt &b) { return ModInt(a) *= ModInt(b); }
	ModInt f2() { return ModInt(x) += ModInt(x); }

	static long long fp(long long u, long long k) {
		long long res = 1;
		while (k > 0) {
			if (k & 1)
				res = (res*u) % MOD;
			u = (u*u) % MOD;
			k /= 2;
		}
		return res;
	}
	ModInt fastpow(long long k) { return ModInt(fp(x, k)); }
	ModInt inv() { return ModInt(fp(x, MOD-2)); }
	static ModInt sign(long long k) { return ((k & 1) ? ModInt(MOD-1) : ModInt(1)); }
	friend ostream& operator << (ostream& out, const ModInt& a) { return out << a.x; }
};

const int m197 = (int)(1e9) + 7;
using Num = ModInt<m197>;

const int upTo = 305;
Num cnt[upTo][upTo];
Num binom[upTo][upTo];
Num fact[upTo];
int nbElem, nbGroup;
vector<int> groups;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	for (int n = 0; n < upTo; ++n) {
		fact[n] = (n ? fact[n-1]*n : 1);
		for (int k = 0; k <= n; ++k) {
			binom[n][k] = (k ? binom[n-1][k] + binom[n-1][k-1] : 1);
		}
	}
	cin >> nbElem;
	map<vector<int>, int> rawGr;
	for (int iElem = 0; iElem < nbElem; ++iElem) {
		int x; cin >> x;
		int cur = x;
		vector<int> setPrime;
		for (int p = 2; p*p <= x; ++p) {
			int expo = 0;
			while (p <= cur && cur % p == 0) {
				cur /= p;
				++expo;
			}
			if (expo&1) setPrime.push_back(p);
		}
		if (cur > 1) setPrime.push_back(cur);
		++rawGr[setPrime];
	}
	for (auto x : rawGr) groups.push_back(x.second);
	nbGroup = SZ(groups);

	cnt[0][0] = 1;
	int sumPrev = 0;
	for (int iGroup = 0; iGroup < nbGroup; ++iGroup) {
		int size = groups[iGroup];
		Num *prev = cnt[iGroup], *suiv = cnt[iGroup+1];
		for (int prevRed = 0; prevRed <= sumPrev; ++prevRed) {
			int prevGreen = (sumPrev+1) - prevRed;
			// green : 0 +0 1 2 3 4
			// red__ : 0 -1 0 1 2 3
			//  size - ng - 2*nr
			for (int tkGreen = 0; tkGreen <= min(size, prevGreen); ++tkGreen) {
				for (int tkRed = 0; tkRed <= min(size-tkGreen, prevRed); ++tkRed) {
					if (tkGreen+tkRed == 0) continue;
					int nextRed = prevRed + size - tkGreen - 2*tkRed;
					Num cRed = binom[prevRed][tkRed];
					Num cGr = binom[prevGreen][tkGreen];
					// X_X|X_X|X : (2,2,1)
					Num cSplit = binom[size-1][tkRed+tkGreen-1];
					suiv[nextRed] += cRed*cGr*cSplit * prev[prevRed]; 
				}
			}
		}	
		sumPrev += size;
	}

	Num res = cnt[nbGroup][0];
	for (int size : groups) res *= fact[size];
	cout << res << "\n";
}
