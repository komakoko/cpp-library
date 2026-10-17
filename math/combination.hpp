#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
階乗と逆階乗を必要な範囲まで自動で拡張し、組合せ nCk と順列 nPk を求める。
`Mint` は `mod()` と `inv()` を持つ modint 型。法は素数、n は法未満で使用する。
使用中に法を変更した場合は、新しいオブジェクトを作る。

コンストラクタ:
`Comb()`: 引数なし。0! とその逆元を1で初期化する。

関数:
- `comb(n, k)`：組合せ nCk。`operator()(n, k)` も同じ。
- `perm(n, k)`：順列 nPk。
- `fact(n)`：階乗 n!。0! は1。
- `ifact(n)`：階乗 n! の逆元。

各関数は計算済みなら O(1)。最大値を m から n に伸ばすとき O(n-m + log mod)。メモリ O(n)。
`comb` と `perm` は n < 0、k < 0、k > n の場合に0を返す。`fact` と `ifact` は 0 <= n < mod が必要。

使用例:
```cpp
using mint = atcoder::modint998244353;
koma::Comb<mint> comb;
auto a = comb.comb(5, 2); // 10
auto b = comb.perm(5, 2); // 20
auto f = comb.fact(5); // 120
auto inv_f = comb.ifact(5); // 120の逆元
```
*/
template <class Mint>
class Comb {
	int modulus_ = Mint::mod();
	std::vector<Mint> factorial_{Mint(1)};
	std::vector<Mint> inverse_factorial_{Mint(1)};

	void extend(int n) {
		int old = static_cast<int>(factorial_.size());
		if(old <= n) {
			factorial_.resize(n + 1);
			inverse_factorial_.resize(n + 1);
			for(int i = old; i <= n; ++i) factorial_[i] = factorial_[i - 1] * i;
			inverse_factorial_[n] = factorial_[n].inv();
			for(int i = n; i >= old; --i) inverse_factorial_[i - 1] = inverse_factorial_[i] * i;
		}
	}

  public:
	/**
	階乗 n! を計算する。
	引数:
	`n`: 0以上、法未満の整数。

	返り値:
	`Mint` 型の n!。0! は1。

	計算量:
	計算済みなら O(1)。新たに追加する要素数 d に対して O(d + log mod)。

	使用例:
	```cpp
	comb.fact(5); // 120
	```
	*/
	Mint fact(int n) {
		assert(Mint::mod() == modulus_);
		assert(0 <= n && n < modulus_);
		extend(n);
		return factorial_[n];
	}

	/**
	逆階乗 1/n! を法の下で計算する。
	引数:
	`n`: 0以上、法未満の整数。

	返り値:
	`Mint` 型の n! の逆元。n=0 の場合は1。

	計算量:
	計算済みなら O(1)。新たに追加する要素数 d に対して O(d + log mod)。

	使用例:
	```cpp
	comb.ifact(5); // 120の逆元
	```
	*/
	Mint ifact(int n) {
		assert(Mint::mod() == modulus_);
		assert(0 <= n && n < modulus_);
		extend(n);
		return inverse_factorial_[n];
	}

	/**
	組合せ nCk を計算する。
	引数:
	`n`, `k`: 選ぶ元の個数と選ぶ個数。正常な入力では n < mod を満たすこと。

	返り値:
	`Mint` 型の nCk。n < 0、k < 0、k > n の場合は0。

	計算量:
	計算済みなら O(1)。新たに追加する要素数 d に対して O(d + log mod)。

	使用例:
	```cpp
	comb.comb(5, 2); // 10
	comb.comb(2, 3); // 0
	```
	*/
	Mint comb(int n, int k) {
		assert(Mint::mod() == modulus_);
		if(n < 0 || k < 0 || n < k) return Mint(0);
		assert(n < modulus_);
		extend(n);
		return factorial_[n] * inverse_factorial_[k] * inverse_factorial_[n - k];
	}

	/**
	順列 nPk を計算する。
	引数:
	`n`, `k`: 選ぶ元の個数と、順番を付けて選ぶ個数。正常な入力では n < mod を満たすこと。

	返り値:
	`Mint` 型の nPk。n < 0、k < 0、k > n の場合は0。

	計算量:
	計算済みなら O(1)。新たに追加する要素数 d に対して O(d + log mod)。

	使用例:
	```cpp
	comb.perm(5, 2); // 20
	comb.perm(0, 0); // 1
	```
	*/
	Mint perm(int n, int k) {
		assert(Mint::mod() == modulus_);
		if(n < 0 || k < 0 || n < k) return Mint(0);
		assert(n < modulus_);
		extend(n);
		return factorial_[n] * inverse_factorial_[n - k];
	}

	/** `comb(n, k)` の呼び出しでも組合せを計算する。 */
	Mint operator()(int n, int k) { return comb(n, k); }
};

namespace detail {
/** Mint型ごとに、関数版のcomb・perm・fact・ifactが共有する計算器を返す。 */
template <class Mint>
inline Comb<Mint> &shared_combination() {
	static Comb<Mint> calculator;
	return calculator;
}
} // namespace detail

/**
組合せ nCkを求める。同じMint型の関数版comb・perm・fact・ifactで階乗と逆階乗を共有する。
Mintはmod()とinv()を持つmodint型。法は素数とし、初回呼び出し後に変更しないこと。
引数:
`n`, `k`: 元の個数と選ぶ個数。n < 0、k < 0、k > n では0。それ以外は n < Mint::mod()。

返り値:
Mint型の組合せ nCk。

計算量:
計算済みなら O(1)。追加する要素数 d に対して O(d + log mod)。共有配列のメモリ O(N)。

使用例:
```cpp
using mint = atcoder::modint998244353;
auto x = koma::comb<mint>(5, 2); // 10
```
*/
template <class Mint>
inline Mint comb(int n, int k) {
	return detail::shared_combination<Mint>().comb(n, k);
}

/**
順列 nPkを求める。同じMint型の関数版comb・perm・fact・ifactで階乗と逆階乗を共有する。
Mintはmod()とinv()を持つmodint型。法は素数とし、初回呼び出し後に変更しないこと。
引数:
`n`, `k`: 元の個数と選ぶ個数。n < 0、k < 0、k > n では0。それ以外は n < Mint::mod()。

返り値:
Mint型の順列 nPk。

計算量:
計算済みなら O(1)。追加する要素数 d に対して O(d + log mod)。共有配列のメモリ O(N)。

使用例:
```cpp
using mint = atcoder::modint998244353;
auto x = koma::perm<mint>(5, 2); // 20
```
*/
template <class Mint>
inline Mint perm(int n, int k) {
	return detail::shared_combination<Mint>().perm(n, k);
}

/**
階乗 n!を求める。同じMint型の関数版comb・perm・fact・ifactで階乗と逆階乗を共有する。
Mintはmod()とinv()を持つmodint型。法は素数とし、初回呼び出し後に変更しないこと。
引数:
`n`: 0以上 Mint::mod() 未満。

返り値:
Mint型の階乗 n!。

計算量:
計算済みなら O(1)。追加する要素数 d に対して O(d + log mod)。共有配列のメモリ O(N)。

使用例:
```cpp
using mint = atcoder::modint998244353;
auto x = koma::fact<mint>(5); // 120
```
*/
template <class Mint>
inline Mint fact(int n) {
	return detail::shared_combination<Mint>().fact(n);
}

/**
逆階乗 1/n!を求める。同じMint型の関数版comb・perm・fact・ifactで階乗と逆階乗を共有する。
Mintはmod()とinv()を持つmodint型。法は素数とし、初回呼び出し後に変更しないこと。
引数:
`n`: 0以上 Mint::mod() 未満。

返り値:
Mint型の逆階乗 1/n!。

計算量:
計算済みなら O(1)。追加する要素数 d に対して O(d + log mod)。共有配列のメモリ O(N)。

使用例:
```cpp
using mint = atcoder::modint998244353;
auto x = koma::ifact<mint>(5); // 120の逆元
```
*/
template <class Mint>
inline Mint ifact(int n) {
	return detail::shared_combination<Mint>().ifact(n);
}

} // namespace koma
