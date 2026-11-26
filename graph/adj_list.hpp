#pragma once
#include <bits/stdc++.h>

namespace koma {

using AdjList = std::vector<std::vector<long long>>;

template <class W = long long, class I = long long>
using WeightedAdjList = std::vector<std::vector<std::pair<I, W>>>;

/**
隣接リストのすべての辺の向きを反転する。

引数:
`adj`: 重みなしの隣接リスト。頂点番号の型 I は bool 以外の整数型。
各行き先は 0 <= v < adj.size()。すべての頂点番号が I で表現できること。

返り値:
入力と同じ型の逆向きの隣接リスト。元の adj は変更しない。
自己ループ・多重辺・孤立頂点を保持する。各頂点の隣接リストの順序は保証しない。

計算量:
頂点数 V、辺数 E として O(V + E)。

使用例:
```cpp
koma::AdjList g = {{1, 2}, {2}, {}};
auto rev = koma::reverse_adj_list(g); // {{}, {0}, {0, 1}}
```
*/
template <class I>
std::vector<std::vector<I>> reverse_adj_list(const std::vector<std::vector<I>> &adj) {
	static_assert(std::is_integral_v<I> && !std::is_same_v<I, bool>);
	std::vector<std::vector<I>> reversed(adj.size());
	for(std::size_t u = 0; u < adj.size(); ++u) {
		assert(std::in_range<I>(u));
		for(auto v : adj[u]) {
			assert(std::cmp_greater_equal(v, 0) && std::cmp_less(v, adj.size()));
			reversed[v].push_back(static_cast<I>(u));
		}
	}
	return reversed;
}

/**
重み付き隣接リストのすべての辺の向きを反転する。

引数:
`adj`: 各辺が {行き先, 重み} の隣接リスト。頂点番号の型 I は bool 以外の整数型。
各行き先は 0 <= v < adj.size()。すべての頂点番号が I で表現できること。
重みの型 W はコピー可能であること。

返り値:
入力と同じ型の逆向きの隣接リスト。元の adj は変更しない。
重み・自己ループ・多重辺・孤立頂点を保持する。各頂点の隣接リストの順序は保証しない。

計算量:
頂点数 V、辺数 E として O(V + E)。重みのコピーを O(1) とする。

使用例:
```cpp
koma::WeightedAdjList<> g = {{{1, 5}}, {{2, -2}}, {}};
auto rev = koma::reverse_adj_list(g); // {{}, {{0, 5}}, {{1, -2}}}
```
*/
template <class I, class W>
std::vector<std::vector<std::pair<I, W>>> reverse_adj_list(const std::vector<std::vector<std::pair<I, W>>> &adj) {
	static_assert(std::is_integral_v<I> && !std::is_same_v<I, bool>);
	std::vector<std::vector<std::pair<I, W>>> reversed(adj.size());
	for(std::size_t u = 0; u < adj.size(); ++u) {
		assert(std::in_range<I>(u));
		for(const auto &[v, weight] : adj[u]) {
			assert(std::cmp_greater_equal(v, 0) && std::cmp_less(v, adj.size()));
			reversed[v].emplace_back(static_cast<I>(u), weight);
		}
	}
	return reversed;
}

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
