#pragma once
#include "rand_ll.hpp"
#include "../graph/adj_list.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
指定した頂点数のランダムな木を生成する。

引数:
`n`: 頂点数。0以上。不正な引数では `std::invalid_argument`。
共通の生成器 `rand_engine` を使う。すべての木を等確率で生成するものではない。

返り値:
0始まりの隣接リスト `koma::AdjList`。各辺を両方向に格納する。
n > 0なら連結で、辺数はn-1。n = 0なら空配列。

計算量:
O(n)。

使用例:
```cpp
auto adj = koma::rand_tree(10);
```
*/
inline AdjList rand_tree(long long n) {
	if(n < 0) throw std::invalid_argument("rand_tree: expected n >= 0");
	AdjList adj(n);
	std::vector<long long> order(n);
	std::iota(order.begin(), order.end(), 0LL);
	std::shuffle(order.begin(), order.end(), rand_engine);
	for(long long i = 1; i < n; ++i) {
		long long u = order[i], v = order[rand_ll(0, i - 1)];
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	return adj;
}

} // namespace koma
