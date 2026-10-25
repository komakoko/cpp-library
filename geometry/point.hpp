#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
`T` は符号付き整数型または浮動小数点型 既定値は `long long`。
コンストラクタ:
`Point()`: 原点 (0, 0) を作る。
`Point(T x, T y)`: x座標とy座標を指定。
*/
template <class T = long long>
struct Point {
	T x{}, y{};
	constexpr Point() = default;
	constexpr Point(T x, T y) : x(x), y(y) {}
	constexpr Point operator+() const { return *this; }
	constexpr Point operator-() const { return Point(-x, -y); }
	constexpr Point &operator+=(const Point &q) {
		x += q.x;
		y += q.y;
		return *this;
	}
	constexpr Point &operator-=(const Point &q) {
		x -= q.x;
		y -= q.y;
		return *this;
	}
	constexpr Point &operator*=(T s) {
		x *= s;
		y *= s;
		return *this;
	}
	constexpr Point &operator/=(T s) {
		x /= s;
		y /= s;
		return *this;
	}
	friend constexpr Point operator+(Point p, const Point &q) { return p += q; }
	friend constexpr Point operator-(Point p, const Point &q) { return p -= q; }
	friend constexpr Point operator*(Point p, T s) { return p *= s; }
	friend constexpr Point operator*(T s, Point p) { return p *= s; }
	friend constexpr Point operator/(Point p, T s) { return p /= s; }
	constexpr bool operator==(const Point &) const = default;
	constexpr auto operator<=>(const Point &) const = default;
	friend std::istream &operator>>(std::istream &in, Point &p) { return in >> p.x >> p.y; }
	friend std::ostream &operator<<(std::ostream &out, const Point &p) { return out << p.x << ' ' << p.y; }
};

using PointI = Point<long long>;
using PointF = Point<long double>;

/**
原点から点 p へ向かうベクトルの偏角を求める。

引数:
`p`: 符号付き整数型または浮動小数点型の座標を持つ点。浮動小数点の座標は有限値とする。

返り値:
正のx軸から反時計回りに測った角度をラジアンで表す `long double`。範囲は [-π, π]。
原点では0を返す。負のx軸上では y の符号付きゼロに応じて π または -π を返す。
整数座標も浮動小数点に変換して計算するため、結果には丸め誤差を含む。

計算量:
O(1)。

使用例:
```cpp
auto angle = koma::arg(koma::PointI(0, 1)); // π / 2
```
*/
template <class T = long long>
long double arg(const Point<T> &p) {
	if(p.x == 0 && p.y == 0) return 0;
	return std::atan2(static_cast<long double>(p.y), static_cast<long double>(p.x));
}

/**
内積 a.x * b.x + a.y * b.y を求める。

引数:
`a`, `b`:64ビット以下の符号付き整数型、または浮動小数点型。

返り値:
__int128_t。
各積と和を __int128_t の範囲に収める。全座標の絶対値が 2^63 - 1 以下なら満たす。
整数座標では結果が0なら両ベクトルは垂直、または少なくとも一方が零ベクトル。

使用例:
```cpp
koma::PointI a(1, 2), b(3, 4);
auto value = koma::dot(a, b); // __int128_t の 11
```
*/
template <class T>
constexpr auto dot(const Point<T> &a, const Point<T> &b) {
	if constexpr(std::is_integral_v<T>) return __int128_t(a.x) * b.x + __int128_t(a.y) * b.y;
	else return a.x * b.x + a.y * b.y;
}

template <class T>
constexpr auto operator*(const Point<T> &a, const Point<T> &b) { return dot(a, b); }

/**
外積 a.x * b.y - a.y * b.x を求める。

引数:
`a`, `b`: 64ビット以下の符号付き整数型、または浮動小数点型。
各積と和を __int128_t の範囲に収める。全座標の絶対値が 2^63 - 1 以下なら満たす。

返り値:
__int128_t。
符号は通常のx右向き・y上向きの座標系で、正なら a から見て b は左側、負なら右側。
整数座標では0なら平行、または少なくとも一方が零ベクトル。絶対値は張られる平行四辺形の面積。
点の差を渡す場合、引数の引き算は Point<T> の座標型で先に計算されることに注意。

使用例:
```cpp
koma::PointI a(1, 0), b(0, 1);
auto value = koma::cross(a, b); // __int128_t の 1（左側）
```
*/
template <class T>
constexpr auto cross(const Point<T> &a, const Point<T> &b) {
	if constexpr(std::is_integral_v<T>) return __int128_t(a.x) * b.y - __int128_t(a.y) * b.x;
	else return a.x * b.y - a.y * b.x;
}

} // namespace koma
