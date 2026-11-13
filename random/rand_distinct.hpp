#pragma once
#include "rand_ll.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
指定した整数範囲から重複なしでランダムに選ぶ。

引数:
`l`: 最小値。
`r`: 最大値。l <= r。両端を含む。
`k`: 選ぶ個数。0 <= k <= r-l+1。
共通の生成器 `rand_engine` を使う。不正な引数では `std::invalid_argument`。

返り値:
相異なるk個の整数をランダムな順番に並べた `std::vector<long long>`。
k = 0なら空配列。範囲全体を列挙せずに生成する。

計算量:
期待 O(k)。

使用例:
```cpp
auto v = koma::rand_distinct(1, 10, 2);
auto a = v[0], b = v[1]; // 必ずa != b
```
*/
inline std::vector<long long> rand_distinct(long long l, long long r, long long k) {
	if(l > r || k < 0 || static_cast<__int128>(k) > static_cast<__int128>(r) - l + 1)
		throw std::invalid_argument("rand_distinct: expected l <= r and 0 <= k <= r-l+1");
	std::unordered_set<long long> chosen;
	chosen.reserve(static_cast<std::size_t>(k));
	std::vector<long long> result;
	result.reserve(static_cast<std::size_t>(k));
	for(long long i = 0; i < k; ++i) {
		long long upper = static_cast<long long>(static_cast<__int128>(r) - k + 1 + i);
		long long x = rand_ll(l, upper);
		if(!chosen.insert(x).second) {
			x = upper;
			chosen.insert(x);
		}
		result.push_back(x);
	}
	std::shuffle(result.begin(), result.end(), rand_engine);
	return result;
}

} // namespace koma
