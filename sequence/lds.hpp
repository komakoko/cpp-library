#pragma once
#include "lis.hpp"
#include <bits/stdc++.h>

namespace koma {

/**
最長減少部分列（LDS）の長さ、DP配列、復元情報を保持する。LisResult と同じ型。

コンストラクタ:
`LdsResult()`: 空の列の結果として構築する。通常は lds(a, strict) の返り値を使う。

関数:
`length`: LDSの長さ。`dp[i]`: i を末尾とする長さ。`suffix_dp[i]`: i を先頭とする長さ。
`prev[i]` / `next[i]`: 復元時の直前・直後の添字。存在しない場合は -1。
`tails[k]`: 長さ k+1 の減少部分列について、末尾の値が最大となる要素の添字。
`restore_indices()`: 全体のLDSを復元する。O(length)。
`restore_indices(end)`: end を末尾とするLDSを復元する。O(dp[end])。
`restore_indices_from(start)`: start を先頭とするLDSを復元する。O(suffix_dp[start])。
復元結果は0始まりの添字列。複数の答えがある場合はそのうち1つで、辞書順最小は保証しない。

使用例:
```cpp
auto res = koma::lds(std::vector<int>{5, 3, 4, 2});
auto ids = res.restore_indices(); // 例: {0, 2, 3}
```
*/
using LdsResult = LisResult;

/**
最長減少部分列（LDS）の長さ、DP配列、先頭・末尾指定での復元情報を求める。

引数:
`a`: constでランダムアクセスと要素数の取得ができる列。要素の < が厳密弱順序を満たすこと。
vector、array、span、string などに対応する。
`strict`: true（既定）なら狭義減少（前 > 次）。false なら同じ値を許す広義減少（前 >= 次）。

返り値:
`LdsResult`。length は全体の長さ、dp[i] は i を末尾とする長さ、suffix_dp[i] は i を先頭とする長さ。
restore_indices()、restore_indices(end)、restore_indices_from(start) で部分列を復元できる。
空の列なら length は0、各配列は空。

計算量:
要素数 N、答えの長さ L として時間 O(N log(L+1))、空間 O(N)。

使用例:
```cpp
std::vector<int> a{5, 3, 3, 1};
auto res = koma::lds(a);         // length == 3
auto non_strict = koma::lds(a, false); // length == 4
auto ids = res.restore_indices_from(1); // {1, 3}
```
*/
template <class R>
	requires std::ranges::random_access_range<const R> && std::ranges::sized_range<const R> &&
			 std::strict_weak_order<std::less<>, std::ranges::range_reference_t<const R>,
									std::ranges::range_reference_t<const R>>
LdsResult lds(const R &a, bool strict = true) {
	return detail::monotone_subsequence_result(a, strict, true);
}

/**
最長減少部分列（LDS）の長さを求める。

引数:
`a`: constでランダムアクセスと要素数の取得ができる列。要素の < が厳密弱順序を満たすこと。
`strict`: true（既定）なら狭義減少、false なら同じ値を許す広義減少。

返り値:
LDSの長さを long long で返す。空の列なら0。

計算量:
要素数 N、答えの長さ L として時間 O(N log(L+1))、空間 O(L)。

使用例:
```cpp
auto n = koma::lds_length(std::vector<int>{5, 3, 3, 1}); // 3
```
*/
template <class R>
	requires std::ranges::random_access_range<const R> && std::ranges::sized_range<const R> &&
			 std::strict_weak_order<std::less<>, std::ranges::range_reference_t<const R>,
									std::ranges::range_reference_t<const R>>
long long lds_length(const R &a, bool strict = true) {
	return static_cast<long long>(detail::lis_tails(a, strict, nullptr, nullptr, false, true).size());
}

/**
最長減少部分列（LDS）の添字を1つ復元する。

引数:
`a`: constでランダムアクセスと要素数の取得ができる列。要素の < が厳密弱順序を満たすこと。
`strict`: true（既定）なら狭義減少、false なら同じ値を許す広義減少。

返り値:
0始まりの添字を元の列における順序で並べた std::vector<long long>。
空の列なら空配列。複数の答えがある場合はそのうち1つ。辞書順最小は保証しない。

計算量:
要素数 N、答えの長さ L として時間 O(N log(L+1))、空間 O(N)。

使用例:
```cpp
auto ids = koma::lds_indices(std::vector<int>{5, 3, 4, 2}); // 例: {0, 2, 3}
```
*/
template <class R>
	requires std::ranges::random_access_range<const R> && std::ranges::sized_range<const R> &&
			 std::strict_weak_order<std::less<>, std::ranges::range_reference_t<const R>,
									std::ranges::range_reference_t<const R>>
std::vector<long long> lds_indices(const R &a, bool strict = true) {
	return lds(a, strict).restore_indices();
}

} // namespace koma
