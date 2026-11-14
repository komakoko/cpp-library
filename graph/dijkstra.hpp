#pragma once
#include "adj_list.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
ダイクストラ法の計算結果を保持する。

`dist`: 最も近い始点から各頂点への最短距離。
`prev`: 最短経路における各頂点の直前の頂点。始点と未到達の頂点は -1。
`inf`: 到達不能を表す値。

`restore_path(goal)` で最も近い始点から終点への最短経路を復元できる。
*/
template <class W>
struct DijkstraResult {
	std::vector<W> dist;
	std::vector<long long> prev;
	W inf;

	/**
	最も近い始点から指定した頂点への最短経路を復元する。

	引数:
	`goal`: 終点の頂点番号（0始まり）。

	返り値:
	最も近い始点から終点までの頂点番号の配列。
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
非負整数の重みを持つグラフで、最も近い始点から各頂点への最短距離を求める。
最短経路の復元にも対応する。

引数:
`starts`: 始点の頂点番号の vector（0始まり）。各要素は 0 <= s < adj.size()。
重複は無視する。空なら全頂点が未到達となる。型 S は bool 以外の整数型。
`adj`: 隣接リスト。各辺は `{行き先, 重み}`。重みは非負整数。
`inf`: 無限大相当の正の値。省略時は重みの型 `W` の最大値。

返り値:
`DijkstraResult<W>`。
`dist[v]` は最も近い始点から頂点 `v` への最短距離。
`prev[v]` は最短経路における直前の頂点。
未到達の距離は `inf`。到達可能な距離は `inf` 未満。
各始点の距離は0、prev は -1。最短経路が複数ある場合はそのうち1つを記録する。

計算量:
頂点数 V、辺数 E、始点配列の長さ K として O(V + K + (V+E) log(V+E+1))。

使用例:
```cpp
koma::WeightedAdjList g(3);
g[0].push_back({1, 5});
g[0].push_back({2, 1});
g[2].push_back({1, 2});

auto res = koma::dijkstra(std::vector<long long>{0, 2}, g);
auto dist = res.dist;              // {0, 2, 0}
auto path = res.restore_path(1);   // {2, 1}
auto prev = res.prev;              // {-1, 2, -1}
```
*/
template <class S, class I, class W>
DijkstraResult<W> dijkstra(
	const std::vector<S> &starts,
	const WeightedAdjList<W, I> &adj,
	std::type_identity_t<W> inf = std::numeric_limits<W>::max()) {
	static_assert(std::is_integral_v<I> && std::is_integral_v<W>);
	static_assert(!std::is_same_v<I, bool> && !std::is_same_v<W, bool>);
	static_assert(std::is_integral_v<S> && !std::is_same_v<S, bool>);

	assert(inf > 0);

	std::vector<W> dist(adj.size(), inf);
	std::vector<long long> prev(adj.size(), -1);

	using Entry = std::pair<W, long long>; // {距離, 頂点}
	std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> q;

	for(auto start : starts) {
		assert(std::cmp_greater_equal(start, 0) && std::cmp_less(start, adj.size()));
		if(dist[start] == 0) continue;
		dist[start] = 0;
		q.emplace(0, static_cast<long long>(start));
	}

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

/**
非負整数の重みを持つグラフで、1つの始点からの最短距離と経路を求める。

引数:
`start` (オーバーロード): 単一の始点の頂点番号。0 <= start < adj.size()。
`adj`: 隣接リスト。各辺は {行き先, 重み}。重みは非負整数。
`inf`: 未到達を表す正の値。既定は重みの型 W の最大値。

返り値:
`DijkstraResult<W>`。dist[v] は最短距離、prev[v] は直前の頂点。
未到達の距離は inf。距離が inf 以上の経路は記録しない。
restore_path(goal) で最短経路を復元できる。

計算量:
頂点数 V、辺数 E として O(V + E log(E+1))。

使用例:
```cpp
koma::WeightedAdjList g = {{{1, 5}, {2, 1}}, {}, {{1, 2}}};
auto res = koma::dijkstra(0, g);
auto path = res.restore_path(1); // {0, 2, 1}
```
*/
template <class I, class W>
DijkstraResult<W> dijkstra(
	long long start,
	const WeightedAdjList<W, I> &adj,
	std::type_identity_t<W> inf = std::numeric_limits<W>::max()) {
	return dijkstra(std::vector<long long>{start}, adj, inf);
}

/**
始点を波括弧で指定し、最も近い始点からの最短距離と経路を求める。

引数:
`starts` (オーバーロード): 始点の初期化リスト。各要素は 0 <= s < adj.size()。
重複は無視する。空なら全頂点が未到達となる。
`adj`: 隣接リスト。各辺は {行き先, 重み}。重みは非負整数。
`inf`: 未到達を表す正の値。既定は重みの型 W の最大値。

返り値:
`DijkstraResult<W>`。dist[v] は最も近い始点からの最短距離、prev[v] は直前の頂点。
未到達の距離は inf。距離が inf 以上の経路は記録しない。
restore_path(goal) で最も近い始点からの経路を復元できる。

計算量:
頂点数 V、辺数 E、始点数 K として O(V + K + (V+E) log(V+E+1))。

使用例:
```cpp
koma::WeightedAdjList g = {{{1, 5}, {2, 1}}, {}, {{1, 2}}};
auto res = koma::dijkstra({0, 2}, g);
auto path = res.restore_path(1); // {2, 1}
```
*/
template <class I, class W>
DijkstraResult<W> dijkstra(
	std::initializer_list<long long> starts,
	const WeightedAdjList<W, I> &adj,
	std::type_identity_t<W> inf = std::numeric_limits<W>::max()) {
	return dijkstra(std::vector<long long>(starts), adj, inf);
}

} // namespace koma
