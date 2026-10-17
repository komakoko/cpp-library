#pragma once
#include <bits/stdc++.h>

namespace koma {

/**
SplitMix64による高速な疑似乱数生成器。

コンストラクタ:
`FastRand(seed_value = 0)`: 初期シードを指定する。同じシードなら同じ列を生成する。

関数:
`operator()()`: 64ビット符号なし整数を返す。O(1)。
`operator()(n)`: 半開区間 [0, n) の整数を返す。n > 0。
`operator()(l, r)`: 半開区間 [l, r) の整数を返す。l < r。
範囲指定時は剰余による偏りを除き、期待 O(1)。不正な範囲では `std::invalid_argument`。
`seed(value)`: シードを再設定する。O(1)。
`min()`, `max()`: 生成可能な最小値・最大値。標準の乱数生成器としても利用できる。

使用例:
```cpp
koma::FastRand rng(42);
auto x = rng(10);     // 0〜9
auto y = rng(-5, 6);  // -5〜5
std::shuffle(v.begin(), v.end(), rng);
auto z = koma::fastrand(100); // 共通の生成器も利用可能
```
*/
class FastRand {
  public:
	using result_type = std::uint64_t;

	explicit FastRand(result_type seed_value = 0) : state_(seed_value) {}

	/** 生成器の状態を指定したシードに戻す。 */
	void seed(result_type value = 0) { state_ = value; }

	/** 生成可能な最小値を返す。 */
	static constexpr result_type min() { return 0; }

	/** 生成可能な最大値を返す。 */
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

	/** 次の64ビット符号なし疑似乱数を返す。O(1)。 */
	result_type operator()() {
		result_type z = (state_ += 0x9e3779b97f4a7c15ULL);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
		return z ^ (z >> 31);
	}

	/** [0, n) の整数を返す。n > 0。期待 O(1)。 */
	long long operator()(long long n) { return (*this)(0, n); }

	/** [l, r) の整数を返す。l < r。期待 O(1)。 */
	long long operator()(long long l, long long r) {
		if(l >= r) throw std::invalid_argument("fastrand: expected l < r");
		return std::uniform_int_distribution<long long>(l, r - 1)(*this);
	}

  private:
	result_type state_;
};

/** 共通の高速疑似乱数生成器。初期シードは0。`fastrand.seed(42)` で再設定できる。 */
inline FastRand fastrand;

} // namespace koma
