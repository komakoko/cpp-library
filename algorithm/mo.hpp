#pragma once
#include <bits/stdc++.h>

namespace koma {
/**
静的な区間クエリをMoの順序で処理する。区間は半開区間 [l, r)。

コンストラクタ:
`Mo(n)`: n は配列長（0以上）。

関数:
`add_query(l, r)` は登録順のIDを返す。
`run(add, erase, answer)` は左右共通の追加・削除処理で実行する。
`run(add_left, add_right, erase_left, erase_right, answer)` は左右を区別する。
各追加・削除関数には配列の添字、answer にはクエリIDを渡す。
run を呼ぶたび、利用者が管理する状態を空区間の状態に初期化しておくこと。
追加・削除O(1)なら、Q件の処理は O(Q log Q + N sqrt(Q) + Q)、メモリ O(Q)。

使用例:
区間和を求める。
```cpp
koma::Mo mo(a.size());
mo.add_query(0, 3);
long long total = 0;
std::vector<long long> ans(1);
mo.run([&](long long i) { total += a[i]; },
	   [&](long long i) { total -= a[i]; },
	   [&](long long id) { ans[id] = total; });
```
*/
class Mo {
	long long n_;
	std::vector<std::pair<long long, long long>> queries_;

  public:
	/** 配列長 n（0以上）で構築する。O(1)。使用例: `koma::Mo mo(100);` */
	explicit Mo(long long n) : n_(n) { assert(n >= 0); }
	/** [l, r) を登録しIDを返す。0 <= l <= r <= n。ならしO(1)。使用例: `mo.add_query(1, 4);` */
	long long add_query(long long l, long long r) {
		assert(0 <= l && l <= r && r <= n_);
		queries_.emplace_back(l, r);
		return static_cast<long long>(queries_.size()) - 1;
	}
	/** 左右別の追加・削除関数と回答関数で実行する。引数・計算量はクラス説明を参照。使用例: `mo.run(al, ar, el, er, answer);` */
	template <class AL, class AR, class EL, class ER, class Answer>
	void run(AL add_left, AR add_right, EL erase_left, ER erase_right, Answer answer) const {
		long long q = static_cast<long long>(queries_.size());
		if(q == 0) return;
		long long width = std::max(1LL, static_cast<long long>(n_ / std::sqrt(static_cast<long double>(q))));
		std::vector<long long> order(q);
		std::iota(order.begin(), order.end(), 0LL);
		std::sort(order.begin(), order.end(), [&](long long a, long long b) {
			auto [la, ra] = queries_[a];
			auto [lb, rb] = queries_[b];
			if(la / width != lb / width) return la / width < lb / width;
			return (la / width & 1) ? ra > rb : ra < rb;
		});
		long long l = 0, r = 0;
		for(auto id : order) {
			auto [nl, nr] = queries_[id];
			while(l > nl) add_left(--l);
			while(r < nr) add_right(r++);
			while(l < nl) erase_left(l++);
			while(r > nr) erase_right(--r);
			answer(id);
		}
	}
	/** 左右共通の追加・削除関数で実行する。使用例: `mo.run(add, erase, answer);` */
	template <class Add, class Erase, class Answer>
	void run(Add add, Erase erase, Answer answer) const { run(add, add, erase, erase, answer); }
};
} // namespace koma
