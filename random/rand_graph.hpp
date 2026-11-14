#pragma once
#include "rand_ll.hpp"
#include "../graph/adj_list.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
指定した頂点数と辺数のランダムな単純無向グラフを生成する。

引数:
`n`: 頂点数。0以上。
`m`: 辺数。0 <= m <= n(n-1)/2。連結性は保証しない。
共通の生成器 `rand_engine` を使う。
n(n-1)/2はlong longに収まること。不正な引数では `std::invalid_argument`。

返り値:
0始まりの隣接リスト `koma::AdjList`。各辺を両方向に格納する。
自己ループ・多重辺はない。完全グラフに近い場合も再抽選を繰り返さない。

計算量:
期待 O(n + m log(n+1))。

使用例:
```cpp
auto adj = koma::rand_graph(5, 7);
for(long long u = 0; u < 5; ++u)
    for(auto v : adj[u])
        if(u < v) std::cout << u + 1 << " " << v + 1 << "\n";
```
*/
inline AdjList rand_graph(long long n, long long m) {
	if(n < 0 || m < 0) throw std::invalid_argument("rand_graph: expected n >= 0 and m >= 0");
	__int128 count = static_cast<__int128>(n) * (n - 1) / 2;
	if(count > LLONG_MAX || m > count)
		throw std::invalid_argument("rand_graph: expected m <= n(n-1)/2 <= LLONG_MAX");
	long long total = static_cast<long long>(count);
	std::unordered_set<long long> edges;
	edges.reserve(static_cast<std::size_t>(m));
	for(long long j = total - m; j < total; ++j) {
		long long x = rand_ll(0, j);
		if(!edges.insert(x).second) edges.insert(j);
	}
	AdjList adj(n);
	for(long long x : edges) {
		long long low = 0, high = n - 1;
		while(high - low > 1) {
			long long mid = low + (high - low) / 2;
			__int128 prefix = static_cast<__int128>(mid) * (2 * static_cast<__int128>(n) - mid - 1) / 2;
			if(prefix <= x) low = mid;
			else high = mid;
		}
		long long prefix = static_cast<long long>(static_cast<__int128>(low) * (2 * static_cast<__int128>(n) - low - 1) / 2);
		long long v = low + 1 + (x - prefix);
		adj[low].push_back(v);
		adj[v].push_back(low);
	}
	return adj;
}

} // namespace koma
