#pragma once
#include <atcoder/segtree>
#include <bits/stdc++.h>

namespace koma {
namespace detail {

template <class T>
T segtree_max(T a, T b) { return std::max(a, b); }

template <class T>
T segtree_max_identity() {
	static_assert(std::numeric_limits<T>::is_specialized);
	if constexpr(std::numeric_limits<T>::has_infinity) return -std::numeric_limits<T>::infinity();
	return std::numeric_limits<T>::lowest();
}

template <class T>
T segtree_min(T a, T b) { return std::min(a, b); }

template <class T>
T segtree_min_identity() {
	static_assert(std::numeric_limits<T>::is_specialized);
	if constexpr(std::numeric_limits<T>::has_infinity) return std::numeric_limits<T>::infinity();
	return std::numeric_limits<T>::max();
}

template <class T>
T segtree_sum(T a, T b) { return a + b; }

template <class T>
T segtree_sum_identity() { return T(0); }

template <class T>
T segtree_gcd(T a, T b) { return std::gcd(a, b); }

template <class T>
T segtree_prod(T a, T b) { return a * b; }

template <class T>
T segtree_prod_identity() { return T(1); }

template <class T>
T segtree_xor(T a, T b) { return a ^ b; }

template <class T>
T segtree_or(T a, T b) { return a | b; }

template <class T>
T segtree_and(T a, T b) { return a & b; }

template <class T>
T segtree_and_identity() { return T(~T(0)); }

}

template <class T = long long>
using max_segtree = atcoder::segtree<T, detail::segtree_max<T>, detail::segtree_max_identity<T>>;

template <class T = long long>
using min_segtree = atcoder::segtree<T, detail::segtree_min<T>, detail::segtree_min_identity<T>>;

template <class T = long long>
using sum_segtree = atcoder::segtree<T, detail::segtree_sum<T>, detail::segtree_sum_identity<T>>;

template <class T = long long>
using gcd_segtree = atcoder::segtree<T, detail::segtree_gcd<T>, detail::segtree_sum_identity<T>>;

template <class T = long long>
using prod_segtree = atcoder::segtree<T, detail::segtree_prod<T>, detail::segtree_prod_identity<T>>;

template <class T = long long>
using xor_segtree = atcoder::segtree<T, detail::segtree_xor<T>, detail::segtree_sum_identity<T>>;

template <class T = long long>
using or_segtree = atcoder::segtree<T, detail::segtree_or<T>, detail::segtree_sum_identity<T>>;

template <class T = long long>
using and_segtree = atcoder::segtree<T, detail::segtree_and<T>, detail::segtree_and_identity<T>>;

}
