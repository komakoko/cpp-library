#pragma once
#include "graph_types.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
幅優先探索で求めた最短距離と、経路の復元に必要な情報を保持する。

コンストラクタ:
`BfsResult{dist, prev}`: 最短距離と直前の頂点の配列で初期化する。
dist と prev の長さは等しく、bfs の返り値と同じ条件を満たすこと。

関数:
`dist[v]`: 最も近い始点から頂点 v への最短距離（辺数）。未到達は -1。
`prev[v]`: 最短経路における直前の頂点。始点と未到達の頂点は -1。
`restore_path(goal)`: 最も近い始点から goal への最短経路を返す。未到達なら空配列。O(経路長)。

使用例:
```cpp
koma::AdjList g = {{1}, {2}, {}};
auto res = koma::bfs(0, g);
auto path = res.restore_path(2); // {0, 1, 2}
```
*/
struct BfsResult {
	std::vector<long long> dist;
	std::vector<long long> prev;

	/**
	最も近い始点から指定した頂点への最短経路を復元する。

	引数:
	`goal`: 終点の頂点番号（0始まり）。0 <= goal < dist.size()。

	返り値:
	最も近い始点から終点までの頂点番号の std::vector<long long>。
	未到達なら空配列。始点への経路は始点自身だけの配列。

	計算量:
	経路に含まれる頂点数 L として O(L)。

	使用例:
	```cpp
	koma::AdjList g = {{1}, {2}, {}};
	auto res = koma::bfs(0, g);
	auto path = res.restore_path(2); // {0, 1, 2}
	```
	*/
	std::vector<long long> restore_path(long long goal) const {
		assert(0 <= goal && goal < static_cast<long long>(dist.size()));
		if(dist[goal] == -1) return {};
		std::vector<long long> path;
		for(long long v = goal; v != -1; v = prev[v]) path.push_back(v);
		std::reverse(path.begin(), path.end());
		return path;
	}
};

/**
重みなしグラフで、複数の始点のうち最も近い始点からの最短距離と経路を求める。
有向・無向グラフ、自己ループ、多重辺に対応する。

引数:
`starts`: 始点の頂点番号の vector（0始まり）。各要素は 0 <= s < adj.size()。
重複は無視する。空なら全頂点が未到達となる。始点の型 S は bool 以外の整数型。
`adj`: 重みなしグラフの隣接リスト。各要素は行き先の頂点番号。
無向辺は両方向に登録する。頂点番号の型 I は bool 以外の整数型。

返り値:
`BfsResult`。dist[v] は最も近い始点からの最短距離（辺数）、prev[v] は直前の頂点。
始点の距離は0、未到達の距離は -1。始点と未到達の prev は -1。
最短経路が複数ある場合は、そのうち1つを記録する。

計算量:
頂点数 V、辺数 E、始点配列の長さ K として O(V + E + K)。

使用例:
```cpp
koma::AdjList g = {{1, 2}, {3}, {3}, {}, {}};
auto res = koma::bfs(std::vector<long long>{0, 4}, g);
auto dist = res.dist;            // {0, 1, 1, 2, 0}
auto path = res.restore_path(3); // {0, 1, 3}
```
*/
template <class S, class I>
BfsResult bfs(const std::vector<S> &starts, const std::vector<std::vector<I>> &adj) {
	static_assert(std::is_integral_v<S> && !std::is_same_v<S, bool>);
	static_assert(std::is_integral_v<I> && !std::is_same_v<I, bool>);
	std::vector<long long> dist(adj.size(), -1), prev(adj.size(), -1);
	std::queue<long long> q;
	for(auto start : starts) {
		assert(std::cmp_greater_equal(start, 0) && std::cmp_less(start, adj.size()));
		if(dist[start] != -1) continue;
		dist[start] = 0;
		q.push(static_cast<long long>(start));
	}
	while(!q.empty()) {
		long long v = q.front();
		q.pop();
		for(auto u : adj[v]) {
			assert(std::cmp_greater_equal(u, 0) && std::cmp_less(u, adj.size()));
			if(dist[u] != -1) continue;
			dist[u] = dist[v] + 1;
			prev[u] = v;
			q.push(u);
		}
	}
	return {std::move(dist), std::move(prev)};
}

/**
重みなしグラフで、1つの始点から各頂点への最短距離と経路を求める。

引数:
`start` (オーバーロード): 単一の始点の頂点番号。0 <= start < adj.size()。
`adj`: 重みなしグラフの隣接リスト。行き先の型 I は bool 以外の整数型。
有向・無向グラフ、自己ループ、多重辺に対応する。

返り値:
`BfsResult`。dist[v] は最短距離（辺数）、prev[v] は直前の頂点。
未到達の距離は -1。始点と未到達の prev は -1。restore_path で経路を復元できる。

計算量:
頂点数 V、辺数 E として O(V + E)。

使用例:
```cpp
koma::AdjList g = {{1}, {2}, {}};
auto res = koma::bfs(0, g);
auto path = res.restore_path(2); // {0, 1, 2}
```
*/
template <class I>
BfsResult bfs(long long start, const std::vector<std::vector<I>> &adj) {
	return bfs(std::vector<long long>{start}, adj);
}

/**
始点を波括弧で指定し、最も近い始点から各頂点への最短距離と経路を求める。

引数:
`starts` (オーバーロード): 始点の頂点番号の初期化リスト。各要素は 0 <= s < adj.size()。
重複は無視する。空なら全頂点が未到達となる。
`adj`: 重みなしグラフの隣接リスト。行き先の型 I は bool 以外の整数型。

返り値:
`BfsResult`。dist[v] は最も近い始点からの最短距離、prev[v] は直前の頂点。
未到達の距離は -1。始点と未到達の prev は -1。restore_path で経路を復元できる。

計算量:
頂点数 V、辺数 E、始点数 K として O(V + E + K)。

使用例:
```cpp
koma::AdjList g = {{1}, {2}, {}};
auto res = koma::bfs({0, 2}, g);
auto dist = res.dist; // {0, 1, 0}
```
*/
template <class I>
BfsResult bfs(std::initializer_list<long long> starts, const std::vector<std::vector<I>> &adj) {
	return bfs(std::vector<long long>(starts), adj);
}

} // namespace koma
