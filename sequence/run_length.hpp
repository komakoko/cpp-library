#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
連続する等しい要素をまとめ、値と連続回数の組に圧縮する。
要素の順序は保持される。

引数:
`first`: 対象範囲の先頭イテレータ。
`last`: 対象範囲の終端イテレータ。

返り値:
`std::vector<std::pair<T, long long>>`。
各要素は `{値, 連続回数}` の組。
空の範囲に対しては空の配列を返す。

計算量:
要素数 N に対して O(N)。

使用例:
```cpp
std::string s = "aaabbbcc";
auto res = koma::run_length(s.begin(), s.end());
// {{'a', 3}, {'b', 3}, {'c', 2}}
```
*/
template <typename It>
auto run_length(It first, It last) {
	using T = typename std::iterator_traits<It>::value_type;
	std::vector<std::pair<T, long long>> res;
	if(first == last) return res;
	T cur = *first;
	long long cnt = 0;
	for(; first != last; ++first) {
		if(*first == cur) {
			++cnt;
		} else {
			res.emplace_back(cur, cnt);
			cur = *first;
			cnt = 1;
		}
	}
	res.emplace_back(cur, cnt);
	return res;
}

/**
コンテナ内の連続する等しい要素をまとめ、値と連続回数の組に圧縮する。

引数:
`v`: 圧縮対象のコンテナ。

返り値:
`std::vector<std::pair<T, long long>>`。
各要素は `{値, 連続回数}` の組。
空のコンテナに対しては空の配列を返す。

計算量:
要素数 N に対して O(N)。

使用例:
```cpp
std::string s = "aaabbbcc";
auto res = koma::run_length(s);
// {{'a', 3}, {'b', 3}, {'c', 2}}
```
*/
template <typename Container>
auto run_length(const Container &v) {
	return run_length(std::begin(v), std::end(v));
}

} // namespace koma
