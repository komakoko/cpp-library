#pragma once
#include <bits/stdc++.h>

namespace koma {

struct SieveResult {
	std::vector<long long> primes;
	std::vector<bool> is_prime;
};

/**
n以下の素数一覧と素数判定表を作る。

引数:
`n`: [1, n] の素数を求める。

返り値:
`SieveResult.primes` : n以下の素数の昇順の配列。
`SieveResult.is_prime` : n以下の整数の素数判定表。1なら素数。

計算量:
時間 O(n log log n)。

使用例:
```cpp
auto result = koma::sieve_eratosthenes(10);
vector<ll>& primes = result.primes; // {2, 3, 5, 7}
vector<bool>& is_prime = result.is_prime;
```
*/
inline SieveResult sieve_eratosthenes(long long n) {
	assert(n >= 0);
	SieveResult result;
	result.is_prime.assign(n + 1, true);
	result.is_prime[0] = false;
	if(n >= 1) result.is_prime[1] = false;
	for(long long i = 2; i <= n; ++i) {
		if(!result.is_prime[i]) continue;
		result.primes.push_back(i);
		if(i <= n / i) {
			for(long long j = i * i; j <= n; j += i) result.is_prime[j] = false;
		}
	}
	return result;
}

// 素因数の数を全て求める
inline std::vector<long long> sieve_primefactor(long long n) {
	assert(n >= 0);
	std::vector<long long> factor_count(n + 1, 0);
	for(long long i = 2; i <= n; i++) {
		if(factor_count[i] == 0) {
			for(long long j = i; j <= n; j += i) factor_count[j]++;
		}
	}
	return factor_count;
}

/**
正整数 n を素因数分解する。

引数:
`n`: 1以上の整数。
`primes` (オーバーロード): 素数一覧。渡すと素因数候補が減って高速になる。

返り値:
素数の昇順の `{素数, 指数}` の配列。n=1なら空配列。

計算量:
時間 O(√n)。

使用例:
```cpp
auto factors = koma::prime_factorization(360); // {{2, 3}, {3, 2}, {5, 1}}
auto result = koma::sieve_eratosthenes(100);
auto factors_with_primes = koma::prime_factorization(360, result.primes); // 同じ結果
```
*/
inline std::vector<std::pair<long long, int>> prime_factorization(long long n) {
	assert(n >= 1);
	std::vector<std::pair<long long, int>> factors;
	int exponent = 0;
	while(n % 2 == 0) {
		n /= 2;
		++exponent;
	}
	if(exponent) factors.emplace_back(2, exponent);
	for(long long p = 3; p <= n / p; p += 2) {
		if(n % p != 0) continue;
		exponent = 0;
		do {
			n /= p;
			++exponent;
		} while(n % p == 0);
		factors.emplace_back(p, exponent);
	}
	if(n > 1) factors.emplace_back(n, 1);
	return factors;
}

/**
指定した素数一覧を使って正整数 n を素因数分解する。

引数:
`n`: 1以上の整数。
`primes`: 昇順・重複なしの素数一覧。元の n の平方根以下の素数をすべて含むこと。

返り値:
素数の昇順の `{素数, 指数}` の配列。n=1なら空配列。

計算量:
時間 O(π(√n) + log n)。π(x) は x 以下の素数の個数。

使用例:
```cpp
auto result = koma::sieve_eratosthenes(100);
auto factors = koma::prime_factorization(360, result.primes); // {{2, 3}, {3, 2}, {5, 1}}
```
*/
inline std::vector<std::pair<long long, int>> prime_factorization(
	long long n, const std::vector<long long> &primes) {
	assert(n >= 1);
	std::vector<std::pair<long long, int>> factors;
	for(long long p : primes) {
		if(p > n / p) break;
		if(n % p != 0) continue;
		int exponent = 0;
		do {
			n /= p;
			++exponent;
		} while(n % p == 0);
		factors.emplace_back(p, exponent);
	}
	if(n > 1) factors.emplace_back(n, 1);
	return factors;
}

} // namespace koma
