#pragma once
#include <bits/stdc++.h>

namespace koma {
namespace detail {

/** 各長さの最適な端点と復元情報を求める。from_right は先頭指定用の逆走査、decreasing は減少列の比較を指定する。 */
template <class R>
std::vector<long long> lis_tails(const R &a, bool strict, std::vector<long long> *prev = nullptr,
								 std::vector<long long> *dp = nullptr, bool from_right = false,
								 bool decreasing = false) {
	const long long n = static_cast<long long>(std::ranges::size(a));
	auto first = std::ranges::begin(a);
	std::vector<long long> tails;
	for(long long step = 0; step < n; ++step) {
		long long i = from_right ? n - 1 - step : step;
		long long l = 0, r = static_cast<long long>(tails.size());
		while(l < r) {
			long long m = l + (r - l) / 2;
			bool reverse_order = from_right != decreasing;
			long long left = reverse_order ? i : tails[m];
			long long right = reverse_order ? tails[m] : i;
			bool before = strict ? *(first + left) < *(first + right)
								 : !(*(first + right) < *(first + left));
			if(before) l = m + 1;
			else r = m;
		}
		if(prev && l > 0) (*prev)[i] = tails[l - 1];
		if(dp) (*dp)[i] = l + 1;
		if(l == static_cast<long long>(tails.size())) tails.push_back(i);
		else tails[l] = i;
	}
	return tails;
}

} // namespace detail

/**
最長増加・減少部分列の長さ、DP配列、復元に必要な情報を保持する。
lis の返り値では増加、lds の返り値では減少の条件に従う。

コンストラクタ:
`LisResult()`: 空の列の結果として構築する。
通常は lis(a, strict) または lds(a, strict) の返り値を使う。

関数:
`length`: 列全体の最長部分列の長さ。空の列では0。
`dp[i]`: i 番目の要素を末尾とする最長部分列の長さ。
`prev[i]`: 復元する部分列で i の直前に来る要素の添字。先頭なら -1。
`suffix_dp[i]`: i 番目の要素を必ず先頭とする最長部分列の長さ。
`next[i]`: 先頭指定で復元する部分列で i の直後に来る要素の添字。末尾なら -1。
`tails[k]`: 長さ k+1 の部分列で、増加なら末尾の値が最小、減少なら最大となる要素の添字。
tails は処理完了時の表で、その添字列自体が増加部分列とは限らない。
`restore_indices()`: 列全体の最長部分列の添字を1つ復元する。空なら空配列。O(length)。
`restore_indices(end)`: end を末尾とする最長部分列の添字を復元する。O(dp[end])。
`restore_indices_from(start)`: start を必ず先頭とする最長部分列の添字を復元する。O(suffix_dp[start])。
復元結果の辞書順最小は保証しない。入力の列を保持しないので、元の列の破棄後も添字を復元できる。

使用例:
```cpp
std::vector<int> a{3, 1, 2, 2, 4};
auto res = koma::lis(a);
auto n = res.length;                 // 3
auto dp = res.dp;                    // {1, 1, 2, 2, 3}
auto ids = res.restore_indices();    // 例: {1, 3, 4}
auto part = res.restore_indices(2);  // {1, 2}
auto from = res.restore_indices_from(2); // {2, 4}
```
*/
struct LisResult {
	long long length = 0;
	std::vector<long long> dp;
	std::vector<long long> prev;
	std::vector<long long> tails;
	std::vector<long long> suffix_dp;
	std::vector<long long> next;

	/**
	結果に記録された増加・減少の条件で、列全体の最長部分列の添字を1つ復元する。

	返り値:
	0始まりの添字を元の列における順序で並べた std::vector<long long>。空の列なら空配列。

	計算量:
	部分列の長さ L として O(L)。

	使用例:
	```cpp
	auto res = koma::lis(std::vector<int>{3, 1, 2});
	auto ids = res.restore_indices(); // {1, 2}
	```
	*/
	std::vector<long long> restore_indices() const {
		if(tails.empty()) return {};
		return restore_indices(tails.back());
	}

	/**
	結果に記録された増加・減少の条件で、指定した位置を末尾とする最長部分列の添字を復元する。

	引数:
	`end` (オーバーロード): 末尾にする要素の添字。0 <= end < dp.size()。

	返り値:
	長さ dp[end] の std::vector<long long>。最後の添字は end。

	計算量:
	O(dp[end])。

	使用例:
	```cpp
	auto res = koma::lis(std::vector<int>{3, 1, 2, 4});
	auto ids = res.restore_indices(2); // {1, 2}
	```
	*/
	std::vector<long long> restore_indices(long long end) const {
		assert(0 <= end && end < static_cast<long long>(dp.size()));
		std::vector<long long> result;
		for(long long i = end; i != -1; i = prev[i]) result.push_back(i);
		std::reverse(result.begin(), result.end());
		return result;
	}

	/**
	結果に記録された増加・減少の条件で、指定した位置を必ず先頭とする最長部分列の添字を復元する。

	引数:
	`start`: 先頭にする要素の添字。0 <= start < suffix_dp.size()。

	返り値:
	長さ suffix_dp[start] の std::vector<long long>。
	最初の添字は start で、残りの添字は start より後ろから選ぶ。
	答えが複数ある場合はそのうち1つ。辞書順最小は保証しない。

	計算量:
	O(suffix_dp[start])。

	使用例:
	```cpp
	auto res = koma::lis(std::vector<int>{1, 5, 2, 3, 4});
	auto ids = res.restore_indices_from(2); // {2, 3, 4}
	```
	*/
	std::vector<long long> restore_indices_from(long long start) const {
		assert(0 <= start && start < static_cast<long long>(suffix_dp.size()));
		std::vector<long long> result;
		for(long long i = start; i != -1; i = next[i]) result.push_back(i);
		return result;
	}
};

namespace detail {

/** 増加または減少の条件に従って、両方向のDPと復元情報をまとめて構築する。 */
template <class R>
LisResult monotone_subsequence_result(const R &a, bool strict, bool decreasing) {
	LisResult result;
	result.dp.resize(std::ranges::size(a));
	result.prev.assign(std::ranges::size(a), -1);
	result.tails = lis_tails(a, strict, &result.prev, &result.dp, false, decreasing);
	result.length = static_cast<long long>(result.tails.size());
	result.suffix_dp.resize(std::ranges::size(a));
	result.next.assign(std::ranges::size(a), -1);
	lis_tails(a, strict, &result.next, &result.suffix_dp, true, decreasing);
	return result;
}

} // namespace detail

/**
最長増加部分列（LIS）の長さ、DP配列、復元用の情報を求める。

引数:
`a`: 対象の列。constでランダムアクセスと要素数の取得ができる範囲。
vector、array、span、string などに対応する。要素の < が厳密弱順序を満たすこと。
`strict`: true（既定）なら狭義増加。false なら同じ値を許す広義増加。

返り値:
`LisResult`。length はLISの長さ、dp[i] は i を末尾とする最長増加部分列の長さ。
prev は直前の添字、tails は各長さの最小末尾の添字を保持する。
suffix_dp[i] は i を先頭とする長さ、next[i] はその復元で直後に来る添字。
restore_indices() で全体のLIS、restore_indices(i) で i を末尾とする部分列を復元できる。
restore_indices_from(i) で i を必ず先頭とする部分列を復元できる。
空の列なら length は0、各配列は空。

計算量:
要素数 N、LISの長さ L として時間 O(N log(L+1))、空間 O(N)。

使用例:
```cpp
std::vector<int> a{3, 1, 2, 2, 4};
auto res = koma::lis(a);
auto dp = res.dp;                  // {1, 1, 2, 2, 3}
auto ids = res.restore_indices(); // 例: {1, 3, 4}
auto non_strict = koma::lis(a, false); // length == 4
```
*/
template <class R>
	requires std::ranges::random_access_range<const R> && std::ranges::sized_range<const R> &&
			 std::strict_weak_order<std::less<>, std::ranges::range_reference_t<const R>,
									std::ranges::range_reference_t<const R>>
LisResult lis(const R &a, bool strict = true) {
	return detail::monotone_subsequence_result(a, strict, false);
}

/**
最長増加部分列（LIS）の長さを求める。

引数:
`a`: 対象の列。constでランダムアクセスと要素数の取得ができる範囲。
vector、array、span、string などに対応する。要素の < が厳密弱順序を満たすこと。
`strict`: true（既定）なら狭義増加（前の要素 < 次の要素）。
false なら広義増加（前の要素 <= 次の要素）として同じ値を許す。

返り値:
最長増加部分列の長さを long long で返す。空の列なら0。

計算量:
要素数 N、答えの長さ L として時間 O(N log(L+1))、空間 O(L)。

使用例:
```cpp
std::vector<int> a{3, 1, 2, 2, 4};
auto n = koma::lis_length(a);        // 3（1, 2, 4）
auto m = koma::lis_length(a, false); // 4（1, 2, 2, 4）
```
*/
template <class R>
	requires std::ranges::random_access_range<const R> && std::ranges::sized_range<const R> &&
			 std::strict_weak_order<std::less<>, std::ranges::range_reference_t<const R>,
									std::ranges::range_reference_t<const R>>
long long lis_length(const R &a, bool strict = true) {
	return static_cast<long long>(detail::lis_tails(a, strict).size());
}

/**
最長増加部分列（LIS）を1つ復元し、その添字を返す。

引数:
`a`: 対象の列。constでランダムアクセスと要素数の取得ができる範囲。
vector、array、span、string などに対応する。要素の < が厳密弱順序を満たすこと。
`strict`: true（既定）なら狭義増加、false なら同じ値を許す広義増加。

返り値:
最長増加部分列を構成する0始まりの添字を、元の列における順序で並べた std::vector<long long>。
答えが複数ある場合はそのうち1つ。辞書順最小は保証しない。空の列なら空配列。

計算量:
要素数 N、答えの長さ L として時間 O(N log(L+1))、空間 O(N)。

使用例:
```cpp
std::vector<int> a{3, 1, 2, 2, 4};
auto ids = koma::lis_indices(a); // 例: {1, 3, 4}
std::vector<int> values;
for(auto i : ids) values.push_back(a[i]); // {1, 2, 4}
```
*/
template <class R>
	requires std::ranges::random_access_range<const R> && std::ranges::sized_range<const R> &&
			 std::strict_weak_order<std::less<>, std::ranges::range_reference_t<const R>,
									std::ranges::range_reference_t<const R>>
std::vector<long long> lis_indices(const R &a, bool strict = true) {
	return lis(a, strict).restore_indices();
}

} // namespace koma
