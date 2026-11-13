#pragma once
#include "engine.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
非負のlong long型の乱数を生成する。

引数:
なし。共通の生成器 `rand_engine` を使い、`rand_engine.seed(seed)` で初期化できる。

返り値:
0以上LLONG_MAX以下の整数。

計算量:
O(1)。

使用例:
```cpp
auto x = koma::rand_ll();
```
*/
inline long long rand_ll() {
	return static_cast<long long>(rand_engine() & static_cast<std::uint64_t>(LLONG_MAX));
}

/**
指定した範囲のlong long型の乱数を生成する。

引数:
`l` (オーバーロード): 最小値。
`r` (オーバーロード): 最大値。l <= r。両端を含む。
共通の生成器 `rand_engine` を使う。不正な範囲では `std::invalid_argument`。

返り値:
閉区間 [l, r] の整数。

計算量:
期待 O(1)。

使用例:
```cpp
auto x = koma::rand_ll(-10, 10); // -10以上10以下
```
*/
inline long long rand_ll(long long l, long long r) {
	if(l > r) throw std::invalid_argument("rand_ll: expected l <= r");
	return std::uniform_int_distribution<long long>(l, r)(rand_engine);
}

} // namespace koma
