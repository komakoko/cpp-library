#pragma once
#include <bits/stdc++.h>
#include "point.hpp"

namespace koma {

/**
整数座標の2点について、原点から見た偏角の順序を正確に比較する。

引数:
`a`, `b`: 64ビット以下の符号付き整数型の座標を持つ点。座標型の全範囲を使用できる。

返り値:
正のx軸を始点とする反時計回りの順序 [0, 2π) で a が b より前なら true。
原点はすべての非零ベクトルより前とする。同じ方向の非零ベクトル同士は同順位。
浮動小数点を使わず、外積を __int128_t で計算する。

計算量:
O(1)。

使用例:
```cpp
bool before = koma::arg_less(koma::PointI(1, 0), koma::PointI(0, 1)); // true
```
*/
template <std::signed_integral T = long long>
requires (sizeof(T) <= sizeof(long long))
constexpr bool arg_less(const Point<T> &a, const Point<T> &b) {
	const bool az = a.x == 0 && a.y == 0;
	const bool bz = b.x == 0 && b.y == 0;
	if(az || bz) return az && !bz;
	const bool ah = a.y < 0 || (a.y == 0 && a.x < 0);
	const bool bh = b.y < 0 || (b.y == 0 && b.x < 0);
	if(ah != bh) return ah < bh;
	return cross(a, b) > 0;
}

/**
整数座標の点列を原点から見た偏角順に並べ替える。

引数:
`points`: 64ビット以下の符号付き整数型の座標を持つ点列。座標型の全範囲を使用できる。
正のx軸から反時計回りの順序 [0, 2π) に、その場で並べ替える。
原点は先頭に置く。同じ方向の点同士や重複点の順序は保証しない。
比較は整数演算だけで行う。

返り値:
なし。

計算量:
点数 N として O(N log(N+1))。

使用例:
```cpp
std::vector<koma::PointI> points{{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
koma::arg_sort(points); // {(1,0), (0,1), (-1,0), (0,-1)}
```
*/
template <std::signed_integral T = long long>
requires (sizeof(T) <= sizeof(long long))
void arg_sort(std::vector<Point<T>> &points) {
	std::sort(points.begin(), points.end(), arg_less<T>);
}

} // namespace koma
