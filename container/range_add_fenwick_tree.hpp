#pragma once
#include <atcoder/fenwicktree>
#include <bits/stdc++.h>

namespace koma {

/**
半開区間への一括加算と、指定した1要素の値の取得ができる。
T は ACL の fenwick_tree が対応する整数型または modint。既定は long long。
整数型の演算は ACL と同様に型のビット幅を法として扱う。

コンストラクタ:
`range_add_fenwick_tree<T>()`: 要素数0で構築する。
`range_add_fenwick_tree<T>(int n)`: n >= 0。n 要素を0で初期化する。O(n)。
`range_add_fenwick_tree<T>(const std::vector<T>& v)`: v の各要素で初期化する。
要素数は INT_MAX 以下。O(n log(n+1))。

関数:
`add(l, r, x)`: 0 <= l <= r <= size() の半開区間 [l, r) に x を加算する。
空区間は何もしない。O(log(n+1))。
`get(p)`: 0 <= p < size() の要素の値を取得する。O(log(n+1))。
`size()`: 要素数を返す。O(1)。

使用例:
```cpp
koma::range_add_fenwick_tree<> fw(5);
fw.add(1, 4, 3);
fw.add(2, 5, 2);
auto x = fw.get(2); // 5
auto y = fw.get(4); // 2
```
*/
template <class T = long long>
class range_add_fenwick_tree {
  public:
	/** 要素数0で構築する。 */
	range_add_fenwick_tree() = default;

	/** n >= 0 の n 要素を0で初期化する。O(n)。 */
	explicit range_add_fenwick_tree(int n) : n_(n) {
		assert(n >= 0);
		bit_ = atcoder::fenwick_tree<T>(n);
	}

	/** v の各要素で初期化する。v.size() <= INT_MAX。O(n log(n+1))。 */
	explicit range_add_fenwick_tree(const std::vector<T> &v) {
		assert(v.size() <= static_cast<std::size_t>(INT_MAX));
		n_ = static_cast<int>(v.size());
		bit_ = atcoder::fenwick_tree<T>(n_);
		U previous = U(0);
		for(int i = 0; i < n_; ++i) {
			U current = U(v[i]);
			bit_.add(i, T(current - previous));
			previous = current;
		}
	}

	/** 半開区間 [l, r) に x を加算する。0 <= l <= r <= size()。O(log(n+1))。 */
	void add(int l, int r, T x) {
		assert(0 <= l && l <= r && r <= n_);
		if(l == r) return;
		bit_.add(l, x);
		if(r < n_) bit_.add(r, T(U(0) - U(x)));
	}

	/** p 番目の値を返す。0 <= p < size()。O(log(n+1))。 */
	T get(int p) const {
		assert(0 <= p && p < n_);
		return bit_.sum(0, p + 1);
	}

	/** 要素数を返す。O(1)。 */
	int size() const { return n_; }

  private:
	using U = typename atcoder::fenwick_tree<T>::U;
	int n_ = 0;
	mutable atcoder::fenwick_tree<T> bit_;
};

} // namespace koma
