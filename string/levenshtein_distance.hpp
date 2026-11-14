#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
2つの文字列のレーベンシュタイン距離（編集距離）を求める。

引数:
`s`: 変換元の文字列。std::string、std::string_view、文字列リテラルを指定できる。
`t`: 変換先の文字列。
文字の挿入・削除・置換のコストはそれぞれ1。同じ文字をそのまま使うコストは0。
文字は char 単位で比較する。UTF-8文字列ではバイト単位になる。

返り値:
s を t に変換するために必要な操作回数の最小値を long long で返す。
片方が空ならもう片方の長さ、両方が空なら0。

計算量:
文字列の長さを N、M として時間 O(NM)、空間 O(min(N, M))。
片方が空なら時間・空間ともに O(1)。

使用例:
```cpp
auto d = koma::levenshtein_distance("kitten", "sitting"); // 3
auto e = koma::levenshtein_distance("abc", "");          // 3
```
*/
inline long long levenshtein_distance(std::string_view s, std::string_view t) {
	if(s.size() < t.size()) std::swap(s, t);
	if(t.empty()) return static_cast<long long>(s.size());
	std::vector<long long> dp(t.size() + 1);
	std::iota(dp.begin(), dp.end(), 0LL);
	for(std::size_t i = 0; i < s.size(); ++i) {
		long long diagonal = dp[0];
		dp[0] = static_cast<long long>(i) + 1;
		for(std::size_t j = 0; j < t.size(); ++j) {
			long long above = dp[j + 1];
			dp[j + 1] = std::min({above + 1, dp[j] + 1, diagonal + (s[i] != t[j])});
			diagonal = above;
		}
	}
	return dp.back();
}

} // namespace koma
