#pragma once
#include "line.hpp"

namespace koma {

/**
両端を含む線分を表す。

コンストラクタ:
Segment(Point<T> a, Point<T> b): 両端を指定する。同じ点なら長さ0の線分。
座標と外積の範囲は Line<T> と同じ。

関数:
on_segment で点の包含、segments_intersect で線分同士の交差を判定する。

使用例:
`koma::Segment s{koma::PointI{0, 0}, koma::PointI{2, 2}};`
*/
template <class T = long long>
struct Segment {
	Point<T> a, b;
};

/**
端点を含めて点が線分上にあるか判定する。

引数:
`s`: 線分。`p`: 判定する点。

返り値:
線分上なら true。長さ0の線分では端点と一致するときだけ true。

計算量:
O(1)。

使用例:
`koma::on_segment(s, s.a)`
*/
template <class T>
constexpr bool on_segment(const Segment<T> &s, const Point<T> &p) {
	return std::min(s.a.x, s.b.x) <= p.x && p.x <= std::max(s.a.x, s.b.x)
	    && std::min(s.a.y, s.b.y) <= p.y && p.y <= std::max(s.a.y, s.b.y)
	    && cross(detail::line_vector(s.a, s.b), detail::line_vector(s.a, p)) == 0;
}

/**
2本の閉線分が共通点を持つか判定する。

引数:
`s`, `t`: 判定する線分。

返り値:
端点での接触、同一直線上の重なり、長さ0の線分が他方に含まれる場合も true。

計算量:
O(1)。

使用例:
`koma::segments_intersect(s, t)`
*/
template <class T>
constexpr bool segments_intersect(const Segment<T> &s, const Segment<T> &t) {
	if(s.a == s.b) return on_segment(t, s.a);
	if(t.a == t.b) return on_segment(s, t.a);
	Line ls{s.a, s.b}, lt{t.a, t.b};
	int a = side(ls, t.a), b = side(ls, t.b);
	int c = side(lt, s.a), d = side(lt, s.b);
	if(a == 0 && on_segment(s, t.a)) return true;
	if(b == 0 && on_segment(s, t.b)) return true;
	if(c == 0 && on_segment(t, s.a)) return true;
	if(d == 0 && on_segment(t, s.b)) return true;
	return a * b < 0 && c * d < 0;
}

} // namespace koma
