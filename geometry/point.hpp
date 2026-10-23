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
	constexpr Point &operator+=(const Point &q) { x += q.x; y += q.y; return *this; }
	constexpr Point &operator-=(const Point &q) { x -= q.x; y -= q.y; return *this; }
	constexpr Point &operator*=(T s) { x *= s; y *= s; return *this; }
	constexpr Point &operator/=(T s) { x /= s; y /= s; return *this; }
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
原点からの2つのベクトルの内積 a.x * b.x + a.y * b.y を求める。
引数:
`a`, `b`: 同じ座標型のベクトル。座標型は64ビット以下の符号付き整数型、または浮動小数点型。

返り値:
整数座標では、各座標を __int128_t に変換してから計算した __int128_t。
各積と和が __int128_t の範囲に収まることが前提。全座標の絶対値が 2^63 - 1 以下なら満たす。
浮動小数点座標では、座標と同じ型で計算した値。入力・中間結果・結果は有限値とし、誤差補正は行わない。
零ベクトルとの内積は0。整数座標では結果が0なら両ベクトルは垂直、または少なくとも一方が零ベクトル。

計算量:
時間・追加メモリともに O(1)。

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
原点からの2つのベクトルの2次元外積 a.x * b.y - a.y * b.x を求める。
引数:
`a`, `b`: 同じ座標型のベクトル。座標型は64ビット以下の符号付き整数型、または浮動小数点型。

返り値:
整数座標では、各座標を __int128_t に変換してから計算した __int128_t。
各積と差が __int128_t の範囲に収まることが前提。全座標の絶対値が 2^63 - 1 以下なら満たす。
浮動小数点座標では、座標と同じ型で計算した値。入力・中間結果・結果は有限値とし、誤差補正は行わない。
符号は通常のx右向き・y上向きの座標系で、正なら a から見て b は左側、負なら右側。
整数座標では0なら平行、または少なくとも一方が零ベクトル。絶対値は張られる平行四辺形の面積。
点の差を渡す場合、引数の引き算は Point<T> の座標型で先に計算されることに注意。

計算量:
時間・追加メモリともに O(1)。

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
