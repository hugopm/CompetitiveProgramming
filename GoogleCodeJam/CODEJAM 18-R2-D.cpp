#include <bits/stdc++.h>
using namespace std;

int solve() {
	int nbRow, nbCol;
	cin >> nbRow >> nbCol;
	using vb = vector<bool>;
	vector<vb> grid(nbRow, vb(nbCol));
	for (int row = 0; row < nbRow; ++row) {
		for (int col = 0; col < nbCol; ++col) {
			char c; cin >> c;
			grid[row][col] = (c == 'W');
		}
	}

	int ans = 0;
	for (int mask = 0; mask < 16; ++mask) {
		bool ok = false;
		for (int row = 0; row < max(1, nbRow-1); ++row) {
			for (int col = 0; col < max(1, nbCol-1); ++col) {
				bool cond = true;
				for (int i = 0; i < 4; ++i) {
					int nr = min(nbRow-1, row+!!(i&1));
					int nc = min(nbCol-1, col+!!(i&2));
					cond &= (grid[nr][nc] == !!(mask & (1 << i)));
				}
				ok |= cond;
			}
		}
		if (!ok) continue;
		for (int sepRow = 0; sepRow <= nbRow; ++sepRow) {
			for (int sepCol = 0; sepCol <= nbCol; ++sepCol) {
				vector<vb> seen(nbRow, vb(nbCol));
				function<int(int,int)> dfs = [&] (int row, int col) {
					if (!(0 <= row && row < nbRow && 0 <= col && col < nbCol) || seen[row][col])
						return 0;
					seen[row][col] = true;
					int iBit = 0;
					if (row >= sepRow) iBit |= 1;
					if (col >= sepCol) iBit |= 2;
					bool exceptedColor = (mask & (1 << iBit)) > 0;
					if (grid[row][col] != exceptedColor)
						return 0;

					int cnt = 1;
					cnt += dfs(row+1, col);
					cnt += dfs(row-1, col);
					cnt += dfs(row, col+1);
					cnt += dfs(row, col-1);
					return cnt;
				};
				for (int row = 0; row < nbRow; ++row) {
					for (int col = 0; col < nbCol; ++col) {
						ans = max(ans, dfs(row,col));
					}
				}
			}
		}
	}
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	int nbTests;
	cin >> nbTests;
	for (int iTest = 0; iTest < nbTests; ++iTest) {
		cout << "Case #" << iTest+1 << ": " << solve() << "\n";
	}
}
