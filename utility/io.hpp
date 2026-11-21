#pragma once
#include "lll.hpp"
#include <bits/stdc++.h>

namespace koma {

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &a);

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<std::vector<T>> &a);

template <class T, std::size_t N>
std::ostream &operator<<(std::ostream &os, const std::array<T, N> &a);

template <class T, class S>
std::istream &operator>>(std::istream &is, std::pair<T, S> &a);

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

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<std::vector<T>> &a) {
	for(std::size_t i = 0; i < a.size(); ++i) {
		if(i) os << '\n';
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

/**
標準入力から指定した型の値を読み込んで返す。

引数:
`T`: 入力する型。既定値は long long。std::cin >> value で読み込める型を指定する。
入力には T の値を読み込めるデータがあること。

返り値:
読み込んだ T 型の値。

計算量:
T の入力処理にかかる時間。

使用例:
```cpp
auto x = koma::in();
auto s = koma::in<std::string>();
std::set<std::pair<long long, long long>> points;
points.insert({koma::in(), koma::in()});
```
*/
template <class T = long long>
T in() {
	T value{};
	std::cin >> value;
	return value;
}

} // namespace koma
