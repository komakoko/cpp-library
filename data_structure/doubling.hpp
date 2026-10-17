#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
同じ遷移を繰り返したときの、k回後の頂点をダブリングで求める。
頂点番号は0始まり。遷移先がない場合は -1 を指定する。

コンストラクタ:
- `Doubling()`: 空のオブジェクトを作る。問い合わせ前に build が必要。
- `Doubling(nxt, max_k)`: nxt は遷移先の `std::vector<long long>`、max_k は問い合わせる最大回数（0以上）。

関数:
- `build(nxt, max_k)`: 遷移表を構築し直す。引数なしで作った場合は、問い合わせ前に呼ぶ。
- `jump(v, k)`: 頂点 v から k 回遷移した頂点を返す。途中で遷移先がなくなれば -1。

`max_k` は0以上、`long long` の最大値まで。問い合わせは 0 <= k <= max_k。
段数 L = max(1, max_k のビット数) として、構築 O(NL)、メモリ O(NL)、問い合わせ O(L)。

使用例:
```cpp
koma::Doubling cycle({1, 2, 0}, 1000000000000LL);
cycle.jump(0, 5); // 2
cycle.jump(1, 0); // 1

koma::Doubling chain({1, -1}, 10);
chain.jump(0, 2); // -1
```
*/
class Doubling {
	long long n_ = 0;
	long long max_k_ = 0;
	bool built_ = false;
	std::vector<std::vector<long long>> up_;

  public:
	/** 空のオブジェクトを作る。jump の前に build を呼ぶ。 */
	Doubling() = default;

	/**
	遷移表を構築する。
	引数:
	`nxt`: 各頂点の1回後の遷移先を格納した `std::vector<long long>`。
	頂点番号は0始まり。値は -1（遷移先なし）、または0以上N未満。
	`max_k`: 問い合わせる最大回数。0以上の `long long`。
	`jump(v, k)` は 0 <= k <= max_k の範囲で使用する。

	計算量:
	頂点数N、L = max(1, max_k のビット数) として、時間・メモリ O(NL)。

	使用例:
	```cpp
	koma::Doubling db({1, 2, 0}, 100);
	db.jump(0, 5); // 2
	```
	*/
	Doubling(const std::vector<long long> &nxt, long long max_k) { build(nxt, max_k); }

	/**
	遷移表を構築する。既存の表は置き換える。
	引数:
	`nxt`: 各頂点の1回後の遷移先。値は -1 または0以上N未満。
	`max_k`: 問い合わせる最大回数。0以上の long long。

	返り値:
	なし。

	計算量:
	L = max(1, max_k のビット数) として、時間・メモリ O(NL)。

	使用例:
	```cpp
	koma::Doubling db;
	db.build({1, 2, 0}, 100);
	```
	*/
	void build(const std::vector<long long> &nxt, long long max_k) {
		assert(max_k >= 0);
		assert(nxt.size() <= static_cast<std::size_t>(std::numeric_limits<long long>::max()));
		for(long long v : nxt) assert(-1 <= v && v < static_cast<long long>(nxt.size()));
		long long n = static_cast<long long>(nxt.size());
		long long levels = std::max(1LL, static_cast<long long>(std::bit_width(static_cast<unsigned long long>(max_k))));
		std::vector<std::vector<long long>> table(levels, std::vector<long long>(n, -1));
		table[0] = nxt;
		for(long long j = 1; j < levels; ++j) {
			for(long long v = 0; v < n; ++v) {
				long long p = table[j - 1][v];
				table[j][v] = p == -1 ? -1 : table[j - 1][p];
			}
		}
		up_ = std::move(table);
		n_ = n;
		max_k_ = max_k;
		built_ = true;
	}

	/**
	頂点 v から k 回遷移した先を求める。
	引数:
	`v`: 始点の頂点番号。0以上N未満、または遷移先なしを表す -1。
	`k`: 遷移回数。0以上、構築時の max_k 以下。

	返り値:
	遷移先の頂点番号（long long）。途中で遷移先がなくなれば -1。
	k=0 なら v をそのまま返す。構築済みのオブジェクトで使用する。

	計算量:
	L = max(1, max_k のビット数) として O(L)。

	使用例:
	```cpp
	koma::Doubling db({1, 2, 0}, 100);
	db.jump(0, 5); // 2
	```
	*/
	long long jump(long long v, long long k) const {
		assert(built_);
		assert(0 <= k && k <= max_k_);
		assert(-1 <= v && v < n_);
		for(long long j = 0; k != 0 && v != -1; ++j, k >>= 1) {
			if(k & 1) v = up_[j][v];
		}
		return v;
	}
};

} // namespace koma
