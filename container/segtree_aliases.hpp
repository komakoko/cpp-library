#pragma once
#include <atcoder/segtree>
#include <bits/stdc++.h>

namespace koma {
namespace detail {

/** 2つの値の最大値を返す。 */
template <class T>
T segtree_max(T a, T b) { return std::max(a, b); }

/** 最大値用の単位元を返す。浮動小数点型では負の無限大を使う。 */
template <class T>
T segtree_max_identity() {
	static_assert(std::numeric_limits<T>::is_specialized);
	if constexpr(std::numeric_limits<T>::has_infinity) return -std::numeric_limits<T>::infinity();
	return std::numeric_limits<T>::lowest();
}

/** 2つの値の最小値を返す。 */
template <class T>
T segtree_min(T a, T b) { return std::min(a, b); }

/** 最小値用の単位元を返す。浮動小数点型では正の無限大を使う。 */
template <class T>
T segtree_min_identity() {
	static_assert(std::numeric_limits<T>::is_specialized);
	if constexpr(std::numeric_limits<T>::has_infinity) return std::numeric_limits<T>::infinity();
	return std::numeric_limits<T>::max();
}

/** 2つの値の和を返す。 */
template <class T>
T segtree_sum(T a, T b) { return a + b; }

/** 和用の単位元0を返す。 */
template <class T>
T segtree_sum_identity() { return T(0); }

} // namespace detail

/**
一点更新と区間最大値の取得ができる。ACLのsegtreeと同じ操作を使える。
`T` は整数型または浮動小数点型（NaNは不可）。既定は `long long`。
単位元は整数型なら `std::numeric_limits<T>::lowest()`、浮動小数点型なら負の無限大。

コンストラクタ:
`max_segtree<T>()`: 要素数0で構築する。
`max_segtree<T>(int n)`: n >= 0。全要素を単位元で初期化する。O(n)。
`max_segtree<T>(const std::vector<T>& v)`: v の各要素で初期化する。O(n)。

関数:
`set(p, x)`: 0 <= p < n の要素を x に更新する。O(log n)。
`get(p)`: 0 <= p < n の要素を取得する。O(1)。
`prod(l, r)`: 0 <= l <= r <= n の半開区間 [l, r) の最大値。空区間は単位元。O(log n)。
`all_prod()`: 全要素の最大値。空なら単位元。O(1)。
`max_right(l, f)` / `min_left(r, f)`: 述語 f を満たす区間の境界を二分探索する。O(log n)。
境界探索は 0 <= l, r <= n、f(単位元) == true が必要。f は副作用がなく、区間の伸長に対して単調であること。

使用例:
```cpp
koma::max_segtree<> seg(std::vector<long long>{-5, -2, -8});
seg.set(2, -1);
auto x = seg.prod(0, 3); // -1
auto r = seg.max_right(0, [](long long x) { return x < -1; }); // 2
```
*/
template <class T = long long>
using max_segtree = atcoder::segtree<T, detail::segtree_max<T>, detail::segtree_max_identity<T>>;

/**
一点更新と区間最小値の取得ができる。ACLのsegtreeと同じ操作を使える。
`T` は整数型または浮動小数点型（NaNは不可）。既定は `long long`。
単位元は整数型なら `std::numeric_limits<T>::max()`、浮動小数点型なら正の無限大。

コンストラクタ:
`min_segtree<T>()`: 要素数0で構築する。
`min_segtree<T>(int n)`: n >= 0。全要素を単位元で初期化する。O(n)。
`min_segtree<T>(const std::vector<T>& v)`: v の各要素で初期化する。O(n)。

関数:
`set(p, x)`: 0 <= p < n の要素を x に更新する。O(log n)。
`get(p)`: 0 <= p < n の要素を取得する。O(1)。
`prod(l, r)`: 0 <= l <= r <= n の半開区間 [l, r) の最小値。空区間は単位元。O(log n)。
`all_prod()`: 全要素の最小値。空なら単位元。O(1)。
`max_right(l, f)` / `min_left(r, f)`: 述語 f を満たす区間の境界を二分探索する。O(log n)。
境界探索は 0 <= l, r <= n、f(単位元) == true が必要。f は副作用がなく、区間の伸長に対して単調であること。

使用例:
```cpp
koma::min_segtree<int> seg(std::vector<int>{5, 2, 8});
seg.set(1, 7);
auto x = seg.prod(0, 2); // 5
```
*/
template <class T = long long>
using min_segtree = atcoder::segtree<T, detail::segtree_min<T>, detail::segtree_min_identity<T>>;

/**
一点更新と区間和の取得ができる。ACLのsegtreeと同じ操作を使える。
`T` は加算と `T(0)` に対応する型。既定は `long long`。modintも使用できる。
単位元は0。和が型の範囲を超えないようにすること。

コンストラクタ:
`sum_segtree<T>()`: 要素数0で構築する。
`sum_segtree<T>(int n)`: n >= 0。全要素を0で初期化する。O(n)。
`sum_segtree<T>(const std::vector<T>& v)`: v の各要素で初期化する。O(n)。

関数:
`set(p, x)`: 0 <= p < n の要素を x に更新する。O(log n)。加算は `set(p, get(p) + x)`。
`get(p)`: 0 <= p < n の要素を取得する。O(1)。
`prod(l, r)`: 0 <= l <= r <= n の半開区間 [l, r) の和。空区間は0。O(log n)。
`all_prod()`: 全要素の和。空なら0。O(1)。
`max_right(l, f)` / `min_left(r, f)`: 述語 f を満たす区間の境界を二分探索する。O(log n)。
境界探索は 0 <= l, r <= n、f(0) == true が必要。f は副作用がなく、区間の伸長に対して単調であること。

使用例:
```cpp
koma::sum_segtree<> seg(3);
seg.set(0, 5);
seg.set(1, 2);
auto x = seg.prod(0, 2); // 7
```
*/
template <class T = long long>
using sum_segtree = atcoder::segtree<T, detail::segtree_sum<T>, detail::segtree_sum_identity<T>>;

} // namespace koma
