#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
先頭に0を付けた累積和を求める。

引数:
`a`: constで走査できる列。vector、array、span、list などに対応する。
テンプレート引数 T で集計型を指定できる。省略時は要素型を使うが、
long long より小さい整数型（boolを含む）は long long に広げる。
集計型は0から構築でき、各要素からの変換と加算に対応すること。
計算途中の累積和も集計型の範囲内で使う。modint は法を取った和になる。

返り値:
集計型の std::vector。入力の長さを N として長さ N+1。
s[0] は0、s[i] は先頭 i 要素の和。空の入力なら {0}。
半開区間 [l, r) の和は s[r] - s[l] で取得できる。

計算量:
要素数 N として O(N)。

使用例:
```cpp
std::vector<int> a{2, 3, 1};
auto s = koma::cumsum(a); // vector<long long>{0, 2, 5, 6}
auto total = s[3] - s[1];    // [1, 3) の和: 4
auto wide = koma::cumsum<__int128_t>(a);
```
*/
template <class T = void, class R>
	requires std::ranges::input_range<const R>
auto cumsum(const R &a) {
	using V = std::ranges::range_value_t<const R>;
	using Default = std::conditional_t<std::is_integral_v<V> && sizeof(V) < sizeof(long long), long long, V>;
	using Sum = std::conditional_t<std::is_void_v<T>, Default, T>;
	std::vector<Sum> result;
	if constexpr(std::ranges::sized_range<const R>) result.reserve(std::ranges::size(a) + 1);
	result.push_back(Sum(0));
	for(auto &&value : a) result.push_back(Sum(result.back() + Sum(value)));
	return result;
}

} // namespace koma
