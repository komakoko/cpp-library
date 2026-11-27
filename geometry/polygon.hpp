#pragma once
#include "line.hpp"

namespace koma {

/**
周に沿って並んだ頂点から、多角形の符号付き面積の2倍を求める。

引数:
`vertices`: 単純多角形の頂点列。凹多角形にも対応し、末尾に始点を再掲してもよい。
整数座標では各外積の積・差と途中の総和を符号付き128ビットに収める。
浮動小数点座標では中間値まで有限とする。

返り値:
反時計回りなら正、時計回りなら負。頂点が3個未満、または全点が一直線上なら0。
整数座標なら __int128_t、浮動小数点座標なら long double。

計算量:
頂点数を N として O(N)。

使用例:
`koma::polygon_area2({{0, 0}, {0, 1}, {1, 0}}) == -1`
*/
template <class T = long long>
constexpr auto polygon_area2(const std::vector<Point<T>> &vertices) {
	using W = std::conditional_t<std::is_integral_v<T>, __int128_t, long double>;
	W sum = 0;
	for(std::size_t i = 2; i < vertices.size(); ++i) {
		auto u = detail::line_vector(vertices.front(), vertices[i - 1]);
		auto v = detail::line_vector(vertices.front(), vertices[i]);
		sum += cross(u, v);
	}
	return sum;
}

/**
周に沿って並んだ頂点から、多角形の面積を求める。

引数:
`vertices`: polygon_area2 と同じ条件の頂点列。

返り値:
頂点順によらない非負の面積を long double で返す。大きな整数からの変換では丸めを含む。

計算量:
頂点数を N として O(N)。

使用例:
`koma::polygon_area({{0, 0}, {0, 1}, {1, 0}}) == 0.5L`
*/
template <class T = long long>
constexpr long double polygon_area(const std::vector<Point<T>> &vertices) {
	return std::abs(static_cast<long double>(polygon_area2(vertices))) / 2.0L;
}

} // namespace koma
