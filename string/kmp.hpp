
#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
KMP法で使用する前処理配列を構築する。
文字列や数列など、等価比較が可能な要素の列に対応する。

引数:
`pattern`: 検索するパターン。ランダムアクセス可能な範囲を指定する。

返り値:
長さ `N+1` の `std::vector<long long>`。
`b[0] = -1`。
`b[i]` は長さ `i` の接頭部分列について、全体を除いた接頭部分列と接尾部分列が一致する最長の長さ。

計算量:
パターンの長さ N として O(N)。

使用例:
```cpp
auto b = koma::kmp_preprocess(std::string("ababaca"));
// {-1, 0, 0, 1, 2, 3, 0, 1}
```
*/
template <std::ranges::random_access_range R>
	requires std::ranges::sized_range<R> &&
			 std::equality_comparable<std::ranges::range_value_t<R>>
std::vector<long long> kmp_preprocess(const R &pattern) {
	long long n = (long long)std::ranges::size(pattern);
	auto first = std::ranges::begin(pattern);

	std::vector<long long> b(n + 1);
	b[0] = -1;

	long long j = -1;
	for(long long i = 0; i < n;) {
		while(j >= 0 && *(first + i) != *(first + j)) j = b[j];
		b[++i] = ++j;
	}
	return b;
}

/**
KMP法を用いて、列の中からパターンが出現する位置をすべて求める。
文字列や数列など、等価比較が可能な要素の列に対応する。
重複する一致もすべて検出する。

引数:
`text`: 検索対象の列。
`pattern`: 検索するパターン。

返り値:
一致する開始位置を0始まりで格納した `std::vector<long long>`。
一致しない場合は空配列。
空のパターンは、末尾を含むすべての境界に一致する。

計算量:
`text` の長さ N、`pattern` の長さ M として O(N + M)。

使用例:
```cpp
std::string s = "abababa";
auto pos = koma::kmp_search(s, std::string("aba"));
// {0, 2, 4}

std::vector<int> a = {1, 2, 1, 2, 1};
std::vector<int> p = {1, 2, 1};
auto res = koma::kmp_search(a, p);
// {0, 2}
```
*/
template <std::ranges::random_access_range R1, std::ranges::random_access_range R2>
	requires std::ranges::sized_range<R1> &&
			 std::ranges::sized_range<R2> &&
			 std::equality_comparable_with<
				 std::ranges::range_reference_t<const R1>,
				 std::ranges::range_reference_t<const R2>>
std::vector<long long> kmp_search(const R1 &text, const R2 &pattern) {
	long long n = (long long)std::ranges::size(text);
	long long m = (long long)std::ranges::size(pattern);

	std::vector<long long> matches;
	if(m == 0) {
		for(long long i = 0; i <= n; ++i) matches.push_back(i);
		return matches;
	}

	auto t = std::ranges::begin(text);
	auto p = std::ranges::begin(pattern);
	auto b = kmp_preprocess(pattern);

	long long j = 0;
	for(long long i = 0; i < n; ++i) {
		while(j >= 0 && *(t + i) != *(p + j)) j = b[j];
		if(++j == m) {
			matches.push_back(i + 1 - j);
			j = b[j];
		}
	}
	return matches;
}

} // namespace koma
