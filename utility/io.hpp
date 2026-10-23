#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
vectorの要素を空白区切りで出力する。末尾の空白・改行は付けない。

引数: `os`: 出力先。`a`: 各要素が出力可能なvector。
返り値: 出力先ストリームへの参照。
計算量: 要素数 N に対して O(N + 各要素の出力時間の合計)。
使用例: `using namespace koma; std::cout << std::vector<int>{1, 2}; // 1 2`
*/
template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &a);

/**
arrayの要素を空白区切りで出力する。末尾の空白・改行は付けない。

引数: `os`: 出力先。`a`: 各要素が出力可能なarray。
返り値: 出力先ストリームへの参照。
計算量: 要素数 N に対して O(N + 各要素の出力時間の合計)。
使用例: `using namespace koma; std::cout << std::array<int, 2>{1, 2}; // 1 2`
*/
template <class T, std::size_t N>
std::ostream &operator<<(std::ostream &os, const std::array<T, N> &a);

/**
pairのfirst、secondの順に入力する。

引数: `is`: 入力元。`a`: 各要素が入力可能なpair。
返り値: 入力元ストリームへの参照。
計算量: 各要素の入力時間の合計。
使用例: `using namespace koma; std::pair<int, int> p; std::cin >> p;`
*/
template <class T, class S>
std::istream &operator>>(std::istream &is, std::pair<T, S> &a);

/**
vectorの各要素に順に入力する。要素数は変更しない。

引数: `is`: 入力元。`a`: 必要な要素数を確保済みの、各要素が入力可能なvector。
返り値: 入力元ストリームへの参照。
計算量: 要素数 N に対して O(N + 各要素の入力時間の合計)。
使用例: `using namespace koma; std::vector<int> a(3); std::cin >> a;`
*/
template <class T>
std::istream &operator>>(std::istream &is, std::vector<T> &a);

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &a) {
	for(std::size_t i = 0; i < a.size(); ++i) {
		if(i) os << ' ';
		os << a[i];
	}
	return os;
}

template <class T, std::size_t N>
std::ostream &operator<<(std::ostream &os, const std::array<T, N> &a) {
	for(std::size_t i = 0; i < N; ++i) {
		if(i) os << ' ';
		os << a[i];
	}
	return os;
}

template <class T, class S>
std::istream &operator>>(std::istream &is, std::pair<T, S> &a) { return is >> a.first >> a.second; }

template <class T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
	for(T &value : a) is >> value;
	return is;
}

} // namespace koma
