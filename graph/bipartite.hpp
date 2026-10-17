#pragma once
#include "graph_types.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
無向グラフが二部グラフか判定し、各頂点を2色に塗り分ける。
非連結グラフにも対応する。

引数:
`adj`: 重みなしグラフの隣接リスト。頂点番号は0始まり。

返り値:
`std::optional<std::vector<int>>`。
二部グラフなら、各頂点の色を `1` または `-1` で返す。
二部グラフでなければ `std::nullopt` を返す。

計算量:
頂点数 V、辺数 E として O(V + E)。

使用例:
```cpp
koma::Graph g(4);
g[0].push_back(1);
g[1].push_back(0);
g[1].push_back(2);
g[2].push_back(1);
g[2].push_back(3);
g[3].push_back(2);

auto res = koma::bipartite_graph_paint(g);
if(res) {
	auto colors = *res; // {1, -1, 1, -1}
}
```
*/
template <class I>
std::optional<std::vector<int>> bipartite_graph_paint(const std::vector<std::vector<I>> &adj) {
	std::vector<int> colors(adj.size(), 0);
	std::queue<long long> q;

	for(long long s = 0; s < (long long)adj.size(); ++s) {
		if(colors[s]) continue;
		colors[s] = 1;
		q.push(s);

		while(!q.empty()) {
			auto u = q.front();
			q.pop();

			for(auto v : adj[u]) {
				assert(std::cmp_greater_equal(v, 0) && std::cmp_less(v, adj.size()));
				if(colors[v] == colors[u]) return std::nullopt;
				if(colors[v]) continue;
				colors[v] = -colors[u];
				q.push(v);
			}
		}
	}
	return colors;
}

} // namespace koma