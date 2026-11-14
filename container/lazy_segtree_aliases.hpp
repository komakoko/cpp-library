#pragma once
#include "segtree_aliases.hpp"
#include <atcoder/lazysegtree>
#include <bits/stdc++.h>

namespace koma {
namespace detail {

/** 遅延セグメント木の集約方法を指定する。 */
enum class lazy_segtree_kind { sum,
							   min,
							   max };

/** 区間の集約値と、実際に含まれる要素数を保持する。 */
template <class T>
struct lazy_segtree_node {
	T value;
	int length;
};

/** 保留する区間代入または区間加算を保持する。 */
template <class T>
struct lazy_segtree_action {
	std::optional<T> assign;
	T add;
};

/** 集約方法に応じた単位元を返す。 */
template <class T, lazy_segtree_kind Kind>
T lazy_segtree_identity() {
	if constexpr(Kind == lazy_segtree_kind::sum) return T(0);
	else if constexpr(Kind == lazy_segtree_kind::min) return segtree_min_identity<T>();
	else return segtree_max_identity<T>();
}

/** 隣接する2区間を集約する。空区間はそのまま無視する。 */
template <class T, lazy_segtree_kind Kind>
lazy_segtree_node<T> lazy_segtree_op(lazy_segtree_node<T> a, lazy_segtree_node<T> b) {
	if(a.length == 0) return b;
	if(b.length == 0) return a;
	if constexpr(Kind == lazy_segtree_kind::sum) return {T(a.value + b.value), a.length + b.length};
	else if constexpr(Kind == lazy_segtree_kind::min) return {std::min(a.value, b.value), a.length + b.length};
	else return {std::max(a.value, b.value), a.length + b.length};
}

/** 空区間を表すノードを返す。 */
template <class T, lazy_segtree_kind Kind>
lazy_segtree_node<T> lazy_segtree_empty() { return {lazy_segtree_identity<T, Kind>(), 0}; }

/** 区間代入・加算を集約値に反映する。 */
template <class T, lazy_segtree_kind Kind>
lazy_segtree_node<T> lazy_segtree_mapping(lazy_segtree_action<T> f, lazy_segtree_node<T> x) {
	if(x.length == 0) return x;
	if(f.assign) {
		if constexpr(Kind == lazy_segtree_kind::sum) x.value = *f.assign * T(x.length);
		else x.value = *f.assign;
	}
	if constexpr(Kind == lazy_segtree_kind::sum) x.value = x.value + f.add * T(x.length);
	else x.value = x.value + f.add;
	return x;
}

/** g の後に f を適用する更新を合成する。後からの代入はそれ以前の更新を上書きする。 */
template <class T>
lazy_segtree_action<T> lazy_segtree_composition(lazy_segtree_action<T> f, lazy_segtree_action<T> g) {
	if(f.assign) return f;
	if(g.assign) return {T(*g.assign + f.add), T(0)};
	return {std::nullopt, T(g.add + f.add)};
}

/** 値を変更しない更新を返す。 */
template <class T>
lazy_segtree_action<T> lazy_segtree_no_action() { return {std::nullopt, T(0)}; }

/** 区間加算・区間代入と、集約方法に応じた区間取得を提供する共通実装。 */
template <class T, lazy_segtree_kind Kind>
class basic_lazy_segtree {
	using node = lazy_segtree_node<T>;
	using action = lazy_segtree_action<T>;
	using tree_type = atcoder::lazy_segtree<node, lazy_segtree_op<T, Kind>, lazy_segtree_empty<T, Kind>,
											action, lazy_segtree_mapping<T, Kind>, lazy_segtree_composition<T>, lazy_segtree_no_action<T>>;
	int n_ = 0;
	tree_type tree_;

	/** n 要素を0で初期化するための葉を作る。n >= 0。O(n)。 */
	static std::vector<node> zero_nodes(int n) {
		assert(n >= 0);
		return std::vector<node>(n, node{T(0), 1});
	}

	/** 初期値配列を要素数1の葉に変換する。v.size() <= INT_MAX。O(n)。 */
	static std::vector<node> make_nodes(const std::vector<T> &v) {
		assert(v.size() <= static_cast<std::size_t>(INT_MAX));
		std::vector<node> nodes;
		nodes.reserve(v.size());
		for(const auto &value : v) nodes.push_back({value, 1});
		return nodes;
	}

  public:
	/** 要素数0で構築する。 */
	basic_lazy_segtree() = default;

	/** n >= 0 の n 要素を0で初期化する。O(n)。 */
	explicit basic_lazy_segtree(int n) : n_(n), tree_(zero_nodes(n)) {}

	/** v の各要素で構築する。v.size() <= INT_MAX。O(n)。 */
	explicit basic_lazy_segtree(const std::vector<T> &v) : tree_(make_nodes(v)) {
		n_ = static_cast<int>(v.size());
	}

	/**
	半開区間 [l, r) に x を加算する。

	引数:
	`l`, `r`: 0 <= l <= r <= size()。空区間は何もしない。
	`x`: 各要素への加算量。

	計算量:
	O(log(n+1))。

	使用例:
	```cpp
	koma::lazy_sum_segtree<> seg(5);
	seg.add(1, 4, 3);
	```
	*/
	void add(int l, int r, T x) { tree_.apply(l, r, action{std::nullopt, x}); }

	/**
	半開区間 [l, r) の全要素に x を代入する。

	引数:
	`l`, `r`: 0 <= l <= r <= size()。空区間は何もしない。
	`x`: 代入する値。番兵として予約される値はない。

	計算量:
	O(log(n+1))。

	使用例:
	```cpp
	koma::lazy_sum_segtree<> seg(5);
	seg.assign(1, 4, 5);
	```
	*/
	void assign(int l, int r, T x) { tree_.apply(l, r, action{x, T(0)}); }

	/** p 番目の要素を x にする。0 <= p < size()。O(log(n+1))。使用例: `seg.set(0, 7);` */
	void set(int p, T x) { tree_.set(p, node{x, 1}); }

	/** p 番目の値を返す。0 <= p < size()。O(log(n+1))。使用例: `auto x = seg.get(0);` */
	T get(int p) { return tree_.get(p).value; }

	/**
	半開区間 [l, r) の集約値を返す。

	引数:
	`l`, `r`: 0 <= l <= r <= size()。

	返り値:
	型に応じて区間和・最小値・最大値を返す。空区間は各型の単位元。

	計算量:
	O(log(n+1))。

	使用例:
	```cpp
	koma::lazy_sum_segtree<> seg(std::vector<long long>{1, 2, 3});
	auto total = seg.prod(0, 3); // 6
	```
	*/
	T prod(int l, int r) { return tree_.prod(l, r).value; }

	/** 全区間の集約値を返す。空なら単位元。O(1)。使用例: `auto total = seg.all_prod();` */
	T all_prod() { return tree_.all_prod().value; }

	/** 要素数を返す。O(1)。使用例: `auto n = seg.size();` */
	int size() const { return n_; }

	/**
	区間 [l, r) の集約値が述語 f を満たす最大の r を求める。

	引数:
	`l`: 左端。0 <= l <= size()。
	`f`: 集約値 T を受け取る述語。f(単位元) == true が必要。
	副作用がなく、区間の伸長に対して true から false へ単調に変わること。

	返り値:
	条件を満たす最大の右端 r。

	計算量:
	O(log(n+1))。

	使用例:
	```cpp
	koma::lazy_sum_segtree<> seg(std::vector<long long>{1, 2, 3});
	auto r = seg.max_right(0, [](long long x) { return x <= 3; }); // 2
	```
	*/
	template <class F>
	int max_right(int l, F f) {
		return tree_.max_right(l, [&](node x) { return f(x.value); });
	}

	/**
	区間 [l, r) の集約値が述語 f を満たす最小の l を求める。

	引数:
	`r`: 右端。0 <= r <= size()。
	`f`: 集約値 T を受け取る述語。f(単位元) == true が必要。
	副作用がなく、区間の伸長に対して true から false へ単調に変わること。

	返り値:
	条件を満たす最小の左端 l。

	計算量:
	O(log(n+1))。

	使用例:
	```cpp
	koma::lazy_sum_segtree<> seg(std::vector<long long>{1, 2, 3});
	auto l = seg.min_left(3, [](long long x) { return x <= 3; }); // 2
	```
	*/
	template <class F>
	int min_left(int r, F f) {
		return tree_.min_left(r, [&](node x) { return f(x.value); });
	}
};

} // namespace detail

/**
区間加算・区間代入と、区間和の取得ができる。
T の既定は long long。T(0)、加算、乗算に対応する型で、modint も使える。
要素の値・区間和・遅延更新の合成など、計算途中の値も T の範囲内で使うこと。

コンストラクタ:
`lazy_sum_segtree<T>()`: 要素数0で構築する。
`lazy_sum_segtree<T>(int n)`: n >= 0。全要素を0で初期化する。O(n)。
`lazy_sum_segtree<T>(const std::vector<T>& v)`: v の各要素で構築する。要素数は INT_MAX 以下。O(n)。

関数:
`add(l, r, x)`: 半開区間 [l, r) の全要素に x 加算。O(log(n+1))。
`assign(l, r, x)`: 半開区間 [l, r) の全要素を x にする。O(log(n+1))。
`set(p, x)` / `get(p)`: 1点の代入・取得。O(log(n+1))。
`prod(l, r)`: 区間和。空区間なら0。O(log(n+1))。
`all_prod()`: 全体の和。空なら0。O(1)。`size()`: 要素数。O(1)。
`max_right(l, f)` / `min_left(r, f)`: ACLと同じ境界探索。O(log(n+1))。
区間は 0 <= l <= r <= size()、点は 0 <= p < size()。
境界探索の f は副作用がなく、区間の伸長に対して単調で、f(0) == true が必要。

使用例:
```cpp
koma::lazy_sum_segtree<> seg(std::vector<long long>{1, 2, 3});
seg.assign(0, 2, 5); // {5, 5, 3}
seg.add(1, 3, 2);    // {5, 7, 5}
auto total = seg.prod(0, 3); // 17
```
*/
template <class T = long long>
using lazy_sum_segtree = detail::basic_lazy_segtree<T, detail::lazy_segtree_kind::sum>;

/**
区間加算・区間代入と、区間最小値の取得ができる。
T の既定は long long。整数型または浮動小数点型（NaNは不可）。
整数では要素の値と遅延更新の合成が T の範囲内であり、加算が大小関係を保つこと。

コンストラクタ:
`lazy_min_segtree<T>()`: 要素数0で構築する。
`lazy_min_segtree<T>(int n)`: n >= 0。全要素を0で初期化する。O(n)。
`lazy_min_segtree<T>(const std::vector<T>& v)`: v の各要素で構築する。要素数は INT_MAX 以下。O(n)。

関数:
`add(l, r, x)` / `assign(l, r, x)`: 半開区間への加算・代入。O(log(n+1))。
`set(p, x)` / `get(p)`: 1点の代入・取得。O(log(n+1))。
`prod(l, r)`: 区間最小値。O(log(n+1))。
`all_prod()`: 全体の最小値。O(1)。`size()`: 要素数。O(1)。
空区間は整数なら T の最大値、浮動小数点なら正の無限大。
`max_right(l, f)` / `min_left(r, f)`: ACLと同じ境界探索。O(log(n+1))。
区間は 0 <= l <= r <= size()、点は 0 <= p < size()。
境界探索の f は副作用がなく、区間の伸長に対して単調で、f(単位元) == true が必要。

使用例:
```cpp
koma::lazy_min_segtree<> seg(std::vector<long long>{1, 2, 3});
seg.add(0, 2, 4);
seg.assign(2, 3, -1);
auto minimum = seg.prod(0, 3); // -1
```
*/
template <class T = long long>
using lazy_min_segtree = detail::basic_lazy_segtree<T, detail::lazy_segtree_kind::min>;

/**
区間加算・区間代入と、区間最大値の取得ができる。
T の既定は long long。整数型または浮動小数点型（NaNは不可）。
整数では要素の値と遅延更新の合成が T の範囲内であり、加算が大小関係を保つこと。

コンストラクタ:
`lazy_max_segtree<T>()`: 要素数0で構築する。
`lazy_max_segtree<T>(int n)`: n >= 0。全要素を0で初期化する。O(n)。
`lazy_max_segtree<T>(const std::vector<T>& v)`: v の各要素で構築する。要素数は INT_MAX 以下。O(n)。

関数:
`add(l, r, x)` / `assign(l, r, x)`: 半開区間への加算・代入。O(log(n+1))。
`set(p, x)` / `get(p)`: 1点の代入・取得。O(log(n+1))。
`prod(l, r)`: 区間最大値。O(log(n+1))。
`all_prod()`: 全体の最大値。O(1)。`size()`: 要素数。O(1)。
空区間は整数なら T の最小値、浮動小数点なら負の無限大。
`max_right(l, f)` / `min_left(r, f)`: ACLと同じ境界探索。O(log(n+1))。
区間は 0 <= l <= r <= size()、点は 0 <= p < size()。
境界探索の f は副作用がなく、区間の伸長に対して単調で、f(単位元) == true が必要。

使用例:
```cpp
koma::lazy_max_segtree<> seg(std::vector<long long>{1, 2, 3});
seg.assign(0, 2, 5);
seg.add(1, 3, 2);
auto maximum = seg.prod(0, 3); // 7
```
*/
template <class T = long long>
using lazy_max_segtree = detail::basic_lazy_segtree<T, detail::lazy_segtree_kind::max>;

} // namespace koma
