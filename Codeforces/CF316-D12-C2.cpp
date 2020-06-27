// https://codeforces.com/blog/entry/7937
#include <bits/extc++.h> 
using namespace std;

template <typename T, typename C>
class mcmf {
	public:
		static constexpr T eps = (T) 1e-9;
		struct edge {
			int from;
			int to;
			T c;
			T f;
			C cost;
		};
		vector< vector<int> > g;
		vector<edge> edges;
		vector<C> d;
		vector<int> q;
		vector<bool> in_queue;
		vector<int> pe;
		int n;
		int st, fin;
		T flow;
		C cost;
		mcmf(int _n, int _st, int _fin) : n(_n), st(_st), fin(_fin) {
			assert(0 <= st && st < n && 0 <= fin && fin < n && st != fin);
			g.resize(n);
			d.resize(n);
			in_queue.resize(n);
			pe.resize(n);
			flow = 0;
			cost = 0;
		}
		void clear_flow() {
			for (const edge &e : edges) {
				e.f = 0;
			}
			flow = 0;
		}

		void add(int from, int to, T forward_cap, T backward_cap, C cost) {
			assert(0 <= from && from < n && 0 <= to && to < n);
			g[from].push_back((int) edges.size());
			edges.push_back({from, to, forward_cap, 0, cost});
			g[to].push_back((int) edges.size());
			edges.push_back({to, from, backward_cap, 0, -cost});
		}
		bool expath() {
			fill(d.begin(), d.end(), numeric_limits<C>::max());
			q.clear();
			q.push_back(st);
			d[st] = 0;
			in_queue[st] = true;
			int beg = 0;
			bool found = false;
			while (beg < (int) q.size()) {
				int i = q[beg++];
				if (i == fin) {
					found = true;
				}
				in_queue[i] = false;
				for (int id : g[i]) {
					const edge &e = edges[id];
					if (e.c - e.f > eps && d[i] + e.cost < d[e.to]) {
						d[e.to] = d[i] + e.cost;
						pe[e.to] = id;
						if (!in_queue[e.to]) {
							q.push_back(e.to);
							in_queue[e.to] = true;
						}
					}
				}
			}
			if (found) {
				T push = numeric_limits<T>::max();
				int v = fin;
				while (v != st) {
					const edge &e = edges[pe[v]];
					push = min(push, e.c - e.f);
					v = e.from;
				}
				v = fin;
				while (v != st) {
					edge &e = edges[pe[v]];
					e.f += push;
					edge &back = edges[pe[v] ^ 1];
					back.f -= push;
					v = e.from;
				}
				flow += push;
				cost += push * d[fin];
			}
			return found;
		}

		pair<T, C> max_flow_min_cost() {
			while (expath()) {
			}
			return make_pair(flow, cost);
		}
};
const int MAX_SIDE = 81;
int nbRow, nbCol, nbCell;
int mat[MAX_SIDE][MAX_SIDE];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> nbRow >> nbCol;
	for (int row = 0; row < nbRow; ++row) {
		for (int col = 0; col < nbCol; ++col) {
			cin >> mat[row][col];
		}
	}
	nbCell = nbRow*nbCol; 
	int source = nbCell, sink = nbCell+1;
	mcmf<int, int> util(nbCell+2, source, sink);

	auto gid = [&] (int r, int c) { return r*nbCol+c; };
	for (int row = 0; row < nbRow; ++row) {
		for (int col = 0; col < nbCol; ++col) {
			if ((row+col) % 2 == 0) {
				util.add(source, gid(row,col), 1, 0, 0);
				auto con = [&] (int r2, int c2) {
					if (r2 < 0 || r2 >= nbRow || c2 < 0 || c2 >= nbCol) return;
					int cost = (mat[row][col] != mat[r2][c2]);
					util.add(gid(row,col), gid(r2,c2), 1, 0, cost); 
				};
				con(row-1, col);
				con(row+1, col);
				con(row, col-1);
				con(row, col+1);
			} else {
				util.add(gid(row,col), sink, 1, 0, 0);
			}
		}
	}

	cout << util.max_flow_min_cost().second << "\n";
}
