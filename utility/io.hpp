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

} // namespace koma
