#pragma once
#include <bits/stdc++.h>

namespace koma {

using AdjList = std::vector<std::vector<long long>>;

template <class W = long long, class I = long long>
using WeightedAdjList = std::vector<std::vector<std::pair<I, W>>>;

namespace detail {

/** 辺の両端を読み込み、0始まりの頂点番号に直す。入力失敗時は std::ios_base::failure。 */
inline std::pair<long long, long long> read_adj_endpoints([[maybe_unused]] long long n, bool one_indexed, std::istream &in) {
	long long u, v;
	if(!(in >> u >> v)) throw std::ios_base::failure("failed to read edge endpoints");
	if(one_indexed) {
		assert(1 <= u && u <= n && 1 <= v && v <= n);
		--u;
		--v;
	} else {
		assert(0 <= u && u < n && 0 <= v && v < n);
	}
	return {u, v};
}

} // namespace detail

/**
辺の一覧を読み込み、重みなしの隣接リストを作る。

引数:
`n`: 頂点数。n >= 0。
`m`: 入力する辺数。m >= 0。無向辺も1本として数える。
`directed`: true なら有向、false（既定）なら無向。
`one_indexed`: true（既定）なら入力の頂点番号は1始まり、false なら0始まり。
`in`: 入力ストリーム。既定は std::cin。各辺を u v の順で読む。頂点番号は範囲内であること。

返り値:
0始まりの AdjList。無向辺は両方向に登録する。
自己ループ・多重辺はそのまま保持する。無向の自己ループは2回登録する。
入力失敗時は std::ios_base::failure を送出する。

計算量:
O(n + m)。

使用例:
```cpp
auto g = koma::read_adj_list(n, m);              // 無向・1始まりの入力
auto directed = koma::read_adj_list(n, m, true); // 有向・1始まりの入力
auto zero = koma::read_adj_list(n, m, false, false); // 無向・0始まりの入力
```
*/
inline AdjList read_adj_list(long long n, long long m, bool directed = false, bool one_indexed = true,
							 std::istream &in = std::cin) {
	assert(n >= 0 && m >= 0);
	AdjList adj(n);
	for(long long i = 0; i < m; ++i) {
		auto [u, v] = detail::read_adj_endpoints(n, one_indexed, in);
		adj[u].push_back(v);
		if(!directed) adj[v].push_back(u);
	}
	return adj;
}

/**
辺の一覧を読み込み、重み付きの隣接リストを作る。

引数:
`n`: 頂点数。n >= 0。
`m`: 入力する辺数。m >= 0。無向辺も1本として数える。
`directed`: true なら有向、false（既定）なら無向。
`one_indexed`: true（既定）なら入力の頂点番号は1始まり、false なら0始まり。
`in`: 入力ストリーム。既定は std::cin。各辺を u v w の順で読む。
頂点番号は範囲内であること。重みの型 W の既定は long long。W は入力可能な型。

返り値:
0始まりの WeightedAdjList<W>。各辺は {行き先, 重み}。
無向辺は両方向に登録する。自己ループ・多重辺は保持し、重みの符号は制限しない。
入力失敗時は std::ios_base::failure を送出する。

計算量:
O(n + m)。

使用例:
```cpp
auto g = koma::read_weighted_adj_list(n, m);
auto directed = koma::read_weighted_adj_list<int>(n, m, true);
```
*/
template <class W = long long>
WeightedAdjList<W> read_weighted_adj_list(long long n, long long m, bool directed = false, bool one_indexed = true,
										  std::istream &in = std::cin) {
	assert(n >= 0 && m >= 0);
	WeightedAdjList<W> adj(n);
	for(long long i = 0; i < m; ++i) {
		auto [u, v] = detail::read_adj_endpoints(n, one_indexed, in);
		W weight;
		if(!(in >> weight)) throw std::ios_base::failure("failed to read edge weight");
		adj[u].emplace_back(v, weight);
		if(!directed) adj[v].emplace_back(u, weight);
	}
	return adj;
}

} // namespace koma
