#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
Manacher法を用いて、各位置を中心とする最長回文の半径を求める。
奇数長・偶数長の回文の両方を求める。

引数:
`s`: 対象の列。ランダムアクセス可能な範囲を指定する。

返り値:
長さ `2*N-1` の `std::vector<long long>`。空の列の場合は空配列。
`res[2*i]`: 要素 `i` を中心とする奇数長回文の半径。
`res[2*i+1]`: 要素 `i` と `i+1` の間を中心とする偶数長回文の半径。
奇数長回文の長さは `2*r-1`、偶数長回文の長さは `2*r`。

計算量:
列の長さ N として O(N)。

使用例:
```cpp
auto r1 = koma::manacher(std::string("abba"));
// {1, 0, 1, 2, 1, 0, 1}

std::vector<int> a = {1, 2, 3, 2, 1};
auto r2 = koma::manacher(a);
// {1, 0, 1, 0, 3, 0, 1, 0, 1}
```
*/
template <std::ranges::random_access_range R>
	requires std::ranges::sized_range<R> &&
			 std::equality_comparable<std::ranges::range_value_t<R>>
std::vector<long long> manacher(const R &s) {
	long long n = (long long)std::ranges::size(s);
	if(n == 0) return {};

	auto first = std::ranges::begin(s);
	auto eq = [&](long long i, long long j) {
		return *(first + i) == *(first + j);
	};

	std::vector<long long> odd(n), even(n);

	for(long long i = 0, l = 0, r = -1; i < n; ++i) {
		long long k = (i > r) ? 1 : std::min(odd[l + r - i], r - i + 1);

		while(i - k >= 0 && i + k < n && eq(i - k, i + k)) ++k;

		odd[i] = k;
		if(i + k - 1 > r) {
			l = i - k + 1;
			r = i + k - 1;
		}
	}

	for(long long i = 0, l = 0, r = -1; i < n; ++i) {
		long long k = (i > r) ? 0 : std::min(even[l + r - i + 1], r - i + 1);

		while(i - k - 1 >= 0 && i + k < n && eq(i - k - 1, i + k)) ++k;

		even[i] = k;
		if(i + k - 1 > r) {
			l = i - k;
			r = i + k - 1;
		}
	}

	std::vector<long long> res(2 * n - 1);
	for(long long i = 0; i < n; ++i) {
		res[2 * i] = odd[i];
		if(i + 1 < n) res[2 * i + 1] = even[i + 1];
	}
	return res;
}

} // namespace koma