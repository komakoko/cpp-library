#pragma once
#include "graph_types.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
ダイクストラ法の計算結果を保持する。

`dist`: 始点から各頂点への最短距離。
`prev`: 最短経路における各頂点の直前の頂点。始点と未到達の頂点は -1。
`inf`: 到達不能を表す値。

`restore_path(goal)` で始点から終点への最短経路を復元できる。
*/
template <class W>
struct DijkstraResult {
	std::vector<W> dist;
	std::vector<long long> prev;
	W inf;

	/**
	始点から指定した頂点への最短経路を復元する。

	引数:
	`goal`: 終点の頂点番号（0始まり）。

	返り値:
	始点から終点までの頂点番号の配列。
	到達不能な場合は空配列を返す。

	計算量:
	経路に含まれる頂点数 L として O(L)。
	*/
	std::vector<long long> restore_path(long long goal) const {
		assert(0 <= goal && goal < (long long)dist.size());
		if(dist[goal] == inf) return {};

		std::vector<long long> path;
		for(long long v = goal; v != -1; v = prev[v]) {
			path.push_back(v);
		}
		std::reverse(path.begin(), path.end());
		return path;
	}
};

/**
非負整数の重みを持つグラフで、始点から各頂点への最短距離を求める。
最短経路の復元にも対応する。

引数:
`start`: 始点の頂点番号（0始まり）。
`adj`: 隣接リスト。各辺は `{行き先, 重み}`。重みは非負整数。
`inf`: 無限大相当の値。省略時は重みの型 `W` の最大値。

返り値:
`DijkstraResult<W>`。
`dist[v]` は始点から頂点 `v` への最短距離。
`prev[v]` は最短経路における直前の頂点。
未到達の距離は `inf`。到達可能な距離は `inf` 未満。

計算量:
頂点数 V、辺数 E として O(V + E log(E+1))。
単純グラフでは O((V+E) log V)。

使用例:
```cpp
koma::WeightedGraph g(3);
g[0].push_back({1, 5});
g[0].push_back({2, 1});
g[2].push_back({1, 2});

auto res = koma::dijkstra(0, g);
auto dist = res.dist;              // {0, 3, 1}
auto path = res.restore_path(1);   // {0, 2, 1}
auto prev = res.prev;              // {-1, 2, 0}
```
*/
template <class I, class W>
DijkstraResult<W> dijkstra(
	long long start,
	const WeightedGraph<W, I> &adj,
	std::type_identity_t<W> inf = std::numeric_limits<W>::max()) {
	static_assert(std::is_integral_v<I> && std::is_integral_v<W>);
	static_assert(!std::is_same_v<I, bool> && !std::is_same_v<W, bool>);

	assert(0 <= start && start < (long long)adj.size());
	assert(inf > 0);

	std::vector<W> dist(adj.size(), inf);
	std::vector<long long> prev(adj.size(), -1);

	using Entry = std::pair<W, long long>; // {距離, 頂点}
	std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> q;

	dist[start] = 0;
	q.emplace(0, start);

	while(!q.empty()) {
		auto [d, u] = q.top();
		q.pop();
		if(d != dist[u]) continue;

		for(auto [v, w] : adj[u]) {
			assert(std::cmp_greater_equal(v, 0) && std::cmp_less(v, adj.size()));
			assert(std::cmp_greater_equal(w, 0));

			if(std::cmp_greater_equal(w, inf - d)) continue;

			W next = d + static_cast<W>(w);
			if(next >= dist[v]) continue;

			dist[v] = next;
			prev[v] = u;
			q.emplace(next, v);
		}
	}

	return {std::move(dist), std::move(prev), inf};
}

} // namespace koma