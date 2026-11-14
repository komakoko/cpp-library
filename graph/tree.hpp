#pragma once
#include "adj_list.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
木の重心をすべて求める。

引数:
`adj`: 重みなし無向木の隣接リスト。各辺を両方向に登録する。頂点番号は0始まり。

返り値:
重心の頂点番号を昇順に並べた `std::vector<long long>`。
重心は、その頂点を取り除いた各連結成分の頂点数がすべて N/2 以下になる頂点。
空のグラフなら空配列、空でない木なら1個または2個を返す。

計算量:
頂点数 N として O(N)。再帰は使用しない。

使用例:
```cpp
koma::AdjList g = {{1}, {0, 2}, {1, 3}, {2}};
auto centers = koma::tree_centroids(g); // {1, 2}
```
*/
template <class I>
std::vector<long long> tree_centroids(const std::vector<std::vector<I>> &adj) {
	const long long n = static_cast<long long>(adj.size());
	if(n == 0) return {};
	std::vector<long long> parent(n, -1), order{0}, size(n, 1);
	parent[0] = 0;
	for(long long i = 0; i < static_cast<long long>(order.size()); ++i) {
		long long v = order[i];
		for(auto u : adj[v]) {
			assert(std::cmp_greater_equal(u, 0) && std::cmp_less(u, adj.size()));
			if(parent[u] != -1) continue;
			parent[u] = v;
			order.push_back(u);
		}
	}
	assert(static_cast<long long>(order.size()) == n);
	for(long long i = n - 1; i > 0; --i) {
		long long v = order[i];
		size[parent[v]] += size[v];
	}
	std::vector<long long> result;
	for(long long v = 0; v < n; ++v) {
		long long largest = n - size[v];
		for(auto u : adj[v]) {
			if(parent[u] == v && u != v) largest = std::max(largest, size[u]);
		}
		if(largest <= n / 2) result.push_back(v);
	}
	return result;
}

/**
木の直径をなす経路を1つ求める。

引数:
`adj`: 重みなし無向木の隣接リスト。各辺を両方向に登録する。頂点番号は0始まり。

返り値:
直径の一端から他端まで順に頂点番号を並べた `std::vector<long long>`。
直径の長さ（辺数）は、空でない場合 `static_cast<long long>(path.size()) - 1`。
答えが複数ある場合はそのうち1つを返す。経路の向きは保証しない。
空のグラフなら空配列、1頂点なら `{0}` を返す。

計算量:
頂点数 N として O(N)。再帰は使用しない。

使用例:
```cpp
koma::AdjList g = {{1}, {0, 2}, {1, 3}, {2}};
auto path = koma::tree_diameter(g); // {0, 1, 2, 3} またはその逆順
long long length = static_cast<long long>(path.size()) - 1; // 3
auto u = path.front(), v = path.back(); // 直径の両端
```
*/
template <class I>
std::vector<long long> tree_diameter(const std::vector<std::vector<I>> &adj) {
	const long long n = static_cast<long long>(adj.size());
	if(n == 0) return {};
	std::vector<long long> dist(n), parent(n), order;
	order.reserve(n);
	auto farthest = [&](long long start) {
		std::fill(dist.begin(), dist.end(), -1);
		std::fill(parent.begin(), parent.end(), -1);
		order.clear();
		order.push_back(start);
		dist[start] = 0;
		for(long long i = 0; i < static_cast<long long>(order.size()); ++i) {
			long long v = order[i];
			for(auto u : adj[v]) {
				assert(std::cmp_greater_equal(u, 0) && std::cmp_less(u, adj.size()));
				if(dist[u] != -1) continue;
				dist[u] = dist[v] + 1;
				parent[u] = v;
				order.push_back(u);
			}
		}
		assert(static_cast<long long>(order.size()) == n);
		return order.back();
	};
	long long start = farthest(0);
	long long goal = farthest(start);
	std::vector<long long> path;
	for(long long v = goal; v != -1; v = parent[v]) path.push_back(v);
	std::reverse(path.begin(), path.end());
	return path;
}

/**
木の中心をすべて求める。

引数:
`adj`: 重みなし無向木の隣接リスト。各辺を両方向に登録する。頂点番号は0始まり。

返り値:
中心の頂点番号を昇順に並べた `std::vector<long long>`。
中心は、最も遠い頂点までの距離が最小になる頂点で、直径の経路の中央に位置する。
頂点数のバランスを基準にする重心（`tree_centroids`）とは異なる。
空のグラフなら空配列、空でない木なら1個または2個を返す。

計算量:
頂点数 N として O(N)。再帰は使用しない。

使用例:
```cpp
koma::AdjList g = {{1}, {0, 2}, {1, 3}, {2}};
auto center = koma::tree_centers(g); // {1, 2}
```
*/
template <class I>
std::vector<long long> tree_centers(const std::vector<std::vector<I>> &adj) {
	auto path = tree_diameter(adj);
	const long long size = static_cast<long long>(path.size());
	if(size == 0) return {};
	std::vector<long long> result{path[size / 2]};
	if(size % 2 == 0) result.push_back(path[size / 2 - 1]);
	std::sort(result.begin(), result.end());
	return result;
}

} // namespace koma
