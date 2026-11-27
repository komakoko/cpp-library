#pragma once
#include "point.hpp"

namespace koma {

/**
異なる2点 a, b を通り、a から b に向く無限直線を表す。

コンストラクタ:
Line(Point<T> a, Point<T> b): 異なる2点を指定する。
座標は64ビット以下の符号付き整数または浮動小数点数。
整数では各外積の積と差を符号付き128ビットに収める。
浮動小数点では中間値まで有限とし、判定に許容誤差を設けない。

関数:
side, parallel, intersection で位置関係と交点を求める。

使用例:
`koma::Line l{koma::PointI{0, 0}, koma::PointI{1, 2}};`
*/
template <class T = long long>
struct Line {
	Point<T> a, b;
	constexpr Line(Point<T> a, Point<T> b) : a(a), b(b) { assert(a != b); }
};

namespace detail {
template <class T>
constexpr auto line_vector(Point<T> from, Point<T> to) {
	using W = std::conditional_t<std::is_integral_v<T>, __int128_t, long double>;
	return Point<W>{W(to.x) - W(from.x), W(to.y) - W(from.y)};
}
}

/**
点が有向直線のどちら側にあるかを求める。

引数:
`line`: a から b へ向く直線。`p`: 判定する点。

返り値:
左側は +1、直線上は 0、右側は -1。

計算量:
O(1)。

使用例:
`koma::side(line, line.a) == 0`
*/
template <class T>
constexpr int side(const Line<T> &line, const Point<T> &p) {
	auto det = cross(detail::line_vector(line.a, line.b), detail::line_vector(line.a, p));
	return (det > 0) - (det < 0);
}

/**
2直線が平行かを判定する。

引数:
`l`, `m`: 判定する直線。

返り値:
方向ベクトルの外積が0なら true。一致する直線も含む。

計算量:
O(1)。

使用例:
`koma::parallel(line, line)`
*/
template <class T>
constexpr bool parallel(const Line<T> &l, const Line<T> &m) {
	return cross(detail::line_vector(l.a, l.b), detail::line_vector(m.a, m.b)) == 0;
}

/**
2直線の交点を求める。

引数:
`l`, `m`: 交差を調べる直線。

返り値:
一意な交点の近似座標。平行または一致なら std::nullopt。

計算量:
O(1)。

使用例:
```cpp
if (auto p = koma::intersection(l, m)) {
    std::cout << p->x << ' ' << p->y << '\n';
} else {
    std::cout << "交点は一意に定まりません\n";
}
```
*/
template <class T>
std::optional<PointF> intersection(const Line<T> &l, const Line<T> &m) {
	auto u = detail::line_vector(l.a, l.b);
	auto v = detail::line_vector(m.a, m.b);
	auto det = cross(u, v);
	if(det == 0) return std::nullopt;
	auto num = cross(detail::line_vector(l.a, m.a), v);
	long double t = static_cast<long double>(num) / det;
	return PointF{std::fma(t, static_cast<long double>(u.x), static_cast<long double>(l.a.x)),
	              std::fma(t, static_cast<long double>(u.y), static_cast<long double>(l.a.y))};
}

} // namespace koma
