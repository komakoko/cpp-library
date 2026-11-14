#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
列に含まれる各値の出現回数を数え、値をキーとするmapを返す。

引数:
`a`: constで走査できる列。vector、array、string、string_view などに対応する。
要素の < が厳密弱順序を満たし、要素をコピーできること。
string_view に変換できる入力は文字列として扱い、終端のヌル文字を数えない。

返り値:
要素型を T として std::map<T, long long>。キーは昇順で、値は出現回数。
空の入力なら空のmap。返り値への cnt[x] は、未登録の x を個数0で登録する。

計算量:
要素数 N、異なる値の数 K として O(N log(K+1))。
ヌル終端文字列から長さを調べる場合は、その長さに比例する時間もかかる。

使用例:
```cpp
auto cnt = koma::counter(std::vector<int>{3, 1, 3, 2, 3});
auto n = cnt[3]; // 3
auto chars = koma::counter("banana"); // {'a': 3, 'b': 1, 'n': 2}
```
*/
template <class R>
	requires std::ranges::input_range<const R>
auto counter(const R &a) {
	auto data = [&] {
		if constexpr(std::convertible_to<const R &, std::string_view>) return std::string_view(a);
		else return (std::views::all)(a);
	}();
	using T = std::ranges::range_value_t<decltype(data)>;
	std::map<T, long long> result;
	for(auto &&value : data) ++result[value];
	return result;
}

/**
整数列に含まれる各値の出現回数を数え、値を添字とするvectorを返す。

引数:
`a`: constで走査できる整数列。全要素が 0 以上 m 未満であること。
`m` (オーバーロード): 取りうる値の範囲 [0, m) の大きさ。m >= 0。
値の種類数ではなく、添字として使う値をすべて含む大きさを指定する。
文字列リテラルなど string_view に変換できる入力は、終端のヌル文字を数えない。

返り値:
長さ m の std::vector<long long>。cnt[x] は x の出現回数。
出現しない値の個数は0。空の入力でも長さ m の0埋め配列を返す。

計算量:
要素数 N として時間 O(N + m)、空間 O(m)。

使用例:
```cpp
auto cnt = koma::counter(std::vector<int>{3, 1, 3, 2, 3}, 5);
// {0, 1, 1, 3, 0}
```
*/
template <class R>
	requires std::ranges::input_range<const R> && std::integral<std::ranges::range_value_t<const R>>
std::vector<long long> counter(const R &a, int m) {
	assert(m >= 0);
	std::vector<long long> result(m, 0);
	auto data = [&] {
		if constexpr(std::convertible_to<const R &, std::string_view>) return std::string_view(a);
		else return (std::views::all)(a);
	}();
	for(auto &&x : data) {
		auto value = +x;
		assert(std::cmp_greater_equal(value, 0) && std::cmp_less(value, m));
		++result[static_cast<std::size_t>(value)];
	}
	return result;
}

} // namespace koma
