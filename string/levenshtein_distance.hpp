#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
2つの列のレーベンシュタイン距離（編集距離）を求める。

引数:
`s`: 変換元の列。constでランダムアクセスと要素数の取得ができる範囲。
`t`: 変換先の列。s の要素と等価比較できること。
std::string、std::string_view、文字列リテラル、vector、array、span などに対応する。
string_view に変換できる入力は文字列として扱い、終端のヌル文字を数えない。
要素の挿入・削除・置換のコストはそれぞれ1。同じ要素をそのまま使うコストは0。
std::string の UTF-8文字列ではバイト単位になる。

返り値:
s を t に変換するために必要な操作回数の最小値を long long で返す。
片方が空ならもう片方の長さ、両方が空なら0。

計算量:
列の長さを N、M として時間 O(NM)、空間 O(min(N, M))。
片方が空なら時間・空間ともに O(1)。
ヌル終端文字列から長さを調べる場合は、その文字列の長さに比例する時間が別途かかる。

使用例:
```cpp
auto d = koma::levenshtein_distance("kitten", "sitting"); // 3
auto e = koma::levenshtein_distance("abc", "");          // 3
std::vector<int> a{1, 2, 3}, b{1, 4, 3};
auto f = koma::levenshtein_distance(a, b); // 1
```
*/
template <class R1, class R2>
	requires std::ranges::random_access_range<const R1> && std::ranges::sized_range<const R1> &&
			 std::ranges::random_access_range<const R2> && std::ranges::sized_range<const R2> &&
			 std::equality_comparable_with<std::ranges::range_reference_t<const R1>,
										   std::ranges::range_reference_t<const R2>>
long long levenshtein_distance(const R1 &s, const R2 &t) {
	auto view = []<class R>(const R &range) {
		if constexpr(std::convertible_to<const R &, std::string_view>) return std::string_view(range);
		else return (std::views::all)(range);
	};
	auto a = view(s);
	auto b = view(t);
	auto solve = [](const auto &outer, const auto &inner) {
		const auto n = std::ranges::size(outer);
		const auto m = std::ranges::size(inner);
		if(m == 0) return static_cast<long long>(n);
		std::vector<long long> dp(m + 1);
		std::iota(dp.begin(), dp.end(), 0LL);
		auto first = std::ranges::begin(outer);
		auto second = std::ranges::begin(inner);
		for(std::size_t i = 0; i < n; ++i) {
			long long diagonal = dp[0];
			dp[0] = static_cast<long long>(i) + 1;
			for(std::size_t j = 0; j < m; ++j) {
				long long above = dp[j + 1];
				dp[j + 1] = std::min({above + 1, dp[j] + 1, diagonal + !(*(first + i) == *(second + j))});
				diagonal = above;
			}
		}
		return dp.back();
	};
	if(std::ranges::size(a) < std::ranges::size(b)) return solve(b, a);
	return solve(a, b);
}

/**
2つの文字列のレーベンシュタイン距離（編集距離）を求める。

引数:
`s`: 変換元の文字列。ヌル終端文字列へのポインタも指定できる。
`t`: 変換先の文字列。挿入・削除・置換は各コスト1。char 単位で比較する。

返り値:
必要な操作回数の最小値を long long で返す。片方が空ならもう片方の長さ。

計算量:
長さを N、M として時間 O(NM)、空間 O(min(N, M))。片方が空なら O(1)。
ヌル終端文字列から string_view への変換には、その文字列の長さに比例する時間がかかる。

使用例:
```cpp
const char *s = "kitten";
auto d = koma::levenshtein_distance(s, "sitting"); // 3
```
*/
inline long long levenshtein_distance(std::string_view s, std::string_view t) {
	return levenshtein_distance<std::string_view, std::string_view>(s, t);
}

} // namespace koma
